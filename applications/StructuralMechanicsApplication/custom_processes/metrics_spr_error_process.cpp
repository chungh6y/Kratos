// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:		 BSD License
//					 license: structural_mechanics_application/license.txt
//
//  Main authors:    Anna Rehr
//  Co-author   :    Vicente Mataix Ferrandiz
//

// System includes

// External includes

// Project includes
#include "custom_processes/metrics_spr_error_process.h"
#include "utilities/variable_utils.h"

namespace Kratos
{
template<SizeType TDim>
SPRMetricProcess<TDim>::SPRMetricProcess(
    ModelPart& rThisModelPart,
    Parameters ThisParameters
    ): mThisModelPart(rThisModelPart)
{               
    Parameters default_parameters = Parameters(R"(
    {
        "penalty_normal"                      : 1.0e4,
        "penalty_tangential"                  : 1.0e4,
        "echo_level"                          : 0
    })" 
    );
    
    ThisParameters.ValidateAndAssignDefaults(default_parameters);
    
    mPenaltyNormal = ThisParameters["penalty_normal"].GetDouble();
    mPenaltyTangent = ThisParameters["penalty_tangential"].GetDouble();
    mEchoLevel = ThisParameters["echo_level"].GetInt();
}
    
/***********************************************************************************/
/***********************************************************************************/
    
template<SizeType TDim>
void SPRMetricProcess<TDim>::Execute()
{
    // Getting process info
    ProcessInfo::Pointer p_process_info = mThisModelPart.pGetProcessInfo();

    // Some initializations
    VariableUtils().SetNonHistoricalVariable(ELEMENT_ERROR, 0.0, mThisModelPart.Elements());
    VariableUtils().SetNonHistoricalVariable(ELEMENT_H, 0.0, mThisModelPart.Elements());

    /************************************************************************
    --1-- Calculate superconvergent stresses (at the nodes) --1--
    ************************************************************************/
    CalculateSuperconvergentStresses();

    /******************************************************************************
    --2-- Calculate error estimation --2--
    ******************************************************************************/
    double energy_norm_overall = 0.0;
    double error_overall = 0.0;
    CalculateErrorEstimation(energy_norm_overall, error_overall);

    // Final calculations
    const double tolerance = std::numeric_limits<double>::epsilon();
    const double denominator = std::sqrt(std::pow(error_overall, 2) + std::pow(energy_norm_overall , 2));
    const double coeff = denominator < tolerance ? 1.0 : 1.0/denominator;
    KRATOS_WARNING_IF("SPRMetricProcess", denominator < tolerance) << "Denominator of error estimate zero or almost zero " << denominator << std::endl;
    p_process_info->SetValue(ENERGY_NORM_OVERALL, energy_norm_overall);
    p_process_info->SetValue(ERROR_OVERALL, error_overall);
    p_process_info->SetValue(ERROR_RATIO, coeff * error_overall);
}

/***********************************************************************************/
/***********************************************************************************/

template<SizeType TDim>
void SPRMetricProcess<TDim>::CalculateSuperconvergentStresses()
{
    FindNodalNeighboursProcess find_neighbours(mThisModelPart);
    find_neighbours.Execute();

    // Iteration over all nodes -- construction of patches
    NodesArrayType& nodes_array = mThisModelPart.Nodes();
    const int num_nodes = static_cast<int>(nodes_array.size());

    #pragma omp parallel for
    for(int i_node = 0; i_node < num_nodes; ++i_node) {
        auto it_node = nodes_array.begin() + i_node;

        KRATOS_DEBUG_ERROR_IF_NOT(it_node->Has(NEIGHBOUR_ELEMENTS)) << "SPRMetricProcess:: Search didn't work with elements" << std::endl;
        const SizeType neighbour_size = it_node->GetValue(NEIGHBOUR_ELEMENTS).size();

        Vector sigma_recovered(SigmaSize, 0.0);

        if(neighbour_size > TDim) {
            CalculatePatch(it_node, it_node, neighbour_size,sigma_recovered);
            it_node->SetValue(RECOVERED_STRESS, sigma_recovered);

            KRATOS_INFO_IF("SPRMetricProcess", mEchoLevel > 2) << "Recovered sigma: " << sigma_recovered << std::endl;
        } else {
            KRATOS_DEBUG_ERROR_IF_NOT(it_node->Has(NEIGHBOUR_NODES)) << "SPRMetricProcess:: Search didn't work with nodes" << std::endl;
            auto& neigh_nodes = it_node->GetValue(NEIGHBOUR_NODES);
            for(auto it_neighbour_nodes = neigh_nodes.begin(); it_neighbour_nodes != neigh_nodes.end(); it_neighbour_nodes++) {

                Vector sigma_recovered_i(SigmaSize,0);

                IndexType count_i = 0;
                for(IndexType i_node_loop = 0; i_node_loop < num_nodes; ++i_node_loop) { // FIXME: Avoid this double loop, extreamily expensive
                    auto it_node_loop = nodes_array.begin() + i_node_loop;
                    const SizeType size_elem_neigh = it_node_loop->GetValue(NEIGHBOUR_ELEMENTS).size();
                    if (it_node_loop->Id() == it_neighbour_nodes->Id() && size_elem_neigh > TDim){
                        CalculatePatch(it_node, it_node_loop, neighbour_size, sigma_recovered_i);
                        ++count_i;
                    }
                }

                // Average solution from different patches
                if(count_i != 0)
                    sigma_recovered = sigma_recovered*(count_i-1)/count_i + sigma_recovered_i/count_i;
            }

            it_node->SetValue(RECOVERED_STRESS,sigma_recovered);
            KRATOS_INFO_IF("SPRMetricProcess", mEchoLevel > 2) << "Recovered sigma: " << sigma_recovered << std::endl;
        }
    }
}

/***********************************************************************************/
/***********************************************************************************/

template<SizeType TDim>
void SPRMetricProcess<TDim>::CalculateErrorEstimation(
    double& rEnergyNormOverall,
    double& rErrorOverall
    )
{
    // Loop over all elements:
    ElementsArrayType& elements_array = mThisModelPart.Elements();
    const int num_elem = static_cast<int>(elements_array.size());

    // Compute the error estimate per element
    double error_overall= 0.0;
    double energy_norm_overall = 0.0;
    #pragma omp parallel for reduction(+:error_overall, energy_norm_overall)
    for(int i_elem = 0; i_elem < num_elem; ++i_elem){
        auto it_elem = elements_array.begin() + i_elem;

        std::vector<double> error_integration_point;
        const auto& process_info = mThisModelPart.GetProcessInfo();
        it_elem->GetValueOnIntegrationPoints(ERROR_INTEGRATION_POINT, error_integration_point, process_info);

        KRATOS_INFO_IF("SPRMetricProcess", mEchoLevel > 2) << "Error GP:" << error_integration_point << std::endl;
        
        // We compute the error overall 
        double error_energy_norm = 0.0;
        for(IndexType i = 0;i < error_integration_point.size();++i)
            error_energy_norm += error_integration_point[i];
        error_overall += error_energy_norm;
        error_energy_norm = std::sqrt(error_energy_norm);
        it_elem->SetValue(ELEMENT_ERROR, error_energy_norm);


        // We compute now the energy norm
        std::vector<double> strain_energy;
        it_elem->GetValueOnIntegrationPoints(STRAIN_ENERGY, strain_energy, process_info);

        double energy_norm = 0.0;
        for(IndexType i = 0;i < strain_energy.size(); ++i)
            energy_norm += 2.0 * strain_energy[i];
        energy_norm_overall += energy_norm;
        energy_norm= std::sqrt(energy_norm);

        KRATOS_INFO_IF("SPRMetricProcess", mEchoLevel > 2) << "Element Id:" << it_elem->Id() << ". Element error: " << error_energy_norm << ". Energy norm: " << energy_norm << std::endl;
    }

    rErrorOverall = std::sqrt(error_overall);
    rEnergyNormOverall = std::sqrt(energy_norm_overall);
    double error_percentage = rErrorOverall/std::sqrt((std::pow(rErrorOverall, 2) + std::pow(rEnergyNormOverall, 2)));

    KRATOS_INFO_IF("SPRMetricProcess", mEchoLevel > 1)
        << "Overall error norm: " << rErrorOverall << std::endl
        << "Overall energy norm: "<< rEnergyNormOverall << std::endl
        << "Error in percent: " << error_percentage << std::endl;
}

/***********************************************************************************/
/***********************************************************************************/

template<SizeType TDim>
void SPRMetricProcess<TDim>::CalculatePatch(
    NodeItType itNode,
    NodeItType itPatchNode,
    SizeType NeighbourSize,
    Vector& rSigmaRecovered
    )
{
    // Determine if contact BC has to be regarded
    // We take the geometry GP from the core
    const double tolerance = std::numeric_limits<double>::epsilon();
    const bool regard_contact = itNode->Has(CONTACT_PRESSURE) ? (std::abs(itNode->GetValue(CONTACT_PRESSURE)) > tolerance ? true : false) : false;
    
    // NOTE: Code commented. Could be of interest in the future
//     regard_contact = itPatchNode->Has(CONTACT_PRESSURE);
//     if(regard_contact == false) {
//         for( auto& i_neighbour_nodes : itPatchNode->GetValue(NEIGHBOUR_NODES)) {
//             if (i_neighbour_nodes.Has(CONTACT_PRESSURE)) {
//                 regard_contact = true;
//                 break;
//             }
//         }
//     }
    
    if (regard_contact == false)
        CalculatePatchStandard(itNode, itPatchNode, NeighbourSize, rSigmaRecovered);
    else
        CalculatePatchContact(itNode, itPatchNode, NeighbourSize, rSigmaRecovered);
}

/***********************************************************************************/
/***********************************************************************************/
    
template<SizeType TDim>
void SPRMetricProcess<TDim>::CalculatePatchStandard(
    NodeItType itNode,
    NodeItType itPatchNode,
    SizeType NeighbourSize,
    Vector& rSigmaRecovered
    )
{
    // Triangle and tetrahedra have only one GP by default
    std::vector<Vector> stress_vector(1);
    std::vector<array_1d<double,3>> coordinates_vector(1);

    // Our interest is to assemble the system A and b to solve a local problem for the element and estimate the new element size
    BoundedMatrix<double, TDim + 1, TDim + 1> A = ZeroMatrix(TDim + 1,TDim + 1);
    BoundedMatrix<double, TDim + 1, SigmaSize> b = ZeroMatrix(TDim + 1,SigmaSize);
    BoundedMatrix<double, 1, TDim + 1> p_k;
    
    auto& neigh_elements = itPatchNode->GetValue(NEIGHBOUR_ELEMENTS);
    for( WeakElementItType it_elem = neigh_elements.begin(); it_elem != neigh_elements.end(); ++it_elem) {
        
        it_elem->GetValueOnIntegrationPoints(CAUCHY_STRESS_VECTOR,stress_vector,mThisModelPart.GetProcessInfo());
        it_elem->GetValueOnIntegrationPoints(INTEGRATION_COORDINATES,coordinates_vector,mThisModelPart.GetProcessInfo());

        KRATOS_INFO_IF("SPRMetricProcess", mEchoLevel > 3)
        << "\tStress: " << stress_vector[0] << std::endl
        << "\tx: " << coordinates_vector[0][0] << "\ty: " << coordinates_vector[0][1] << "\tz_coordinate: " << coordinates_vector[0][2] << std::endl;
        
        BoundedMatrix<double, 1, SigmaSize> sigma;
        for(IndexType j = 0; j < SigmaSize; ++j)
            sigma(0,j) = stress_vector[0][j];
        p_k(0,0) = 1.0;
        p_k(0,1) = coordinates_vector[0][0] - itPatchNode->X();
        p_k(0,2) = coordinates_vector[0][1] - itPatchNode->Y();
        if(TDim == 3)
            p_k(0,3)=coordinates_vector[0][2] - itPatchNode->Z();
        
        // Finally we add the contributiosn to our local system (A, b)
        noalias(A) += prod(trans(p_k), p_k);
        noalias(b) += prod(trans(p_k), sigma);
    }
    
    double det;
    BoundedMatrix<double, TDim + 1, TDim + 1> invA = MathUtils<double>::InvertMatrix<TDim + 1>(A, det, -1.0); // We consider a negative tolerance in order to avoid error

    KRATOS_INFO_IF("SPRMetricProcess", mEchoLevel > 3) << A << std::endl << invA << std::endl << det<< std::endl;

    // We do a little correction trick in case of almost zero determinant
    const double tolerance = std::numeric_limits<double>::epsilon();
    if(det < tolerance){
        KRATOS_WARNING_IF("SPRMetricProcess", mEchoLevel == 2) << A << std::endl;
        for( IndexType i = 0; i < TDim + 1;i++){
            for( IndexType j = 0; j < TDim + 1; j++)
                A(i,j) += 0.001;
        }
        invA = MathUtils<double>::InvertMatrix<TDim + 1>(A,det);
        KRATOS_WARNING_IF("SPRMetricProcess", mEchoLevel > 0) << "det: " << det << std::endl;
    }

    const BoundedMatrix<double, TDim + 1, SigmaSize> coeff = prod(invA, b);
    
    if(NeighbourSize > TDim) {
        noalias(rSigmaRecovered) = row(coeff, 0);
    } else {
        p_k(0,1) = itNode->X() - itPatchNode->X();
        p_k(0,2) = itNode->Y() - itPatchNode->Y();
        if(TDim ==3)
            p_k(0,3) = itNode->Z() - itPatchNode->Z();
        const BoundedMatrix<double, 1, SigmaSize> sigma = prod(p_k,coeff);
        noalias(rSigmaRecovered) = row(sigma, 0);
    }
}

/***********************************************************************************/
/***********************************************************************************/

template<SizeType TDim>
void SPRMetricProcess<TDim>::CalculatePatchContact(
    NodeItType itNode,
    NodeItType itPatchNode,
    SizeType NeighbourSize,
    Vector& rSigmaRecovered)
{
    // Triangle and tetrahedra have only one GP by default
    std::vector<Vector> stress_vector(1);
    std::vector<array_1d<double,3>> coordinates_vector(1);

    // Our interest is to assemble the system A and b to solve a local problem for the element and estimate the new element size
    CompressedMatrix A(SigmaSize * (TDim + 1), SigmaSize * (TDim + 1), 0.0);
    BoundedMatrix<double, SigmaSize * (TDim+1),1> b = ZeroMatrix(SigmaSize * (TDim+1), 1);
    BoundedMatrix<double, SigmaSize, SigmaSize * (TDim+1)> p_k;
    BoundedMatrix<double, 1, SigmaSize> N_k;
    BoundedMatrix<double, 1, SigmaSize> T_k1;
    BoundedMatrix<double, 1, SigmaSize> T_k2;  // in case of 3D: second tangential vector
    BoundedMatrix<double, SigmaSize, 1> sigma;
    
    /* Computation A and b */
    // PART 1: Contributions from the neighboring elements
    auto& neigh_elements = itPatchNode->GetValue(NEIGHBOUR_ELEMENTS);
    for( WeakElementItType it_elem = neigh_elements.begin(); it_elem != neigh_elements.end(); ++it_elem) {
        
        auto& process_info = mThisModelPart.GetProcessInfo();
        it_elem->GetValueOnIntegrationPoints(CAUCHY_STRESS_VECTOR,stress_vector, process_info);
        it_elem->GetValueOnIntegrationPoints(INTEGRATION_COORDINATES,coordinates_vector, process_info);

        KRATOS_INFO_IF("SPRMetricProcess", mEchoLevel > 3)
        << "\tElement: " << it_elem->Id() << std::endl
        << "\tStress: " << stress_vector[0] << std::endl
        << "\tX: " << coordinates_vector[0][0] << "\tY: " << coordinates_vector[0][1] << "\tZ: " << coordinates_vector[0][2] << std::endl;
        
        for( IndexType j = 0; j < SigmaSize; ++j)
            sigma(j,0) = stress_vector[0][j];
        
        for ( IndexType j = 0; j < SigmaSize; ++j){
            p_k(j, j * (TDim+1) + 0) = 1.0;
            p_k(j, j * (TDim+1) + 1) = coordinates_vector[0][0] - itPatchNode->X();
            p_k(j, j * (TDim+1) + 2) = coordinates_vector[0][1] - itPatchNode->Y();
            if(TDim == 3)
                p_k(j, j * (TDim+1) + 3) = coordinates_vector[0][2] - itPatchNode->Z();
        }
        
        // Finally we add the contributiosn to our local system (A, b)
        noalias(A) += prod(trans(p_k), p_k);
        noalias(b) += prod(trans(p_k), sigma);
    }
    
    // Computing A and b
    BoundedMatrix<double, SigmaSize * (TDim + 1), 1> A1;
    BoundedMatrix<double, 1, SigmaSize * (TDim + 1)> A2;
    for (IndexType j = 0; j < SigmaSize; ++j){
        p_k(j,j * (TDim + 1) + 1)= itNode->X() - itPatchNode->X();
        p_k(j,j * (TDim + 1) + 2)= itNode->Y() - itPatchNode->Y();
        if(TDim == 3)
            p_k(j,j * (TDim + 1) + 3)= itNode->Z() - itPatchNode->Z();
    }
    
    // Set the normal and tangential vectors in Voigt Notation
    const array_1d<double, 3>& normal = itNode->GetValue(NORMAL);

    if(TDim == 2) {
        N_k(0,0) = normal[0] * normal[0];
        N_k(0,1) = normal[1] * normal[1];
        N_k(0,2) = 2.0 * normal[0] * normal[1];
        
        T_k1(0,0) =  normal[0] * normal[1];
        T_k1(0,1) = -normal[0] * normal[1];
        T_k1(0,2) =  normal[1] * normal[1] - normal[0] * normal[0];
    } else if (TDim == 3) {
        N_k(0,0) = normal[0] * normal[0];
        N_k(0,1) = normal[1] * normal[1];
        N_k(0,1) = normal[2] * normal[2];
        N_k(0,3) = 2.0 * normal[1] * normal[2];
        N_k(0,4) = 2.0 * normal[2] * normal[0];
        N_k(0,5) = 2.0 * normal[0] * normal[1];

        // Set tangential vectors
        array_1d<double, 3> t1(3, 0.0);
        array_1d<double, 3> t2(3, 0.0);

        const double tolerance = std::numeric_limits<double>::epsilon();
        if(std::abs(normal[0]) > tolerance || std::abs(normal[1]) > tolerance) {
            const double norm = std::sqrt((t1[0]*t1[0]+t1[1]*t1[1]));
            t1[0] = normal[1]/norm;
            t1[1] = -normal[0]/norm;

            t2[0] = -normal[0]*normal[2]/norm;
            t2[1] = -normal[1]*normal[2]/norm;
            t2[2] = normal[0]*normal[0]+normal[1]*normal[1]/norm;
        } else{
            t1[0] = 1.0;
            t2[1] = 1.0;
        }

        T_k1(0,0) = normal[0]*t1[0];
        T_k1(0,1) = normal[1]*t1[1];
        T_k1(0,2) = normal[2]*t1[2];
        T_k1(0,3) = normal[1]*t1[2]+normal[2]*t1[1];
        T_k1(0,4) = normal[2]*t1[0]+normal[0]*t1[2];
        T_k1(0,5) = normal[0]*t1[1]+normal[1]*t1[0];
        
        T_k2(0,0) = normal[0]*t2[0];
        T_k2(0,1) = normal[1]*t2[1];
        T_k2(0,2) = normal[2]*t2[2];
        T_k2(0,3) = normal[1]*t2[2]+normal[2]*t2[1];
        T_k2(0,4) = normal[2]*t2[0]+normal[0]*t2[2];
        T_k2(0,5) = normal[0]*t2[1]+normal[1]*t2[0];
    }
    
    noalias(A1) = prod(trans(p_k),trans(N_k));
    noalias(A2) = prod(N_k,p_k);
    noalias(A) += mPenaltyNormal * prod(A1, A2);

    noalias(A1) = prod(trans(p_k),trans(T_k1));
    noalias(A2) = prod(T_k1,p_k);

    // Finally we add the contributiosn to our local system (A, b)
    noalias(A) += mPenaltyTangent*prod(A1, A2);
    noalias(b) += mPenaltyNormal*prod(trans(p_k),trans(N_k)) * itNode->GetValue(CONTACT_PRESSURE);

    //PART 2: Contributions from contact nodes: regard all nodes from the patch which are in contact
    // Patch center node:
    if (itPatchNode->Has(CONTACT_PRESSURE)){
        const array_1d<double, 3>& normal_patch_node = itPatchNode->GetValue(NORMAL);
        p_k(0,1)=0.0;
        p_k(0,2)=0.0;
        p_k(1,4)=0.0;
        p_k(1,5)=0.0;
        p_k(2,7)=0.0;
        p_k(2,8)=0.0;
        N_k(0,0) = normal_patch_node[0]*normal_patch_node[0];
        N_k(0,1) = normal_patch_node[1]*normal_patch_node[1];
        N_k(0,2) = 2*normal_patch_node[0]*normal_patch_node[1];
        T_k1(0,0) = normal_patch_node[0]*normal_patch_node[1];
        T_k1(0,1) = -normal_patch_node[0]*normal_patch_node[1];
        T_k1(0,2) = normal_patch_node[1]*normal_patch_node[1]-normal_patch_node[0]*normal_patch_node[0];

        noalias(A1) = prod(trans(p_k),trans(N_k));
        noalias(A2) = prod(N_k,p_k);
        noalias(A) += mPenaltyNormal*prod(A1, A2);

        noalias(A1) = prod(trans(p_k),trans(T_k1));
        noalias(A2) = prod(T_k1,p_k);

        // Finally we add the contributiosn to our local system (A, b)
        noalias(A) += mPenaltyTangent*prod(A1, A2);
        // NOTE: Code commented. Could be of interest in the future
//         noalias(A) += mPenaltyNormal*prod(prod(trans(p_k),trans(N_k)),prod(N_k,p_k));
//         noalias(A) += mPenaltyTangent*prod(prod(prod(trans(p_k),trans(T_k1)),T_k1),p_k);
        noalias(b) -= mPenaltyNormal*prod(trans(p_k),trans(N_k))*itPatchNode->GetValue(CONTACT_PRESSURE);
    }

    // Neighboring nodes:
    for( auto& i_neighbour_node : itPatchNode->GetValue(NEIGHBOUR_NODES)) {
        if (i_neighbour_node.Has(CONTACT_PRESSURE)){
            const array_1d<double, 3>& normal_neigh_node = i_neighbour_node.GetValue(NORMAL);
            const double x_patch = itPatchNode->X();
            const double y_patch = itPatchNode->Y();
            const double x_neigh = i_neighbour_node.X();
            const double y_neigh = i_neighbour_node.Y();
            p_k(0,1)= x_neigh-x_patch;
            p_k(0,2)= y_neigh-y_patch;
            p_k(1,4)= x_neigh-x_patch;
            p_k(1,5)= y_neigh-y_patch;
            p_k(2,7)= x_neigh-x_patch;
            p_k(2,8)= y_neigh-y_patch;
            N_k(0,0) = normal_neigh_node[0]*normal_neigh_node[0];
            N_k(0,1) = normal_neigh_node[1]*normal_neigh_node[1];
            N_k(0,2) = 2*normal_neigh_node[0]*normal_neigh_node[1];
            T_k1(0,0) = normal_neigh_node[0]*normal_neigh_node[1];
            T_k1(0,1) = -normal_neigh_node[0]*normal_neigh_node[1];
            T_k1(0,2) = normal_neigh_node[1]*normal_neigh_node[1]-normal_neigh_node[0]*normal_neigh_node[0];

            noalias(A1) = prod(trans(p_k),trans(N_k));
            noalias(A2) = prod(N_k,p_k);
            noalias(A) += mPenaltyNormal*prod(A1, A2);

            noalias(A1) = prod(trans(p_k),trans(T_k1));
            noalias(A2) = prod(T_k1,p_k);

            // Finally we add the contributiosn to our local system (A, b)
            noalias(A) += mPenaltyTangent*prod(A1, A2);
            noalias(b) += mPenaltyNormal*prod(trans(p_k),trans(N_k))*i_neighbour_node.GetValue(CONTACT_PRESSURE);
        }
    }

    // Computing coefficients a: A*a=b
    KRATOS_INFO_IF("SPRMetricProcess", mEchoLevel > 3) << A << std::endl;
    
    Vector coeff(SigmaSize * (TDim+1));
    Vector b_vector = row(b, 0);
    Matrix invA;
    double det;
    MathUtils<double>::InvertMatrix(A, invA, det);
    noalias(coeff) = prod(invA, b_vector);

    for (IndexType j = 0; j < SigmaSize;++j){
        p_k(j,j*(TDim + 1) + 1) = itNode->X() - itPatchNode->X();
        p_k(j,j*(TDim + 1) + 2) = itNode->Y() - itPatchNode->Y();
        if (TDim == 3)
            p_k(j,j*(TDim + 1) + 3) = itNode->Z() - itPatchNode->Z();
    }
    
    BoundedMatrix<double, SigmaSize*(TDim + 1), 1> coeff_matrix;
    for (IndexType i=0; i<SigmaSize*(TDim + 1); ++i)
        coeff_matrix(i, 0) = coeff[i];
    
    // Asssign the stress recovered
    noalias(sigma) = prod(p_k, coeff_matrix);
    noalias(rSigmaRecovered) = row(sigma,0);
    
    KRATOS_INFO_IF("SPRMetricProcess", mEchoLevel > 1) <<" Recovered pressure: "<< prod(N_k,sigma) <<", LM: "<<itNode->GetValue(CONTACT_PRESSURE)<<std::endl;
}

/***********************************************************************************/
/***********************************************************************************/

template class SPRMetricProcess<2>;
template class SPRMetricProcess<3>;

};// namespace Kratos.
