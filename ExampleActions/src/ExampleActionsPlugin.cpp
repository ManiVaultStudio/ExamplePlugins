#include "ExampleActionsPlugin.h"
#include "ActionsWidget.h"

#include "../Common/common.h"

#include <QDebug>

Q_PLUGIN_METADATA(IID "studio.manivault.ExampleActionsPlugin")

using namespace mv;

ExampleActionsPlugin::ExampleActionsPlugin(const PluginFactory* factory) :
    ViewPlugin(factory)
{
}

void ExampleActionsPlugin::init()
{
    auto layout = new QVBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(new ActionsWidget());

    getWidget().setLayout(layout);
}

ExampleActionsPluginFactory::ExampleActionsPluginFactory()
{
    getPluginMetadata().setDescription("Example actions view plugin");
    getPluginMetadata().setSummary("This example shows how to use action GUI building blocks in ManiVault Studio.");
    getPluginMetadata().setCopyrightHolder({ "BioVault (Biomedical Visual Analytics Unit LUMC - TU Delft)" });
    getPluginMetadata().setAuthors({
        { "T. Kroes", { "Lead software architect" }, { "LUMC" } }
	});
    getPluginMetadata().setOrganizations({
        { "LUMC", "Leiden University Medical Center", "https://www.lumc.nl/en/" },
        { "TU Delft", "Delft university of technology", "https://www.tudelft.nl/" }
	});
    getPluginMetadata().setLicenseText("This plugin is distributed under the [LGPL v3.0](https://www.gnu.org/licenses/lgpl-3.0.en.html) license.");
}

ViewPlugin* ExampleActionsPluginFactory::produce()
{
    return new ExampleActionsPlugin(this);
}

mv::DataTypes ExampleActionsPluginFactory::supportedDataTypes() const
{
    return {};
}
