#include "ExampleViewGLPlugin.h"
#include "ExampleGLWidget.h"

#include <DatasetsMimeData.h>

#include <QLabel>
#include <QDebug>

#include <random>

Q_PLUGIN_METADATA(IID "nl.BioVault.ExampleViewGLPlugin")

using namespace hdps;

// -----------------------------------------------------------------------------
// ExampleViewGLPlugin
// -----------------------------------------------------------------------------
ExampleViewGLPlugin::ExampleViewGLPlugin(const PluginFactory* factory) :
    ViewPlugin(factory),
    _dropWidget(nullptr),
    _exampleGLWidget(new ExampleGLWidget()),
    _points(),
    _currentDatasetName()
{
    setObjectName("Example OpenGL view");

    // Instantiate new drop widget, setting the example Widget as its parent
    _dropWidget = new DropWidget(_exampleGLWidget);

    // Set the drop indicator widget (the widget that indicates that the view is eligible for data dropping)
    _dropWidget->setDropIndicatorWidget(new DropWidget::DropIndicatorWidget(&getWidget(), "No data loaded", "Drag an item from the data hierarchy and drop it here to visualize data..."));

    // Initialize the drop regions
    _dropWidget->initialize([this](const QMimeData* mimeData) -> DropWidget::DropRegions {
        // A drop widget can contain zero or more drop regions
        DropWidget::DropRegions dropRegions;

        const auto datasetsMimeData = dynamic_cast<const DatasetsMimeData*>(mimeData);

        if (datasetsMimeData == nullptr)
            return dropRegions;

        if (datasetsMimeData->getDatasets().count() > 1)
            return dropRegions;

        // Gather information to generate appropriate drop regions
        const auto dataset = datasetsMimeData->getDatasets().first();
        const auto datasetGuiName = dataset->getGuiName();
        const auto datasetId = dataset->getId();
        const auto dataType = dataset->getDataType();
        const auto dataTypes = DataTypes({ PointType });

        // Visually indicate if the dataset is of the wrong data type and thus cannot be dropped
        if (!dataTypes.contains(dataType)) {
            dropRegions << new DropWidget::DropRegion(this, "Incompatible data", "This type of data is not supported", "exclamation-circle", false);
        }
        else {

            // Get points dataset from the core
            auto candidateDataset = _core->requestDataset<Points>(datasetId);

            // Accept points datasets drag and drop
            if (dataType == PointType) {
                const auto description = QString("Load %1 into example view").arg(datasetGuiName);

                if (_points == candidateDataset) {

                    // Dataset cannot be dropped because it is already loaded
                    dropRegions << new DropWidget::DropRegion(this, "Warning", "Data already loaded", "exclamation-circle", false);
                }
                else {

                    // Dataset can be dropped
                    dropRegions << new DropWidget::DropRegion(this, "Points", description, "map-marker-alt", true, [this, candidateDataset]() {
                        _points = candidateDataset;
                    });
                }
            }
        }

        return dropRegions;
    });

    // Respond when the name of the dataset in the dataset reference changes
    connect(&_points, &Dataset<Points>::guiNameChanged, this, [this]() {

        auto newDatasetName = _points->getGuiName();

        // Only show the drop indicator when nothing is loaded in the dataset reference
        _dropWidget->setShowDropIndicator(newDatasetName.isEmpty());
    });

    // Create data so that we do not need to load any in this example
    createData();
}

void ExampleViewGLPlugin::createData()
{
    // Here, we create a random data set, so that we do not need 
    // to use other plugins for loading when trying out this example
    auto points = _core->addDataset<Points>("Points", "ExampleViewGLData");
    events().notifyDatasetAdded(points);

    int numPoints = 5;
    int numDimensions = 3;
    const std::vector<QString> dimNames {"Dim 1", "Dim 2", "Dim 3"};

    qDebug() << "ExampleViewJSPlugin::createData: Create some example data. 2 points, each with 5 dimensions";

    // Create random example data
    std::vector<float> exampleData;
    {
        std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(0.0, 10.0);

        for (int i = 0; i < numPoints * numDimensions; i++)
        {
            exampleData.push_back(distribution(generator));
            qDebug() << "exampleData[" << i << "]: " << exampleData[i];
        }
    }

    // Passing example data with 1000 points and 2 dimensions
    points->setData(exampleData.data(), numPoints, numDimensions);
    points->setDimensionNames(dimNames);

    // Notify the core system of the new data
    events().notifyDatasetDataChanged(points);
    events().notifyDatasetDataDimensionsChanged(points);
}


void ExampleViewGLPlugin::init()
{
    // Create layout
    auto layout = new QVBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(_exampleGLWidget, 100);

    // Apply the layout
    getWidget().setLayout(layout);

    // Update the data when the scatter plot widget is initialized
    connect(_exampleGLWidget, &ExampleGLWidget::initialized, this, []() { qDebug() << "ExampleGLWidget is initialized."; } );

}

// -----------------------------------------------------------------------------
// ExampleViewGLPluginFactory
// -----------------------------------------------------------------------------

ViewPlugin* ExampleViewGLPluginFactory::produce()
{
    return new ExampleViewGLPlugin(this);
}

hdps::DataTypes ExampleViewGLPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;

    // This example analysis plugin is compatible with points datasets
    supportedTypes.append(PointType);

    return supportedTypes;
}

hdps::gui::PluginTriggerActions ExampleViewGLPluginFactory::getPluginTriggerActions(const hdps::Datasets& datasets) const
{
    PluginTriggerActions pluginTriggerActions;

    const auto getPluginInstance = [this]() -> ExampleViewGLPlugin* {
        return dynamic_cast<ExampleViewGLPlugin*>(plugins().requestViewPlugin(getKind()));
    };

    const auto numberOfDatasets = datasets.count();

    if (numberOfDatasets >= 1 && PluginFactory::areAllDatasetsOfTheSameType(datasets, PointType)) {
        auto pluginTriggerAction = new PluginTriggerAction(const_cast<ExampleViewGLPluginFactory*>(this), this, "Example", "OpenGL view example data", getIcon(), [this, getPluginInstance, datasets](PluginTriggerAction& pluginTriggerAction) -> void {
            for (auto dataset : datasets)
                getPluginInstance();
        });

        pluginTriggerActions << pluginTriggerAction;
    }

    return pluginTriggerActions;
}
