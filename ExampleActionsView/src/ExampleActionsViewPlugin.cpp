#include "ExampleActionsViewPlugin.h"

#include <event/Event.h>

#include <actions/GroupAction.h>
#include <actions/ToggleAction.h>
#include <actions/TriggerAction.h>
#include <actions/ColorAction.h>
#include <actions/ColorMapAction.h>
#include <actions/DecimalAction.h>
#include <actions/DecimalRangeAction.h>
#include <actions/IntegralAction.h>
#include <actions/OptionAction.h>
#include <actions/OptionsAction.h>
#include <actions/StringAction.h>
#include <actions/StringsAction.h>
#include <actions/DirectoryPickerAction.h>
#include <actions/FilePickerAction.h>
#include <actions/DatasetPickerAction.h>
#include <actions/ImageAction.h>
#include <actions/VersionAction.h>
#include <actions/WindowLevelAction.h>

#include <PointData/DimensionPickerAction.h>
#include <PointData/DimensionsPickerAction.h>

#include <QDebug>
#include <QMimeData>

Q_PLUGIN_METADATA(IID "NL.ManiVault.ExampleActionsViewPlugin")

using namespace hdps;

ExampleActionsViewPlugin::ExampleActionsViewPlugin(const PluginFactory* factory) :
    ViewPlugin(factory)
{
}

void ExampleActionsViewPlugin::init()
{
    auto layout = new QVBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);

    auto groupAction = new GroupAction(this, "Group");

    auto addAction = [this, groupAction](WidgetAction* action) -> void {
        action->setConnectionPermissionsToAll();
        
        _actions << action;
    };

    addAction(new ToggleAction(this, "Toggle"));
    addAction(new TriggerAction(this, "Trigger"));
    addAction(new ColorAction(this, "Color"));
    addAction(new ColorMapAction(this, "ColorMap"));
    addAction(new DecimalAction(this, "Decimal"));
    addAction(new DecimalRangeAction(this, "Decimal Range"));
    addAction(new IntegralAction(this, "Integral"));
    addAction(new OptionAction(this, "Option"));
    addAction(new OptionsAction(this, "Options"));
    addAction(new StringAction(this, "String"));
    addAction(new StringsAction(this, "Strings"));
    addAction(new DirectoryPickerAction(this, "Directory"));
    addAction(new FilePickerAction(this, "File"));
    addAction(new DatasetPickerAction(this, "Dataset"));
    addAction(new ImageAction(this, "Image"));
    addAction(new VersionAction(this));
    addAction(new WindowLevelAction(this));
    addAction(new DimensionPickerAction(this, "Dimension"));
    addAction(new DimensionsPickerAction(this));

    groupAction->setActions(_actions);

    layout->addWidget(groupAction->createWidget(&getWidget()));
    layout->addStretch(1);

    getWidget().setLayout(layout);
}

void ExampleActionsViewPlugin::fromVariantMap(const QVariantMap& variantMap)
{
    ViewPlugin::fromVariantMap(variantMap);

    for (auto action : _actions)
        action->fromParentVariantMap(variantMap);
}

QVariantMap ExampleActionsViewPlugin::toVariantMap() const
{
    auto variantMap = ViewPlugin::toVariantMap();

    for (auto action : _actions)
        action->insertIntoVariantMap(variantMap);

    return variantMap;
}

ViewPlugin* ExampleActionsViewPluginFactory::produce()
{
    return new ExampleActionsViewPlugin(this);
}

hdps::DataTypes ExampleActionsViewPluginFactory::supportedDataTypes() const
{
    return DataTypes();
}
