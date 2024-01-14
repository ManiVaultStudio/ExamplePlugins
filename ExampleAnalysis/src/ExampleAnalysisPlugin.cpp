#include "ExampleAnalysisPlugin.h"

#include "PointData/PointData.h"

#include <event/Event.h>

#include <QtCore>
#include <QDebug>

Q_PLUGIN_METADATA(IID "nl.BioVault.ExampleAnalysisPlugin")

using namespace mv;
using namespace mv::plugin;

// Initialize the random number generator
QRandomGenerator ExampleAnalysisPlugin::Point::rng;

// Initialize the dimension names
std::vector<QString> ExampleAnalysisPlugin::Point::dimensionNames = std::vector<QString>({"x_pos", "y_pos" , "heading_rad", "x_heading", "y_heading", "vel_position", "vel_heading" });

// Initialize the number of dimensions in the point
std::uint32_t ExampleAnalysisPlugin::Point::numberOfDimensions = 7;

// Initialize the maximum velocity
float ExampleAnalysisPlugin::Point::maximumVelocity = 1.0f;

ExampleAnalysisPlugin::ExampleAnalysisPlugin(const PluginFactory* factory) :
    AnalysisPlugin(factory),
    _settingsAction(),
    _points(),
    _pointHeadings()
{
}

void ExampleAnalysisPlugin::init()
{
    // Create example output dataset (a points dataset which is derived from the input points dataset) and set the output dataset
    setOutputDataset(mv::data().createDerivedDataset("Output Data", getInputDataset(), getInputDataset()));

    // Retrieve the input dataset for our specific data type (in our case points)
    // The HDPS core sets the input dataset reference when the plugin is created
    const auto inputPoints  = getInputDataset<Points>();

    // Retrieve the output dataset for our specific data type (in our case points)
    auto outputPoints = getOutputDataset<Points>();

    // Set the dimension names as visible in the GUI
    outputPoints->setDimensionNames(Point::dimensionNames);

    // Assign the maximum velocity from the GUI
    Point::maximumVelocity = _settingsAction.getMaxVelocityAction().getValue();

    // Performs points update
    const auto updatePoints = [this, &outputPoints]() {

        // Assign the output points to the output dataset
        outputPoints->setData(reinterpret_cast<float*>(_points.data()), _points.count(), Point::numberOfDimensions);

        // Inform the core (and thus others) that the data changed
        events().notifyDatasetDataChanged(outputPoints);
    };

    // Initializes the points
    const auto initializePoints = [this, inputPoints, updatePoints]() {

        if (_points.count() != inputPoints->getNumPoints()) {

            // Resize the point positions and headings
            _points.resize(inputPoints->getNumPoints());
        }
        else {

            // Initialize the points
            for (auto& point : _points)
                point.initialize();
        }

        // Update the points
        updatePoints();
    };

    // Inject the settings action in the output points dataset 
    // By doing so, the settings user interface will be accessible though the data properties widget
    outputPoints->addAction(_settingsAction);
    
    // Update current iteration action
    const auto updateCurrentIterationAction = [this](const std::int32_t& currentIteration = 0) {
        _settingsAction.getCurrentIterationAction().setString(QString::number(currentIteration));
    };

    // Start the analysis when the user clicks the start analysis push button
    connect(&_settingsAction.getStartAnalysisAction(), &mv::gui::TriggerAction::triggered, this, [this, initializePoints, updatePoints, updateCurrentIterationAction]() {

        // Initialize our points
        initializePoints();

        // Disable actions during analysis
        _settingsAction.getMaxVelocityAction().setEnabled(false);
        _settingsAction.getNumberOfIterationsAction().setEnabled(false);

        // Get reference to dataset task for reporting progress
        auto& datasetTask = getOutputDataset()->getTask();

        // Set the task name as it will appear in the data hierarchy viewer
        datasetTask.setName("Example analysis");

        // In order to report progress the task status has to be set to running
        datasetTask.setRunning();

        // Zero progress at the start
        datasetTask.setProgress(0.0f);

        // Set task description as it will appear in the data hierarchy viewer
        datasetTask.setProgressDescription("Initializing");

        // Get the number of iterations from the settings
        const auto numberOfIterations = _settingsAction.getNumberOfIterationsAction().getValue();

        // Do computation
        datasetTask.setProgressDescription("Computing...");

        // Perform the loop
        for (int i = 0; i < numberOfIterations; i++)
        {
            QCoreApplication::processEvents();
            
            // Update task progress
            datasetTask.setProgress(i / static_cast<float>(numberOfIterations));

            // Update task progress
            datasetTask.setProgressDescription(QString("Computing iteration %1/%2").arg(QString::number(i), QString::number(numberOfIterations)));

            // Advance the points
            for (auto& point : _points)
                point.advancePoint();

            // Update the points
            updatePoints();

            // Update current iteration action
            updateCurrentIterationAction(i + 1);
        }

        // Flag the analysis task as finished
        datasetTask.setFinished();

        // Enabled actions again
        _settingsAction.getMaxVelocityAction().setEnabled(true);
        _settingsAction.getNumberOfIterationsAction().setEnabled(true);
    });

    // Initialize current iteration action
    updateCurrentIterationAction();

    // Initialize our points
    initializePoints();

    // Register for points datasets events using a custom callback function
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetAdded));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetDataChanged));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetRemoved));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetDataSelectionChanged));

    _eventListener.registerDataEventByType(PointType, std::bind(&ExampleAnalysisPlugin::onDataEvent, this, std::placeholders::_1));
}

void ExampleAnalysisPlugin::onDataEvent(mv::DatasetEvent* dataEvent)
{
    // The data event has a type so that we know what type of data event occurred (e.g. data added, changed, removed, renamed, selection changes)
    switch (dataEvent->getType()) {

        // A points dataset was added
        case EventType::DatasetAdded:
        {
            // Cast the data event to a data added event
            const auto dataAddedEvent = static_cast<DatasetAddedEvent*>(dataEvent);

            // Get the GUI name of the added points dataset and print to the console
            qDebug() << dataAddedEvent->getDataset()->getGuiName() << "was added";

            break;
        }

        // Points dataset data has changed
        case EventType::DatasetDataChanged:
        {
            // Cast the data event to a data changed event
            const auto dataChangedEvent = static_cast<DatasetDataChangedEvent*>(dataEvent);

            // Get the GUI name of the points dataset of which the data changed and print to the console
            qDebug() << dataChangedEvent->getDataset()->getGuiName() << "data changed";

            break;
        }

        // Points dataset data was removed
        case EventType::DatasetRemoved:
        {
            // Cast the data event to a data removed event
            const auto dataRemovedEvent = static_cast<DatasetRemovedEvent*>(dataEvent);

            // Get the GUI name of the removed points dataset and print to the console
            qDebug() << dataRemovedEvent->getDataset()->getGuiName() << "was removed";

            break;
        }

        // Points dataset selection has changed
        case EventType::DatasetDataSelectionChanged:
        {
            // Cast the data event to a data selection changed event
            const auto dataSelectionChangedEvent = static_cast<DatasetDataSelectionChangedEvent*>(dataEvent);

            // Get points dataset
            const auto& changedDataSet = dataSelectionChangedEvent->getDataset();

            // Get the selection set that changed
            const auto selectionSet = changedDataSet->getSelection<Points>();

            // Print to the console
            qDebug() << changedDataSet->getGuiName() << "selection has changed";

            break;
        }

        default:
            break;
    }
}

AnalysisPlugin* ExampleAnalysisPluginFactory::produce()
{
    // Return a new instance of the example analysis plugin
    return new ExampleAnalysisPlugin(this);
}

mv::DataTypes ExampleAnalysisPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;

    // This example analysis plugin is compatible with points datasets
    supportedTypes.append(PointType);

    return supportedTypes;
}

mv::gui::PluginTriggerActions ExampleAnalysisPluginFactory::getPluginTriggerActions(const mv::Datasets& datasets) const
{
    PluginTriggerActions pluginTriggerActions;

    const auto getPluginInstance = [this](const Dataset<Points>& dataset) -> ExampleAnalysisPlugin* {
        return dynamic_cast<ExampleAnalysisPlugin*>(plugins().requestPlugin(getKind(), { dataset }));
    };

    const auto numberOfDatasets = datasets.count();

    if (numberOfDatasets >= 1 && PluginFactory::areAllDatasetsOfTheSameType(datasets, PointType)) {
        auto pluginTriggerAction = new PluginTriggerAction(const_cast<ExampleAnalysisPluginFactory*>(this), this, "Example Analysis", "Perform an Example Analysis", getIcon(), [this, getPluginInstance, datasets](PluginTriggerAction& pluginTriggerAction) -> void {
            for (auto dataset : datasets)
                getPluginInstance(dataset);
            });

        pluginTriggerActions << pluginTriggerAction;
    }

    return pluginTriggerActions;
}
