#include "ExampleLoaderPlugin.h"

#include "../Common/common.h"

#include "PointData/PointData.h"
#include "Set.h"

#include <QtCore>
#include <QtDebug>

#include <random>

Q_PLUGIN_METADATA(IID "studio.manivault.ExampleLoaderPlugin")

using namespace mv;

// =============================================================================
// View
// =============================================================================

ExampleLoaderPlugin::ExampleLoaderPlugin(const PluginFactory* factory):
    LoaderPlugin(factory),
    _dataSetName("")
{
    
}

ExampleLoaderPlugin::~ExampleLoaderPlugin(void)
{
    
}

/**
 * Mandatory plugin override function. Any initial state can be set here.
 * This function gets called when an instance of the plugin is created.
 * In this case when someone select the loader option from the menu.
 */
void ExampleLoaderPlugin::init()
{
    addNotification(getExampleNotificationMessage());
}

/**
 * Mandatory override function. Gets called when someone selects the Example Loader option
 * from the menu containing loader plugins. This function is responsible for opening the
 * file(s) the user wants to open, process them appropriately and pass the final data to the core.
 */
void ExampleLoaderPlugin::loadData()
{
    _dataSetName = "ExampleData";
    auto points = mv::data().createDataset<Points>("Points", _dataSetName);
    
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

    // Passing example data with 1000 points and 2 dimensions
    points->setData(exampleData.data(), numPoints, numDimensions);

    qDebug() << "Number of dimensions: " << points->getNumDimensions();

    // Notify the core system of the new data
    events().notifyDatasetAdded(points);

    qDebug() << "Example file loaded. Num data points: " << points->getNumPoints();
}

ExampleLoaderPluginFactory::ExampleLoaderPluginFactory()
{
    getPluginMetadata().setDescription("Example loader plugin");
    getPluginMetadata().setSummary("This example shows how to implement a data loader plugin in ManiVault Studio.");
    getPluginMetadata().setCopyrightHolder({ "BioVault (Biomedical Visual Analytics Unit LUMC - TU Delft)" });
    getPluginMetadata().setAuthors({
        { "J. Thijssen", { "Software architect" }, { "LUMC", "TU Delft" } },
        { "T. Kroes", { "Lead software architect" }, { "LUMC" } },
        { "A. Vieth", { "Plugin developer", "Maintainer" }, { "LUMC", "TU Delft" } }
	});
    getPluginMetadata().setOrganizations({
        { "LUMC", "Leiden University Medical Center", "https://www.lumc.nl/en/" },
        { "TU Delft", "Delft university of technology", "https://www.tudelft.nl/" }
	});
    getPluginMetadata().setLicenseText("This plugin is distributed under the [LGPL v3.0](https://www.gnu.org/licenses/lgpl-3.0.en.html) license.");
}

ExampleLoaderPlugin* ExampleLoaderPluginFactory::produce()
{
    return new ExampleLoaderPlugin(this);
}

mv::DataTypes ExampleLoaderPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;
    supportedTypes.append(PointType);
    return supportedTypes;
}
