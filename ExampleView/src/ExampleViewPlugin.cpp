#include "ExampleViewPlugin.h"

#include <QDebug>
#include <QMimeData>

Q_PLUGIN_METADATA(IID "nl.BioVault.ExampleViewPlugin")

using namespace hdps;

ExampleViewPlugin::ExampleViewPlugin(const PluginFactory* factory) :
    ViewPlugin(factory),
    _dropWidget(nullptr),
    _points(),
    _currentDatasetName(),
    _currentDatasetNameLabel(new QLabel())
{
    // This line is mandatory if drag and drop behavior is required
    _currentDatasetNameLabel->setAcceptDrops(true);

    // Align text in the center
    _currentDatasetNameLabel->setAlignment(Qt::AlignCenter);
}

void ExampleViewPlugin::init()
{
    // Create layout
    auto layout = new QVBoxLayout();

    layout->setMargin(0);
    layout->addWidget(_currentDatasetNameLabel);

    // Apply the layout
    setLayout(layout);
    
    // Instantiate new drop widget
    _dropWidget = new DropWidget(_currentDatasetNameLabel);

    // Set the drop indicator widget (the widget that indicates that the view is eligible for data dropping)
    _dropWidget->setDropIndicatorWidget(new DropWidget::DropIndicatorWidget(this, "No data loaded", "Drag an item from the data hierarchy and drop it here to visualize data..."));

    // Initialize the drop regions
    _dropWidget->initialize([this](const QMimeData* mimeData) -> DropWidget::DropRegions {

        // A drop widget can contain zero or more drop regions
        DropWidget::DropRegions dropRegions;

        const auto mimeText = mimeData->text();
        const auto tokens   = mimeText.split("\n");

        if (tokens.count() != 2)
            return dropRegions;

        // Gather information to generate appropriate drop regions
        const auto datasetName          = tokens[0];
        const auto dataType             = DataType(tokens[1]);
        const auto dataTypes            = DataTypes({ PointType });
        const auto currentDatasetName   = _points.getDatasetName();

        // Visually indicate if the dataset is of the wrong data type and thus cannot be dropped
        if (!dataTypes.contains(dataType)) {
            dropRegions << new DropWidget::DropRegion(this, "Incompatible data", "This type of data is not supported", false);
        }
        else {

            // Accept points datasets drag and drop
            if (dataType == PointType) {
                const auto description = QString("Load %1 into example view").arg(datasetName);

                if (!_points.isValid()) {
                    return dropRegions;
                }
                else {
                    if (datasetName == currentDatasetName) {

                        // Dataset cannot be dropped because it is already loaded
                        dropRegions << new DropWidget::DropRegion(this, "Warning", "Data already loaded", false);
                    }
                    else {

                        // Dataset can be dropped
                        dropRegions << new DropWidget::DropRegion(this, "Points", description, true, [this, datasetName]() {
                            _points.setDatasetName(datasetName);
                        });
                    }
                }
            }
        }

        return dropRegions;
    });

    // Respond when the name of the dataset in the dataset reference changes
    connect(&_points, &DatasetRef<Points>::datasetNameChanged, this, [this](const QString& oldDatasetName, const QString& newDatasetName) {

        // Update the current dataset name label
        _currentDatasetNameLabel->setText(QString("Current points dataset: %1").arg(newDatasetName));

        // Only show the drop indicator when nothing is loaded in the dataset reference
        _dropWidget->setShowDropIndicator(newDatasetName.isEmpty());
    });
}

void ExampleViewPlugin::onDataEvent(hdps::DataEvent* dataEvent)
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

ViewPlugin* ExampleViewPluginFactory::produce()
{
    return new ExampleViewPlugin(this);
}

hdps::DataTypes ExampleViewPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;

    // This example analysis plugin is compatible with points datasets
    supportedTypes.append(PointType);

    return supportedTypes;
}
