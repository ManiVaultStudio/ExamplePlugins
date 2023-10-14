#include "ExampleViewGLPlugin.h"
#include "ExampleGLWidget.h"

#include <graphics/Vector2f.h>

#include <DatasetsMimeData.h>

#include <QLabel>
#include <QDebug>

#include <random>
#include <numeric>

Q_PLUGIN_METADATA(IID "nl.BioVault.ExampleViewGLPlugin")

using namespace mv;

// -----------------------------------------------------------------------------
// ExampleViewGLPlugin
// -----------------------------------------------------------------------------
ExampleViewGLPlugin::ExampleViewGLPlugin(const PluginFactory* factory) :
    ViewPlugin(factory),
    _currentDataSet(),
    _currentDimensions({0, 1}),
    _dropWidget(nullptr),
    _exampleGLWidget(new ExampleGLWidget()),
    _settingsAction(this, "Settings Action")
{
    setObjectName("Example OpenGL view");

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
                    });

            }
        }
        else {
            dropRegions << new DropWidget::DropRegion(this, "Incompatible data", "This type of data is not supported", "exclamation-circle", false);
        }

        return dropRegions;
    });

    // update data when data set changed
    connect(&_currentDataSet, &Dataset<Points>::dataChanged, this, &ExampleViewGLPlugin::updatePlot);

    // update settings UI when data set changed
    connect(&_currentDataSet, &Dataset<Points>::changed, this, [this]() {
        const auto enabled = _currentDataSet.isValid();

        auto& nameString = _settingsAction.getDatasetNameAction();
        auto& xDimPicker = _settingsAction.getXDimensionPickerAction();
        auto& yDimPicker = _settingsAction.getYDimensionPickerAction();
        auto& pointSizeA = _settingsAction.getPointSizeAction();

        xDimPicker.setEnabled(enabled);
        yDimPicker.setEnabled(enabled);
        pointSizeA.setEnabled(enabled);

        if (!enabled)
            return;

        nameString.setString(_currentDataSet->getGuiName());

        xDimPicker.setPointsDataset(_currentDataSet);
        yDimPicker.setPointsDataset(_currentDataSet);

        xDimPicker.setCurrentDimensionIndex(0);

        const auto yIndex = xDimPicker.getNumberOfDimensions() >= 2 ? 1 : 0;
        yDimPicker.setCurrentDimensionIndex(yIndex);

    });

    // Create data so that we do not need to load any in this example
    createData();
}


void ExampleViewGLPlugin::init()
{
    // Create layout
    auto layout = new QVBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(_settingsAction.createWidget(&getWidget()));
    layout->addWidget(_exampleGLWidget, 100);

    // Apply the layout
    getWidget().setLayout(layout);

    // Update the data when the scatter plot widget is initialized
    connect(_exampleGLWidget, &ExampleGLWidget::initialized, this, []() { qDebug() << "ExampleGLWidget is initialized."; } );

}

void ExampleViewGLPlugin::updatePlot()
{
    if (!_currentDataSet.isValid())
    {
        qDebug() << "ExampleViewGLPlugin:: dataset is not valid - no data will be displayed";
        return;
    }

    if (_currentDataSet->getNumDimensions() < 2)
    {
        qDebug() << "ExampleViewGLPlugin:: dataset must have at least two dimensions";
        return;
    }

    // Retrieve the data that is to be shown from the core
    auto newDimX = _settingsAction.getXDimensionPickerAction().getCurrentDimensionIndex();
    auto newDimY = _settingsAction.getYDimensionPickerAction().getCurrentDimensionIndex();

    if (newDimX >= 0)
        _currentDimensions[0] = static_cast<unsigned int>(newDimX);

    if (newDimY >= 0)
        _currentDimensions[1] = static_cast<unsigned int>(newDimY);

    std::vector<mv::Vector2f> data;
    _currentDataSet->extractDataForDimensions(data, _currentDimensions[0], _currentDimensions[1]);

    // Set data in OpenGL widget
    _exampleGLWidget->setData(data, _settingsAction.getPointSizeAction().getValue(), _settingsAction.getPointOpacityAction().getValue());
}


void ExampleViewGLPlugin::loadData(const mv::Datasets& datasets)
{
    // Exit if there is nothing to load
    if (datasets.isEmpty())
        return;

    qDebug() << "ExampleViewJSPlugin::loadData: Load data set from ManiVault core";
    _dropWidget->setShowDropIndicator(false);

    // Load the first dataset, changes to _currentDataSet are connected with convertDataAndUpdateChart
    _currentDataSet = datasets.first();
    updatePlot();
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

    int numPoints = 50;
    int numDimensions = 3;
    const std::vector<QString> dimNames {"Dim 1", "Dim 2", "Dim 3"};

    qDebug() << "ExampleViewJSPlugin::createData: Create some example data. " << numPoints << " points, each with " << numDimensions << " dimensions";

    // Create random example data
    std::vector<float> exampleData;
    {
        std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(0.0, 10.0);

        for (int i = 0; i < numPoints * numDimensions; i++)
        {
            exampleData.push_back(distribution(generator));
            //qDebug() << "exampleData[" << i << "]: " << exampleData[i];
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

QIcon ExampleViewGLPluginFactory::getIcon(const QColor& color /*= Qt::black*/) const
{
    return mv::Application::getIconFont("FontAwesome").getIcon("braille", color);
}

mv::DataTypes ExampleViewGLPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;

    // This example analysis plugin is compatible with points datasets
    supportedTypes.append(PointType);

    return supportedTypes;
}

mv::gui::PluginTriggerActions ExampleViewGLPluginFactory::getPluginTriggerActions(const mv::Datasets& datasets) const
{
    PluginTriggerActions pluginTriggerActions;

    const auto getPluginInstance = [this]() -> ExampleViewGLPlugin* {
        return dynamic_cast<ExampleViewGLPlugin*>(plugins().requestViewPlugin(getKind()));
    };

    const auto numberOfDatasets = datasets.count();

    if (numberOfDatasets >= 1 && PluginFactory::areAllDatasetsOfTheSameType(datasets, PointType)) {
        auto pluginTriggerAction = new PluginTriggerAction(const_cast<ExampleViewGLPluginFactory*>(this), this, "Example GL", "OpenGL view example data", getIcon(), [this, getPluginInstance, datasets](PluginTriggerAction& pluginTriggerAction) -> void {
            for (auto& dataset : datasets)
                getPluginInstance()->loadData(Datasets({ dataset }));
        });

        pluginTriggerActions << pluginTriggerAction;
    }

    return pluginTriggerActions;
}
