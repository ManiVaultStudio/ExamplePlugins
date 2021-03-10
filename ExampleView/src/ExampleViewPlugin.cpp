#include "ExampleViewPlugin.h"

#include "PointData.h"

#include <QtCore>
#include <QtDebug>

Q_PLUGIN_METADATA(IID "nl.tudelft.ExampleViewPlugin")

using namespace hdps;

// =============================================================================
// View
// =============================================================================

ExampleViewPlugin::~ExampleViewPlugin(void)
{
    
}

void ExampleViewPlugin::init()
{
    // Widgets can be freely added to the view plugin via the addWidget() function.
    ExampleWidget* exampleWidget = new ExampleWidget();

    auto layout = new QVBoxLayout();

    layout->setMargin(0);
    layout->addWidget(exampleWidget, 0, Qt::AlignCenter);

    setLayout(layout);
}

void ExampleViewPlugin::onDataEvent(hdps::DataEvent* dataEvent)
{
    // Event which gets triggered when a dataset is added to the system.
    if (dataEvent->getType() == EventType::DataAdded)
    {
        // Request the point data that has been added for further processing
        const Points& addedSet = _core->requestData<Points>(dataEvent->dataSetName);

        // ...
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

// =============================================================================
// Factory DOES NOT NEED TO BE ALTERED
// Merely responsible for generating new plugins when requested
// =============================================================================

ViewPlugin* ExampleViewPluginFactory::produce()
{
    return new ExampleViewPlugin();
}
