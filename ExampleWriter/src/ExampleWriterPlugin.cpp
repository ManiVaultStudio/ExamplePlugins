#include "ExampleWriterPlugin.h"

#include "PointData/PointData.h"

#include <random>
#include <fstream>
#include <filesystem>

Q_PLUGIN_METADATA(IID "studio.manivault.ExampleWriterPlugin")

using namespace mv;

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
    addNotification("ManiVault Studio features a notification system that can be triggered from:<br>"
        "<table>"
        "<tr>"
        "<th style='text-align: left;'>Within a plugin:  </th>"
        "<td><a href='https://github.com/ManiVaultStudio/core/blob/3088ad09de6b5a5f3e84b24ffba44294bbb8001d/ManiVault/src/Plugin.h#L170'>addNotification(...)</a></td>"
        "</tr>"
        "<tr>"
        "<th style='text-align: left;'>The help manager:  </th>"
        "<td><a href='https://github.com/ManiVaultStudio/core/blob/126bd905d64d9b795dce188a06e9321eef132bd3/ManiVault/src/AbstractHelpManager.h#L100'>mv::help().addNotification(...)</a></td>"
        "</tr>"
        "</table>"
    );
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

// =============================================================================
// Plugin Factory 
// =============================================================================

ExampleWriterPluginFactory::ExampleWriterPluginFactory()
{
    getPluginMetadata().setDescription("Example writer plugin");
    getPluginMetadata().setSummary("This example shows how to implement a basic data writer plugin in ManiVault Studio.");
    getPluginMetadata().setCopyrightHolder({ "BioVault (Biomedical Visual Analytics Unit LUMC - TU Delft)" });
    getPluginMetadata().setAuthors({
        { "A. Vieth", { "Plugin developer", "Maintainer" }, { "LUMC", "TU Delft" } },
        { "J. Thijssen", { "Software architect" }, { "LUMC", "TU Delft" } },
        { "T. Kroes", { "Lead software architect" }, { "LUMC" } }
	});
    getPluginMetadata().setOrganizations({
        { "LUMC", "Leiden University Medical Center", "https://www.lumc.nl/en/" },
        { "TU Delft", "Delft university of technology", "https://www.tudelft.nl/" }
        });
    getPluginMetadata().setLicenseText("This plugin is distributed under the [LGPL v3.0](https://www.gnu.org/licenses/lgpl-3.0.en.html) license.");
}

ExampleWriterPlugin* ExampleWriterPluginFactory::produce()
{
    return new ExampleWriterPlugin(this);
}

mv::DataTypes ExampleWriterPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;
    supportedTypes.append(PointType);
    return supportedTypes;
}

PluginTriggerActions ExampleWriterPluginFactory::getPluginTriggerActions(const mv::Datasets& datasets) const
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