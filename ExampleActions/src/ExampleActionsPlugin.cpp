#include "ExampleActionsPlugin.h"

#include "../Common/common.h"

#include <QDebug>

#include <actions/TriggerAction.h>
#include <actions/TriggersAction.h>
#include <actions/ToggleAction.h>
#include <actions/OptionAction.h>
#include <actions/OptionsAction.h>
#include <actions/ColorAction.h>
#include <actions/ColorMap1DAction.h>

Q_PLUGIN_METADATA(IID "studio.manivault.ExampleActionsPlugin")using namespace mv;

ExampleActionsPlugin::ExampleActionsPlugin(const PluginFactory* factory) :
    ViewPlugin(factory)
{
}

void ExampleActionsPlugin::init()
{
    auto layout = new QVBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(&_actionsWidget);

    getWidget().setLayout(layout);

    _actionsWidget.addAction("Trigger", "Trigger", [this](QWidget* parent) -> WidgetAction* {
        return new TriggerAction(this, "Example trigger action");
    }, {
        { "Icon", 1 },
        { "Text", 2 },
    }, { "Icon", "Text" });

    _actionsWidget.addAction("Trigger", "Triggers", [this](QWidget* parent) -> WidgetAction* {
    	return new TriggersAction(this, "Example Triggers action", { { "Trigger 1", "Action 1" }, { "Trigger 2", "Action 2" } });
    }, {
    	{ "Horizontal", 1 },
    	{ "Vertical", 2 },
    }, { "Horizontal" });

    _actionsWidget.addAction("Textual", "Strings", [this](QWidget* parent) -> WidgetAction* {
    	return new StringsAction(this, "Example strings action", { "String 1", "String 2", "String 3" });
    }, {
        { "MayEdit", 1 },
        { "ListView", 2 }
    }, { "ListView" });

    _actionsWidget.addAction("Option", "Toggle", [this](QWidget* parent) -> WidgetAction* {
        return new ToggleAction(this, "Example toggle action", true);
    }, {
        { "Icon", 1 },
        { "Text", 2 },
        { "CheckBox", 4 },
        { "PushButton", 8 }
    }, { "CheckBox" });

    _actionsWidget.addAction("Option", "Option select", [this](QWidget* parent) -> WidgetAction* {
        return new OptionAction(this, "Example option action", { "A", "B", "C" });
    }, {
        { "ComboBox", 1 },
        { "LineEdit", 2 },
        { "HorizontalButtons", 4 },
        { "VerticalButtons", 8 },
        { "Clearable", 10 }
    }, { "ComboBox" });

    _actionsWidget.addAction("Option", "Options select", [this](QWidget* parent) -> WidgetAction* {
        return new OptionsAction(this, "Example options action", { "A", "B", "C" }, { "A", "B" });
    }, {
        { "ComboBox", 1 },
        { "ListView", 2 },
        { "Selection", 4 },
        { "File", 8 },
        { "Tags", 10 }
    }, { "ComboBox" });

    _actionsWidget.addAction("Color", "Color", [this](QWidget* parent) -> WidgetAction* {
        return new ColorAction(this, "Example color action");
    });

    _actionsWidget.addAction("Color", "1D color map", [this](QWidget* parent) -> WidgetAction* {
        return new ColorMap1DAction(this, "Example 1D color map action");
    }, {
        { "Settings", 1 },
        { "EditRange", 2 }
    }, { "Settings" });
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
