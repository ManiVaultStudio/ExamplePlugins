#include "ExampleWriterPlugin.h"

#include "PointData/PointData.h"

#include <random>
#include <fstream>
#include <filesystem>

Q_PLUGIN_METADATA(IID "nl.tudelft.ExampleWriterPlugin")

using namespace hdps;

// =============================================================================
// View
// =============================================================================

ExampleWriterPlugin::ExampleWriterPlugin(const PluginFactory* factory) : 
    WriterPlugin(factory),
    _dataSetName("")
{
    
}

ExampleWriterPlugin::~ExampleWriterPlugin(void)
{
    
}

/**
 * Mandatory plugin override function. Any initial state can be set here.
 * This function gets called when an instance of the plugin is created.
 * In this case when someone selects the writer option from the menu.
 */
void ExampleWriterPlugin::init()
{

}

/**
 * Mandatory override function. Gets called when someone selects the Example writer option
 * This function is responsible for opening the writing files to disk.
 */
void ExampleWriterPlugin::writeData()
{
    // Create 2D example data by randomly generating 1000 points
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution;

    int numPoints = 1000;
    int numDimensions = 2;

    std::vector<float> exampleData;
    for (int i = 0; i < numPoints * numDimensions; i++)
    {
        exampleData.push_back(distribution(generator));
    }

    // Write random data to current working directory 
    {
        auto currentPath = std::filesystem::current_path();
        currentPath /= "exampleWriterOutput.bin";

        std::ofstream fout(currentPath.string() + "", std::ofstream::out | std::ofstream::binary);
        fout.write(reinterpret_cast<const char*>(exampleData.data()), exampleData.size() * sizeof(float));
        fout.close();
    }
}

ExampleWriterPlugin* ExampleWriterPluginFactory::produce()
{
    return new ExampleWriterPlugin(this);
}

hdps::DataTypes ExampleWriterPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;
    supportedTypes.append(PointType);
    return supportedTypes;
}

PluginTriggerActions ExampleWriterPluginFactory::getPluginTriggerActions(const hdps::Datasets& datasets) const
{
    PluginTriggerActions pluginTriggerActions;

    const auto getPluginInstance = [this](const Dataset<Points>& dataset) -> ExampleWriterPlugin* {
        return dynamic_cast<ExampleWriterPlugin*>(plugins().requestPlugin(getKind(), { dataset }));
    };

    if (PluginFactory::areAllDatasetsOfTheSameType(datasets, PointType)) {
        if (datasets.count() >= 1) {
            auto pluginTriggerAction = new PluginTriggerAction(const_cast<ExampleWriterPluginFactory*>(this), this, "Example Writer", "Export random data to disk (not the selected data set)", getIcon(), [this, getPluginInstance, datasets](PluginTriggerAction& pluginTriggerAction) -> void {
                for (auto dataset : datasets)
                    getPluginInstance(dataset);
                });

            pluginTriggerActions << pluginTriggerAction;
        }
    }

    return pluginTriggerActions;
}