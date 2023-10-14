#include "ExampleTransformationPlugin.h"

#include <PointData/PointData.h>

#include <actions/PluginTriggerAction.h>

#include <QtCore>
#include <QDebug>

#include <cmath>

Q_PLUGIN_METADATA(IID "nl.BioVault.ExampleTransformationPlugin")

using namespace mv;
using namespace mv::util;

const QMap<ExampleTransformationPlugin::Type, QString> ExampleTransformationPlugin::types = QMap<ExampleTransformationPlugin::Type, QString>({
    { ExampleTransformationPlugin::Type::Abs, "Abs (in-place)" },
    { ExampleTransformationPlugin::Type::Pow2, "Pow2 (derived data)" }
    });


ExampleTransformationPlugin::ExampleTransformationPlugin(const PluginFactory* factory) :
    TransformationPlugin(factory),
    _type(Type::Abs)
{
}

void ExampleTransformationPlugin::transform()
{
    for (auto inputDataset : getInputDatasets()) {
        auto points = Dataset<Points>(inputDataset);

        if (!points.isValid())
            continue;

        // Get reference to dataset task for reporting progress
        auto& datasetTask = inputDataset->getTask();

        datasetTask.setName("Transforming");
        datasetTask.setRunning();
        datasetTask.setProgressDescription(QString("%1 transformation").arg(getTypeName(_type)));

        switch (_type)
        {
            // Transform points in place
            case ExampleTransformationPlugin::Type::Abs:
            {
                points->setLocked(true);

                points->visitData([this, &points, &datasetTask](auto pointData) {
                    std::uint32_t noPointsProcessed = 0;

                    for (auto point : pointData) {
                        for (std::uint32_t dimensionIndex = 0; dimensionIndex < points->getNumDimensions(); dimensionIndex++) {
                            point[dimensionIndex] = std::abs(point[dimensionIndex]);
                        }

                        ++noPointsProcessed;

                        if (noPointsProcessed % 1000 == 0) {
                            datasetTask.setProgress(static_cast<float>(noPointsProcessed) / static_cast<float>(points->getNumPoints()));

                            QApplication::processEvents();
                        }
                    }
                    });

                points->setLocked(false);

                events().notifyDatasetDataChanged(points);

                break;
            }
            // Create new data set
            case ExampleTransformationPlugin::Type::Pow2:
            {
                auto derivedData = _core->createDerivedDataset<Points>(points->getGuiName() + "(Pow2)", points);
                events().notifyDatasetAdded(derivedData);

                std::vector<float> transformedData;
                transformedData.resize(points->getNumPoints() * points->getNumDimensions());

                points->constVisitFromBeginToEnd([&transformedData](auto begin, auto end) {
                    std::uint32_t noItemsProcessed = 0;

                    for (auto it = begin; it != end; ++it) {
                        transformedData[noItemsProcessed] = std::pow(*it, 2.0f);
                        noItemsProcessed++;
                    }
                    });


                derivedData->setData(transformedData.data(), points->getNumPoints(), points->getNumDimensions());
                events().notifyDatasetDataChanged(derivedData);

                break;
            }
            default:
                break;
        }

        datasetTask.setProgress(1.0f);
        datasetTask.setFinished();

    }
}


ExampleTransformationPlugin::Type ExampleTransformationPlugin::getType() const
{
    return _type;
}

void ExampleTransformationPlugin::setType(const Type& type)
{
    if (type == _type)
        return;

    _type = type;
}

QString ExampleTransformationPlugin::getTypeName(const Type& type)
{
    return types[type];
}

ExampleTransformationPlugin* ExampleTransformationPluginFactory::produce()
{
    // Return a new instance of the example transformation plugin
    return new ExampleTransformationPlugin(this);
}

mv::DataTypes ExampleTransformationPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;

    // This example transformation plugin is compatible with points datasets
    supportedTypes.append(PointType);

    return supportedTypes;
}

PluginTriggerActions ExampleTransformationPluginFactory::getPluginTriggerActions(const mv::Datasets& datasets) const
{
    PluginTriggerActions pluginTriggerActions;

    const auto numberOfDatasets = datasets.count();

    if (PluginFactory::areAllDatasetsOfTheSameType(datasets, PointType)) {
        if (numberOfDatasets >= 1 && datasets.first()->getDataType() == PointType) {
            const auto addPluginTriggerAction = [this, &pluginTriggerActions, datasets](const ExampleTransformationPlugin::Type& type) -> void {
                const auto typeName = ExampleTransformationPlugin::getTypeName(type);

                auto pluginTriggerAction = new PluginTriggerAction(const_cast<ExampleTransformationPluginFactory*>(this), this, QString("Example/%1").arg(typeName), QString("Perform %1 data transformation").arg(typeName), getIcon(), [this, datasets, type](PluginTriggerAction& pluginTriggerAction) -> void {
                    for (auto dataset : datasets) {
                        auto pluginInstance = dynamic_cast<ExampleTransformationPlugin*>(plugins().requestPlugin(getKind()));

                        pluginInstance->setInputDatasets(datasets);
                        pluginInstance->setType(type);
                        pluginInstance->transform();
                    }
                    });

                pluginTriggerActions << pluginTriggerAction;
            };

            addPluginTriggerAction(ExampleTransformationPlugin::Type::Abs);
            addPluginTriggerAction(ExampleTransformationPlugin::Type::Pow2);
        }
    }

    return pluginTriggerActions;
}

