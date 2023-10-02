#include "ExampleViewGLPlugin.h"
#include "ExampleGLWidget.h"
#include "SettingsAction.h"

#include <DatasetsMimeData.h>

#include <QLabel>
#include <QDebug>

#include <random>
#include <numeric>

Q_PLUGIN_METADATA(IID "nl.BioVault.ExampleViewGLPlugin")

using namespace hdps;

// -----------------------------------------------------------------------------
// ExampleViewGLPlugin
// -----------------------------------------------------------------------------
ExampleViewGLPlugin::ExampleViewGLPlugin(const PluginFactory* factory) :
    ViewPlugin(factory),
    _dropWidget(nullptr),
    _exampleGLWidget(new ExampleGLWidget()),
    _currentDataSet(),
    _currentDatasetName(),
    _primaryToolbarAction(this, "Primary Toolbar"),
    _settingsAction(this, "Settings Action")
{
    setObjectName("Example OpenGL view");

    _primaryToolbarAction.addAction(&_settingsAction->getDatasetNameAction());
    _primaryToolbarAction.addAction(&_settingsAction->getXDimensionPickerAction());
    _primaryToolbarAction.addAction(&_settingsAction->getYDimensionPickerAction());

    // Instantiate new drop widget, setting the example Widget as its parent
    // the parent widget hat to setAcceptDrops(true) for the drop widget to work
    _dropWidget = new DropWidget(_exampleGLWidget);

    // Set the drop indicator widget (the widget that indicates that the view is eligible for data dropping)
    _dropWidget->setDropIndicatorWidget(new DropWidget::DropIndicatorWidget(&getWidget(), "No data loaded", "Drag the ExampleViewGLData from the data hierarchy here"));

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

        if (dataTypes.contains(dataType)) {

            if (datasetId == getCurrentDataSetID()) {
                dropRegions << new DropWidget::DropRegion(this, "Warning", "Data already loaded", "exclamation-circle", false);
            }
            else {
                auto candidateDataset = _core->requestDataset<Points>(datasetId);

                dropRegions << new DropWidget::DropRegion(this, "Points", QString("Visualize %1 as parallel coordinates").arg(datasetGuiName), "map-marker-alt", true, [this, candidateDataset]() {
                    loadData({ candidateDataset });
                    _dropWidget->setShowDropIndicator(false);
                    });

            }
        }
        else {
            dropRegions << new DropWidget::DropRegion(this, "Incompatible data", "This type of data is not supported", "exclamation-circle", false);
        }

        return dropRegions;
    });

    // update data when data set changed
    connect(&_currentDataSet, &Dataset<Points>::dataChanged, this, &ExampleViewGLPlugin::updateData);

    // Create data so that we do not need to load any in this example
    createData();
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

void ExampleViewGLPlugin::updateData()
{
    if (!_currentDataSet.isValid())
    {
        qDebug() << "ExampleViewGLPlugin:: dataset is not valid - no data will be displayed";
        return;
    }
    
    std::vector<float> data;
    std::vector<unsigned int> dimensionIndices = {0, 1}; 

    // TODO: let user select dimensions
    //std::vector<bool> enabledDimensions = _dimensionSelectionAction.getPickerAction().getEnabledDimensions();
    //const auto numEnabledDimensions = count_if(enabledDimensions.begin(), enabledDimensions.end(), [](bool b) { return b; });
    //for (uint32_t i = 0; i < coreDataset->getNumDimensions(); i++)
    //    if (enabledDimensions[i])
    //        dimensionIndices.push_back(i);

    // points.extractDataForDimensions(_positions, _settingsAction.getPositionAction().getDimensionX(), _settingsAction.getPositionAction().getDimensionY());

    data.resize(2ll * _currentDataSet->getNumPoints());

    _currentDataSet->populateDataForDimensions(data, dimensionIndices);

    _exampleGLWidget->setData(data);
}


void ExampleViewGLPlugin::loadData(const hdps::Datasets& datasets)
{
    // Exit if there is nothing to load
    if (datasets.isEmpty())
        return;

    qDebug() << "ExampleViewJSPlugin::loadData: Load data set from ManiVault core";

    // Load the first dataset, changes to _currentDataSet are connected with convertDataAndUpdateChart
    _currentDataSet = datasets.first();
    updateData();
}

QString ExampleViewGLPlugin::getCurrentDataSetID() const
{
    if (_currentDataSet.isValid())
        return _currentDataSet->getId();
    else
        return QString{};
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

    qDebug() << "ExampleViewJSPlugin::createData: Create some example data. 5 points, each with 3 dimensions";

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