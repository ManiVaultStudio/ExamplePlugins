#include "ExampleViewJSPlugin.h"

#include "ChartWidget.h"

#include <vector>
#include <random>

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QMimeData>
#include <QDebug>

Q_PLUGIN_METADATA(IID "nl.BioVault.ExampleViewJSPlugin")

using namespace hdps;

ExampleViewJSPlugin::ExampleViewJSPlugin(const PluginFactory* factory) :
    ViewPlugin(factory),
    _chartWidget(nullptr),
    _dropWidget(nullptr),
    _currentDataSet(nullptr)
{
}

void ExampleViewJSPlugin::init()
{
    getWidget().setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    // Create layout
    auto layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    // Create chart widget and set html contents of webpage 
    _chartWidget = new ChartWidget(this);
    _chartWidget->setPage(":example_chart/radar_chart.html", "qrc:/example_chart/");

    // Add widget to layout
    layout->addWidget(_chartWidget);

    // Apply the layout
    getWidget().setLayout(layout);

    // Instantiate new drop widget: See ExampleViewPlugin for details
    _dropWidget = new DropWidget(_chartWidget);
    _dropWidget->setDropIndicatorWidget(new DropWidget::DropIndicatorWidget(&getWidget(), "No data loaded", "Drag the ExampleViewJSData in this view"));

    _dropWidget->initialize([this](const QMimeData* mimeData) -> DropWidget::DropRegions {

        // A drop widget can contain zero or more drop regions
        DropWidget::DropRegions dropRegions;

        const auto mimeText = mimeData->text();
        const auto tokens = mimeText.split("\n");

        if (tokens.count() < 2)
            return dropRegions;

        // Gather information to generate appropriate drop regions
        const auto datasetName = tokens[0];
        const auto datasetId = tokens[1];
        const auto dataType = DataType(tokens[2]);
        const auto dataTypes = DataTypes({ PointType });

        if (dataTypes.contains(dataType)) {

            if (datasetId == getCurrentDataSetGuid()) {
                dropRegions << new DropWidget::DropRegion(this, "Warning", "Data already loaded", "exclamation-circle", false);
            }
            else {
                auto candidateDataset = _core->requestDataset<Points>(datasetId);

                dropRegions << new DropWidget::DropRegion(this, "Points", QString("Visualize %1 as parallel coordinates").arg(datasetName), "map-marker-alt", true, [this, candidateDataset]() {
                    loadData({ candidateDataset });
                    });

            }
        }
        else {
            dropRegions << new DropWidget::DropRegion(this, "Incompatible data", "This type of data is not supported", "exclamation-circle", false);
        }

        return dropRegions;
        });

    // load data after drop action
    connect(this, &ExampleViewJSPlugin::dataSetChanged, this, &ExampleViewJSPlugin::convertDataAndUpdateChart);

    // update data when data set changed
    connect(&_currentDataSet, &Dataset<Points>::dataChanged, this, &ExampleViewJSPlugin::convertDataAndUpdateChart);

    //
    createData();
}

void ExampleViewJSPlugin::loadData(const hdps::Datasets& datasets)
{
    // Exit if there is nothing to load
    if (datasets.isEmpty())
        return;

    qDebug() << "ExampleViewJSPlugin: Load data set from ManiVault core";

    // Load the first dataset, changes to _currentDataSet are connected with onDataInput
    _currentDataSet = datasets.first();
    _dropWidget->setShowDropIndicator(false);

    emit dataSetChanged();
}

void ExampleViewJSPlugin::convertDataAndUpdateChart()
{
    if (!_currentDataSet.isValid())
        return;

    qDebug() << "ExampleViewJSPlugin: Prepare payload";

    // convert data
    QVariantList payload;
    QVariantMap entry;

    _currentDataSet->visitFromBeginToEnd([&entry, &payload, this](auto beginOfData, auto endOfData)
        {
            auto pointNames = _currentDataSet->getProperty("PointNames");
            auto dimNames = _currentDataSet->getDimensionNames();
            auto numDims = dimNames.size();

            for (unsigned int pointId = 0; pointId < _currentDataSet->getNumPoints(); pointId++)
            {
                entry.clear();

                entry["className"] = pointNames.isValid() ? pointNames.value<QStringList>()[pointId] : QString::number(pointId);

                QVariantList values;

                for (uint32_t dimId = 0; dimId < numDims; dimId++)
                {
                    QVariantMap axval;
                    axval["axis"] = dimNames[dimId];
                    axval["value"] = static_cast<float>(beginOfData[pointId * numDims + dimId]);
                    values.append(axval);
                }

                entry["axes"] = values;

                payload.append(entry);
            }
        });

    qDebug() << "ExampleViewJSPlugin: Send data from Qt cpp to D3 js";
    emit _chartWidget->getCommunicationObject().qt_js_setDataInJS(payload);
}

QString ExampleViewJSPlugin::getCurrentDataSetGuid() const
{
    if (_currentDataSet.isValid())
        return _currentDataSet->getGuid();
    else
        return QString{};
}

void ExampleViewJSPlugin::createData()
{
    auto points = _core->addDataset<Points>("Points", "ExampleViewJSData");
    events().notifyDatasetAdded(points);

    int numPoints = 2;
    int numDimensions = 5;

    const std::vector<QString> dimNames {"Dim 1", "Dim 2", "Dim 3", "Dim 4", "Dim 5", };
    const QVariant pointNames = QStringList{ "Data point 1", "Data point 2" };
    std::vector<float> exampleData;

    // Create random example data
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

    points->setProperty("PointNames", pointNames);

    // Notify the core system of the new data
    events().notifyDatasetChanged(points);

    qDebug() << "Example file loaded. Num data points: " << points->getNumPoints();

}


// =============================================================================
// Plugin Factory 
// =============================================================================

QIcon ExampleViewJSPluginFactory::getIcon(const QColor& color /*= Qt::black*/) const
{
    return hdps::Application::getIconFont("FontAwesome").getIcon("bullseye", color);
}

ViewPlugin* ExampleViewJSPluginFactory::produce()
{
    return new ExampleViewJSPlugin(this);
}

hdps::DataTypes ExampleViewJSPluginFactory::supportedDataTypes() const
{
    // This example analysis plugin is compatible with points datasets
    DataTypes supportedTypes;
    supportedTypes.append(PointType);
    return supportedTypes;
}

hdps::gui::PluginTriggerActions ExampleViewJSPluginFactory::getPluginTriggerActions(const hdps::Datasets& datasets) const
{
    PluginTriggerActions pluginTriggerActions;

    const auto getPluginInstance = [this]() -> ExampleViewJSPlugin* {
        return dynamic_cast<ExampleViewJSPlugin*>(plugins().requestViewPlugin(getKind()));
    };

    const auto numberOfDatasets = datasets.count();

    if (numberOfDatasets >= 1 && PluginFactory::areAllDatasetsOfTheSameType(datasets, PointType)) {
        auto pluginTriggerAction = new PluginTriggerAction(const_cast<ExampleViewJSPluginFactory*>(this), this, "Example JS", "View JavaScript visualization", getIcon(), [this, getPluginInstance, datasets](PluginTriggerAction& pluginTriggerAction) -> void {
            for (auto dataset : datasets)
                getPluginInstance()->loadData(Datasets({ dataset }));

        });

        pluginTriggerActions << pluginTriggerAction;
    }

    return pluginTriggerActions;
}
