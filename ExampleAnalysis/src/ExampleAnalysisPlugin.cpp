#include "ExampleAnalysisPlugin.h"

#include "PointData.h"

#include <QtCore>
#include <QtDebug>

Q_PLUGIN_METADATA(IID "nl.tudelft.ExampleAnalysisPlugin")

using namespace hdps;

// =============================================================================
// Analysis
// =============================================================================

ExampleAnalysisPlugin::~ExampleAnalysisPlugin(void)
{
    
}

void ExampleAnalysisPlugin::init()
{
    _settings = std::make_unique<ExampleSettingsWidget>();

    // Connect the dataSetPicked signal from the settings widget to a function which handles it in the plugin
    connect(_settings.get(), &ExampleSettingsWidget::dataSetPicked, this, &ExampleAnalysisPlugin::dataSetPicked);

    // Register for data events (data added, changed, removed, renamed, selection changes) using custom callback function
    registerDataEventByType(PointType, std::bind(&ExampleAnalysisPlugin::onDataEvent, this, std::placeholders::_1));
}

void ExampleAnalysisPlugin::onDataEvent(hdps::DataEvent* dataEvent)
{
    // Event which gets triggered when a dataset is added to the system.
    if (dataEvent->getType() == EventType::DataAdded)
    {
        // Add the data set name to our data options in the settings
        _settings->dataOptions.addItem(dataEvent->dataSetName);
    }
    // Event which gets triggered when the data contained in a dataset changes.
    if (dataEvent->getType() == EventType::DataChanged)
    {
        // Request the point data that has changed for further processing
        // Datasets can be retrieved through _core->requestData<Type>(name);
        const Points& changedSet = _core->requestData<Points>(dataEvent->dataSetName);

        // ...
    }
    // Event which gets triggered when a dataset is removed from the system.
    if (dataEvent->getType() == EventType::DataRemoved)
    {
        // Request the point data that has been removed for further processing
        const Points& removedSet = _core->requestData<Points>(dataEvent->dataSetName);

        // ...
    }
    // Event which gets triggered when the selection associated with a dataset changes.
    if (dataEvent->getType() == EventType::SelectionChanged)
    {
        // Request the point data associated with the changed selection, and retrieve the selection from it
        const Points& changedDataSet = _core->requestData<Points>(dataEvent->dataSetName);
        const hdps::DataSet& selectionSet = changedDataSet.getSelection();

        // ...
    }
}

/**
 * Gets triggered when you pick a dataset in the settings widget
 */
void ExampleAnalysisPlugin::dataSetPicked(const QString& name)
{
    // Request the dataset from the core
    Points& set = _core->requestData<Points>(name);

}

// =============================================================================
// Factory DOES NOT NEED TO BE ALTERED
// Merely responsible for generating new plugins when requested
// =============================================================================

AnalysisPlugin* ExampleAnalysisPluginFactory::produce()
{
    return new ExampleAnalysisPlugin(this);
}

hdps::DataTypes ExampleAnalysisPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;
    supportedTypes.append(PointType);
    return supportedTypes;
}
