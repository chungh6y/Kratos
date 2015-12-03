/*
==============================================================================
KratosDEMApplication
A library based on:
Kratos
A General Purpose Software for Multi-Physics Finite Element Analysis
Version 1.0 (Released on march 05, 2007).

Copyright 2007
Pooyan Dadvand, Riccardo Rossi, Janosch Stascheit, Felix Nagel
pooyan@cimne.upc.edu
rrossi@cimne.upc.edu
janosch.stascheit@rub.de
nagel@sd.rub.de
- CIMNE (International Center for Numerical Methods in Engineering),
Gran Capita' s/n, 08034 Barcelona, Spain
- Ruhr-University Bochum, Institute for Structural Mechanics, Germany


Permission is hereby granted, free  of charge, to any person obtaining
a  copy  of this  software  and  associated  documentation files  (the
"Software"), to  deal in  the Software without  restriction, including
without limitation  the rights to  use, copy, modify,  merge, publish,
distribute,  sublicense and/or  sell copies  of the  Software,  and to
permit persons to whom the Software  is furnished to do so, subject to
the following condition:

Distribution of this code for  any  commercial purpose  is permissible
ONLY BY DIRECT ARRANGEMENT WITH THE COPYRIGHT OWNERS.

The  above  copyright  notice  and  this permission  notice  shall  be
included in all copies or substantial portions of the Software.

THE  SOFTWARE IS  PROVIDED  "AS  IS", WITHOUT  WARRANTY  OF ANY  KIND,
EXPRESS OR  IMPLIED, INCLUDING  BUT NOT LIMITED  TO THE  WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT  SHALL THE AUTHORS OR COPYRIGHT HOLDERS  BE LIABLE FOR ANY
CLAIM, DAMAGES OR  OTHER LIABILITY, WHETHER IN AN  ACTION OF CONTRACT,
TORT  OR OTHERWISE, ARISING  FROM, OUT  OF OR  IN CONNECTION  WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

==============================================================================
*/

//
//   Project Name:        Kratos
//   Last modified by:    $Author:  $
//   Date:                $Date: $
//   Revision:            $Revision: 1.3 $
//
//

// System includes

#if defined(KRATOS_PYTHON)
// External includes
#include <boost/python.hpp>


// Project includes
#include "includes/define.h"
#include "DEM_application.h"
#include "custom_python/add_custom_strategies_to_python.h"
#include "custom_python/add_custom_utilities_to_python.h"
#include "custom_python/add_custom_constitutive_laws_to_python.h"


namespace Kratos
{

namespace Python
{

using namespace boost::python;



BOOST_PYTHON_MODULE(KratosDEMApplication)
{

    class_<KratosDEMApplication,
           KratosDEMApplication::Pointer,
           bases<KratosApplication>, boost::noncopyable >("KratosDEMApplication")
           ;

    AddCustomStrategiesToPython();
    AddCustomUtilitiesToPython();
    AddCustomConstitutiveLawsToPython();

    //registering variables in python
    
    //constitutive law
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DEM_CONTINUUM_CONSTITUTIVE_LAW_NAME )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DEM_DISCONTINUUM_CONSTITUTIVE_LAW_NAME )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DEM_DISCONTINUUM_CONSTITUTIVE_LAW_POINTER )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DEM_CONTINUUM_CONSTITUTIVE_LAW_POINTER )
            
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PROBABILITY_DISTRIBUTION )        
  
    
    // OPTIONS AND FLAGS
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(TOP)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(BOTTOM)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(FORCE_INTEGRATION_GROUP)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(BOUNDING_BOX_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(ROTATION_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CRITICAL_TIME_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(VIRTUAL_MASS_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SEARCH_CONTROL)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(LIMIT_SURFACE_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(LIMIT_CYLINDER_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CLEAN_INDENT_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(TRIHEDRON_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(ROLLING_FRICTION_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(NEIGH_INITIALIZED)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(TRIAXIAL_TEST_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(STRESS_STRAIN_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(FIX_VELOCITIES_FLAG)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DUMMY_DEBUG_DOUBLE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DUMMY_DEBUG_INT)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_ID)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONTAINS_CLUSTERS)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(RANDOM_EULER_ANGLES)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(LOCAL_RESOLUTION_METHOD)

    // *************** Continuum only BEGIN *************
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DELTA_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CASE_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SKIN_SPHERE)
    //KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEMPACK_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_COHESION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_TENSION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(COHESIVE_GROUP)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONTACT_MESH_OPTION)
    //KRATOS_REGISTER_IN_PYTHON_VARIABLE(FAILURE_CRITERION_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONCRETE_TEST_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(plot_OPTIONS)
    //KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEMPACK_LOCAL_DAMPING)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEMPACK_GLOBAL_DAMPING)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SURFACE_FRICTION_1)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SURFACE_FRICTION_2)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SURFACE_FRICTION_3)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SURFACE_FRICTION_4)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SURFACE_FRICTION_5)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_FRICTION_1)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_FRICTION_2)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_FRICTION_3)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_FRICTION_4)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_FRICTION_5)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(IF_BOUNDARY_ELEMENT)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PREDEFINED_SKIN)
    // *************** Continuum only END ***************

    // MATERIAL PARAMETERS

    KRATOS_REGISTER_IN_PYTHON_VARIABLE(NODAL_MASS_COEFF)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_MOMENT_OF_INERTIA)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(ROLLING_FRICTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(HISTORICAL_MIN_K)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_INERTIA)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_DENSITY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_FRICTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_STATIC_FRICTION_COEF)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_DYNAMIC_FRICTION_COEF)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(COEFFICIENT_OF_RESTITUTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_ROTATION_DAMP_RATIO)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SQRT_OF_MASS)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(LN_OF_RESTITUTION_COEFF)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DAMPING_GAMMA)

    // *************** Continuum only BEGIN *************
    // *************** Dempack Constitutive Law only BEGIN *************            
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SLOPE_FRACTION_N1)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SLOPE_FRACTION_N2)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SLOPE_FRACTION_N3)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SLOPE_LIMIT_COEFF_C1)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SLOPE_LIMIT_COEFF_C2)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SLOPE_LIMIT_COEFF_C3)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(YOUNG_MODULUS_PLASTIC)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PLASTIC_YIELD_STRESS)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DAMAGE_FACTOR)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SHEAR_ENERGY_COEF)
    // *************** Dempack Constitutive Law only END ***************               
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DONZE_G1)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DONZE_G2)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DONZE_G3)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DONZE_MAX_DEF)
            
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONTACT_FAILURE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONTACT_ORIENTATION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONTACT_SIGMA)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONTACT_TAU)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(FAILURE_CRITERION_STATE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(UNIDIMENSIONAL_DAMAGE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONTACT_SIGMA_MIN)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONTACT_TAU_ZERO)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONTACT_INTERNAL_FRICC)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(LOW_POISSON_FORCE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(HIGH_POISSON_FORCE)
    // *************** Continuum only END *************

    // GEOMETRIC PARAMETERS

    // *************** Continuum only BEGIN *************
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(LOCAL_CONTACT_AREA_HIGH)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(LOCAL_CONTACT_AREA_LOW)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(MEAN_CONTACT_AREA)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(REPRESENTATIVE_VOLUME)    
    // *************** Continuum only END ***************

    // INLET PARAMETERS
    
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(INLET_START_TIME)    
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(INLET_STOP_TIME)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(INLET_NUMBER_OF_PARTICLES) 
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(STANDARD_DEVIATION) 
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(MAX_RAND_DEVIATION_ANGLE)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(INLET_INITIAL_VELOCITY)
                
    // KINEMATICS

    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_ROTATION_ANGLE)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(EULER_ANGLES)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(ORIENTATION_REAL)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(ORIENTATION_IMAG)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PRINCIPAL_MOMENTS_OF_INERTIA)        
    //KRATOS_REGISTER_IN_PYTHON_VARIABLE(INITIAL_AXES_TRACKING)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_1)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_2)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_3)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_4)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_5)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_1)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_2)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_3)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_4)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_5)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_1)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_2)    
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_3)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_4)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_5)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_1)    
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_2)    
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_3)    
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_4)    
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_5)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_RADIUS_1)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_RADIUS_2)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_RADIUS_3)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_RADIUS_4)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_RADIUS_5)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_VELOCITY_1)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_VELOCITY_2)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_VELOCITY_3)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_VELOCITY_4)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_VELOCITY_5)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_ANGULAR_VELOCITY_1)  
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_ANGULAR_VELOCITY_2)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_ANGULAR_VELOCITY_3)  
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_ANGULAR_VELOCITY_4)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CYLINDER_ANGULAR_VELOCITY_5)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(DELTA_DISPLACEMENT)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(DELTA_ROTA_DISPLACEMENT)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(VELOCITY_START_TIME)    
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(VELOCITY_STOP_TIME)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(ANGULAR_VELOCITY_START_TIME)    
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(ANGULAR_VELOCITY_STOP_TIME)

    // FORCE AND MOMENTUM

    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(TOTAL_FORCES)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_1)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_2)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_3)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_4)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_5)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_1)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_2)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_3)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_4)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_5)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_MOMENT)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(MAX_ROTA_MOMENT)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(ELASTIC_FORCES)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(CONTACT_FORCES)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(RIGID_ELEMENT_FORCE)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(TANGENTIAL_ELASTIC_FORCES)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEM_PRESSURE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEM_NODAL_AREA)

            
    // ENERGY

    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PARTICLE_ELASTIC_ENERGY )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PARTICLE_KINEMATIC_ENERGY )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PARTICLE_GRAVITATIONAL_ENERGY )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PARTICLE_INELASTIC_VISCODAMPING_ENERGY )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PARTICLE_INELASTIC_FRICTIONAL_WORK )

    // *************** Continuum only BEGIN *************
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(INITIAL_ROTA_MOMENT)
    //KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_BLOCK_CONTACT_FORCE)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(EXTERNAL_APPLIED_FORCE)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(LOCAL_CONTACT_FORCE)
    //KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_CONTACT_FORCES )

    // CONCRETE TEST

    KRATOS_REGISTER_IN_PYTHON_VARIABLE(FIXED_VEL_TOP)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(FIXED_VEL_BOT)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(AREA_VERTICAL_TAPA)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(AREA_VERTICAL_CENTRE)

    // TENSION

    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEM_STRESS_XX)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEM_STRESS_XY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEM_STRESS_XZ)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEM_STRESS_YX)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEM_STRESS_YY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEM_STRESS_YZ)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEM_STRESS_ZX)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEM_STRESS_ZY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEM_STRESS_ZZ)
    // *************** Continuum only END *************

    // Possible future blocks (no FEM) interaction


    // DUMMIES INT AND DOUBLE VARIABLES

    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DUMMY_SWITCH)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DUMMY_FORCES)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DUMMY_LOCAL_AXES)

    //EXPORTS
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(EXPORT_ID)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(EXPORT_PARTICLE_FAILURE_ID)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(EXPORT_SKIN_SPHERE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PRINT_EXPORT_ID)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PRINT_SKIN_SPHERE)

    //For DEM_FEM Element
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(LOCAL_DAMP_RATIO)
    
    //SLS DEM-FEM
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(WALL_FRICTION)        
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SHEAR_STRESS)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(NON_DIMENSIONAL_VOLUME_WEAR)        
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(IMPACT_WEAR)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(SEVERITY_OF_WEAR)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(BRINELL_HARDNESS)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PRINT_WEAR)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(IMPACT_WEAR_SEVERITY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(WALL_COHESION)
    //DEM_CLUSTERS        
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CHARACTERISTIC_LENGTH)
    //BOUNDING BOX
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(BOUNDING_BOX_START_TIME)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(BOUNDING_BOX_STOP_TIME)
    
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(RIGID_FACE_ROTA_SPEED)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(RIGID_FACE_AXIAL_SPEED)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(RIGID_FACE_PROP_ID)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(RIGID_FACE_ROTA_ORIGIN_COORD)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(RIGID_FACE_ROTA_AXIAL_DIR)
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(RIGID_FACE_ROTA_GLOBAL_VELOCITY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(RIGID_FACE_BEGIN_TIME)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(RIGID_FACE_END_TIME)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(RIGID_FACE_FLAG)
                   
    //OPTIMIZATION

    //MSI DEM-FEM & OTHERS
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(CAUCHY_STRESS_VECTOR )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PK2_STRESS_VECTOR )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( ALMANSI_STRAIN_TENSOR )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( GREEN_LAGRANGE_STRAIN_VECTOR )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( ALMANSI_STRAIN_VECTOR )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( MATERIAL_STIFFNESS_MATRIX )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( GEOMETRIC_STIFFNESS_MATRIX )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( VON_MISES_STRESS )
    
    // *************** Thermal only BEGIN *************
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(HEATFLUX)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(THERMAL_CONDUCTIVITY)        
    // *************** Thermal only END ***************  
                     
    
}


}  // namespace Python.

}  // namespace Kratos.

#endif // KRATOS_PYTHON defined
