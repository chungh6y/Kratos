//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ \.
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics FemDem Application
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Alejandro Cornejo Velázquez

#if !defined (KRATOS_FEM_DEM_ELASTIC_LAW_H_INCLUDED)
#define  KRATOS_FEM_DEM_ELASTIC_LAW_H_INCLUDED

// System includes

// External includes

// Project includes
#include "custom_constitutive/linear_elastic_3D_law.hpp"

namespace Kratos
{
/**
 * Defines a linear isotropic constitutive law in 2D (Plane Strain)
 * This material law is defined by the parameters:
 * 1) YOUNG MODULUS
 * 2) POISSON RATIO
 * As there are no further parameters the functionality is valid
 * for small and large displacements elasticity.
 */

class FemDemElasticLaw : public LinearElastic3DLaw
{
public:
    /**
     * Type Definitions
     */

    /**
     * Counted pointer of FemDemElasticLaw
     */

    KRATOS_CLASS_POINTER_DEFINITION(FemDemElasticLaw);

    /**
     * Life Cycle
     */

    /**
     * Default constructor.
     */
    FemDemElasticLaw();

    /**
     * Clone function (has to be implemented by any derived class)
     * @return a pointer to a new instance of this constitutive law
     */
    ConstitutiveLaw::Pointer Clone() const override;

    /**
     * Copy constructor.
     */
    FemDemElasticLaw (const FemDemElasticLaw& rOther);


    /**
     * Assignment operator.
     */

    //FemDemElasticLaw& operator=(const FemDemElasticLaw& rOther);


    /**
     * Destructor.
     */
    ~FemDemElasticLaw() override;


    double& CalculateValue(Parameters& rParameterValues, const Variable<double>& rThisVariable, double& rValue) override;

    double& GetValue( const Variable<double>& rThisVariable, double& rValue ) override;

private:

    ///@name Static Member Variables
    ///@{
    ///@}
    ///@name Member Variables
    ///@{


    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{
    ///@}

    ///@}
    ///@name Private  Access
    ///@{
    ///@}

    ///@}
    ///@name Serialization
    ///@{

}; // Class LinearElastic3DLaw
}  // namespace Kratos.
#endif // KRATOS_FEM_DEM_ELASTIC_LAW_H_INCLUDED  defined