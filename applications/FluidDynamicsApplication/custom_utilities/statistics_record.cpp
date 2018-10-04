#include "includes/define.h"
#include "includes/element.h"
#include "containers/variable.h"

#include "statistics_record.h"
#include "statistics_data.h"

#include "fluid_dynamics_application_variables.h"

namespace Kratos {

void StatisticsRecord::AddResult(StatisticsSampler::Pointer pResult)
{
    KRATOS_TRY

    KRATOS_ERROR_IF(mInitialized) << "Trying to add statistical data after Initialization of the internal storage." << std::endl;

    std::size_t result_size = pResult->GetSize();
    pResult->SetOffset(mDataBufferSize);

    mDataBufferSize += result_size;
    mAverageData.push_back(pResult);

    KRATOS_CATCH("")
}

void StatisticsRecord::InitializeStorage()
{
    mUpdateBuffer.resize(mDataBufferSize);
    mMeasurementBuffer.resize(mDataBufferSize);
    mInitialized = true;
}

void StatisticsRecord::UpdateStatistics(Element* pElement)
{
    KRATOS_DEBUG_ERROR_IF(pElement->Has(TURBULENCE_STATISTICS_DATA)) << "Trying to compute turbulent statistics, but " << pElement->Info() << " does not have TURBULENCE_STATISTICS_DATA defined." << std::endl;
    auto &r_elemental_statistics = pElement->GetValue(TURBULENCE_STATISTICS_DATA);
    r_elemental_statistics.UpdateMeasurement(pElement, mAverageData, mMeasurementBuffer, mUpdateBuffer, mRecordedSteps);
    //r_elemental_statistics.CalculateUpdateDelta(pElement, mAverageData, mMeasurementBuffer, mUpdateBuffer, mRecordedSteps);
    //r_elemental_statistics.UpdateMeasurement(mMeasurementBuffer);
}

KRATOS_CREATE_VARIABLE( StatisticsRecord::Pointer, STATISTICS_CONTAINER)

//TODO move somewhere else
KRATOS_CREATE_VARIABLE( StatisticsData, TURBULENCE_STATISTICS_DATA)

std::vector<double> StatisticsRecord::mUpdateBuffer = std::vector<double>();
std::vector<double> StatisticsRecord::mMeasurementBuffer = std::vector<double>();

}