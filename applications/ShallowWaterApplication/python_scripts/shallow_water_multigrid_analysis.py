from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7

# Importing Kratos
import KratosMultiphysics as Kratos
import KratosMultiphysics.ExternalSolversApplication
import KratosMultiphysics.MeshingApplication as Meshing
import KratosMultiphysics.ShallowWaterApplication as Shallow

from shallow_water_analysis import ShallowWaterAnalysis
# from multiscale_refining_process import MultiscaleRefiningProcess

class ShallowWaterMultigridAnalysis(ShallowWaterAnalysis):
    ''' Main script for shallow water simulations '''

    def __init__(self, model, project_parameters):
        super(ShallowWaterMultigridAnalysis, self).__init__(model, project_parameters)

        self.maximum_subgrids = 0
        if self.project_parameters["solver_settings"]["multigrid_settings"].Has("maximum_number_of_subscales"):
            self.maximum_subgrids = self.project_parameters["solver_settings"]["multigrid_settings"]["maximum_number_of_subscales"].GetInt()

        self.current_subscale = self._GetSolver()._GetComputingModelPart().ProcessInfo[Meshing.SUBSCALE_INDEX]
        new_parameters = self.project_parameters.Clone() # Create a copy before modifying the parameters
        self._UpdateModelPartNamesInParameters()

        if self.current_subscale < self.maximum_subgrids:
            self._ModifySubAnalysisParameters(new_parameters)
            self.sub_analysis = ShallowWaterMultigridAnalysis(model, new_parameters)

    def Initialize(self):
        super(ShallowWaterMultigridAnalysis, self).Initialize()
        if self.current_subscale < self.maximum_subgrids:
            self.sub_analysis.Initialize()

    def RunSolutionLoop(self):
        while self.time < self.end_time:
            self.time = self._GetSolver().AdvanceInTime(self.time)
            self.InitializeSolutionStep()
            self._GetSolver().Predict()
            self._GetSolver().SolveSolutionStep()
            if self.current_subscale < self.maximum_subgrids:
                self.sub_analysis.end_time = self.time
                self.sub_analysis.RunSolutionLoop()
            self.FinalizeSolutionStep()
            self.OutputSolutionStep()

    def Finalize(self):
        super(ShallowWaterMultigridAnalysis, self).Finalize()
        if self.current_subscale < self.maximum_subgrids:
            self.sub_analysis.Finalize()

    def _UpdateModelPartNamesInParameters(self):
        # Update the model part name in the processes parameters
        old_model_part_name = self.project_parameters["solver_settings"]["model_part_name"].GetString()
        new_model_part_name = self._GetSolver()._GetComputingModelPart().Name
        for name, process_list in self.project_parameters["processes"].items():
            for i in range(0,process_list.size()):
                process = process_list[i]
                if process.Has("Parameters"):
                    if process["Parameters"].Has("model_part_name"):
                        name = process["Parameters"]["model_part_name"].GetString()
                        name = name.replace(old_model_part_name, new_model_part_name)
                        process["Parameters"]["model_part_name"].SetString(name)

    def _ModifySubAnalysisParameters(self, sub_parameters):
        # Increase the current subscale index
        sub_parameters["solver_settings"]["multigrid_settings"]["current_subscale"].SetInt(self.current_subscale + 1)
        # Remove the output processes (there are only output processes on the first level)
        if sub_parameters.Has("output_processes"):
            sub_parameters.RemoveValue("output_processes")

    # def _GetOrderOfProcessesInitialization(self):
    #     return ["multigrid_process_list",
    #             "bathymetry_process_list",
    #             "initial_conditions_process_list",
    #             "boundary_conditions_process_list"]
