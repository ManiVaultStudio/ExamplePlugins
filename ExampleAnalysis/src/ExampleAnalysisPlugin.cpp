#include "ExampleAnalysisPlugin.h"

#include "PointData.h"

#include <QtCore>
#include <QDebug>

Q_PLUGIN_METADATA(IID "nl.BioVault.ExampleAnalysisPlugin")

using namespace hdps;
using namespace hdps::plugin;

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
    // Create example output dataset (a points dataset which is derived from the input points dataset)
    const auto outputDatasetName = _core->createDerivedData("Output Data", getInputDatasetName());

    // Set the output dataset name (this will internally update the _output dataset ref)
    setOutputDatasetName(outputDatasetName);

    // Retrieve the input dataset for our specific data type (in our case points)
    // The HDPS core sets the input dataset reference when the plugin is created
    const auto& inputPoints  = getInputDataset<Points>();

    // Retrieve the output dataset for our specific data type (in our case points)
    auto& outputPoints = getOutputDataset<Points>();

    // Set the dimension names as visible in the GUI
    outputPoints.setDimensionNames(Point::dimensionNames);

    // Assign the maximum velocity from the GUI
    Point::maximumVelocity = _settingsAction.getMaxVelocityAction().getValue();

    // Performs points update
    const auto updatePoints = [this, &outputPoints]() {

        // Assign the output points to the output dataset
        outputPoints.setData(reinterpret_cast<float*>(_points.data()), _points.count(), Point::numberOfDimensions);

        // Inform the core (and thus others) that the data changed
        _output.notifyDataChanged();
    };

    // Initializes the points
    const auto initializePoints = [this, &inputPoints, updatePoints]() {

        if (_points.count() != inputPoints.getNumPoints()) {

            // Resize the point positions and headings
            _points.resize(inputPoints.getNumPoints());
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
    outputPoints.addAction(_settingsAction);
    
    // Update current iteration action
    const auto updateCurrentIterationAction = [this](const std::int32_t& currentIteration = 0) {
        _settingsAction.getCurrentIterationAction().setString(QString::number(currentIteration));
    };

    // Start the analysis when the user clicks the start analysis push button
    connect(&_settingsAction.getStartAnalysisAction(), &hdps::gui::TriggerAction::triggered, this, [this, initializePoints, updatePoints, updateCurrentIterationAction]() {

        // Initialize our points
        initializePoints();

        // Disable actions during analysis
        _settingsAction.getMaxVelocityAction().setEnabled(false);
        _settingsAction.getNumberOfIterationsAction().setEnabled(false);

        // Set the task name as it will appear in the data hierarchy viewer
        setTaskName("Example analysis");

        // In order to report progress the task status has to be set to running
        setTaskRunning();

        // Zero progress at the start
        setTaskProgress(0.0f);

        // Set task description as it will appear in the data hierarchy viewer
        setTaskDescription("Initializing");

        // Get the number of iterations from the settings
        const auto numberOfIterations = _settingsAction.getNumberOfIterationsAction().getValue();

        // Do computation
        setTaskDescription("Computing...");

        // Perform the loop
        for (int i = 0; i < numberOfIterations; i++)
        {
            QCoreApplication::processEvents();
            
            // Update task progress
            setTaskProgress(i / static_cast<float>(numberOfIterations));

            // Update task progress
            setTaskDescription(QString("Computing iteration %1/%2").arg(QString::number(i), QString::number(numberOfIterations)));

            // Advance the points
            for (auto& point : _points)
                point.advancePoint();

            // Update the points
            updatePoints();

            // Update current iteration action
            updateCurrentIterationAction(i + 1);
        }

        // Flag the analysis task as finished
        setTaskFinished();

        // Enabled actions again
        _settingsAction.getMaxVelocityAction().setEnabled(true);
        _settingsAction.getNumberOfIterationsAction().setEnabled(true);
    });

    // Initialize current iteration action
    updateCurrentIterationAction();

    // Initialize our points
    initializePoints();

    // Register for points datasets events using a custom callback function
    registerDataEventByType(PointType, std::bind(&ExampleAnalysisPlugin::onDataEvent, this, std::placeholders::_1));
}

void ExampleAnalysisPlugin::onDataEvent(hdps::DataEvent* dataEvent)
{
    // The data event has a type so that we know what type of data event occurred (e.g. data added, changed, removed, renamed, selection changes)
    switch (dataEvent->getType()) {

        // A points dataset was added
        case EventType::DataAdded:
        {
            // Cast the data event to a data added event
            const auto dataAddedEvent = static_cast<DataAddedEvent*>(dataEvent);

            // Get the name of the added points dataset
            const auto pointsDatasetName = dataAddedEvent->dataSetName;

            // Print to the console
            qDebug() << pointsDatasetName << "was added";

            break;
        }

        // Points dataset data has changed
        case EventType::DataChanged:
        {
            // Cast the data event to a data changed event
            const auto dataChangedEvent = static_cast<DataChangedEvent*>(dataEvent);

            // Get the name of the points dataset of which the data changed
            const auto pointsDatasetName = dataChangedEvent->dataSetName;

            // Print to the console
            qDebug() << pointsDatasetName << "data changed";

            break;
        }

        // Points dataset data was removed
        case EventType::DataRemoved:
        {
            // Cast the data event to a data removed event
            const auto dataRemovedEvent = static_cast<DataRemovedEvent*>(dataEvent);

            // Get the name of the removed points dataset
            const auto pointsDatasetName = dataRemovedEvent->dataSetName;

            // Print to the console
            qDebug() << pointsDatasetName << "was removed";

            break;
        }

        // Points dataset selection has changed
        case EventType::SelectionChanged:
        {
            // Cast the data event to a selection changed event
            const auto selectionChangedEvent = static_cast<SelectionChangedEvent*>(dataEvent);

            // Get the name of the points dataset of which the selection changed
            const auto pointsDatasetName = selectionChangedEvent->dataSetName;

            // Get points dataset
            const auto& changedDataSet = _core->requestData<Points>(dataEvent->dataSetName);

            // Get the selection set that changed
            const auto& selectionSet = changedDataSet.getSelection();

            // Print to the console
            qDebug() << pointsDatasetName << "selection has changed";

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

hdps::DataTypes ExampleAnalysisPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;

    // This example analysis plugin is compatible with points datasets
    supportedTypes.append(PointType);

    return supportedTypes;
}
