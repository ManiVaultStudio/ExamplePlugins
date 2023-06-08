#include "ExampleActionsViewPlugin.h"

#include <event/Event.h>

#include <actions/GroupAction.h>
#include <actions/ToggleAction.h>
#include <actions/TriggerAction.h>
#include <actions/ColorAction.h>
#include <actions/ColorMapAction.h>
#include <actions/DecimalAction.h>
#include <actions/IntegralAction.h>
#include <actions/DecimalRangeAction.h>
#include <actions/IntegralRangeAction.h>
#include <actions/DecimalRectangleAction.h>
#include <actions/IntegralRectangleAction.h>
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
#include <actions/HorizontalGroupAction.h>
#include <actions/VerticalGroupAction.h>
#include <actions/HorizontalToolbarAction.h>

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

    //groupAction->setShowLabels(false);

    auto addAction = [this, groupAction](WidgetAction* action, bool setConnectionPermissionsToAll = true, std::int32_t defaultWidgetFlags = -1) -> void {
        if (setConnectionPermissionsToAll)
            action->setConnectionPermissionsToAll();
        
        if (defaultWidgetFlags >= 0)
            action->setDefaultWidgetFlags(defaultWidgetFlags);

        _actions << action;
    };

    auto toggleAction               = new ToggleAction(this, "Toggle");
    auto triggerAction              = new TriggerAction(this, "Trigger");
    auto colorAction                = new ColorAction(this, "Color");
    auto colorMapAction             = new ColorMapAction(this, "ColorMap");
    auto decimalAction              = new DecimalAction(this, "Decimal");
    auto integralAction             = new IntegralAction(this, "Integral");
    auto decimalRangeAction         = new DecimalRangeAction(this, "Decimal Range");
    auto integralRangeAction        = new IntegralRangeAction(this, "Integral Range");
    auto decimalRectangleAction     = new DecimalRectangleAction(this, "Decimal Rectangle");
    auto integralRectangleAction    = new IntegralRectangleAction(this, "Integral Rectangle");
    auto optionAAction              = new OptionAction(this, "Option A", { "A", "B", "C" });
    auto optionBAction              = new OptionAction(this, "Option B", { "C", "D", "E" });
    auto optionsAAction             = new OptionsAction(this, "Options A", { "A", "B", "C" });
    auto optionsBAction             = new OptionsAction(this, "Options B", { "B", "C", "D" });
    auto stringAction               = new StringAction(this, "String");
    auto stringsAction              = new StringsAction(this, "Strings");
    auto datasetPickerAction        = new DatasetPickerAction(this, "Dataset");
    auto dimensionPickerAction      = new DimensionPickerAction(this, "Dimension");
    auto dimensionsPickerAction     = new DimensionsPickerAction(this, "Dimensions");
    auto imageAction                = new ImageAction(this, "Image");
    auto windowLevelAction          = new WindowLevelAction(this, "Window/level");
    auto versionAction              = new VersionAction(this, "Version");
    auto directoryPickerAction      = new DirectoryPickerAction(this, "Directory");
    auto filePickerAction           = new FilePickerAction(this, "File");
    auto horizontalGroupAction      = new HorizontalGroupAction(this, "Horizontal group");
    auto verticalGroupAction        = new VerticalGroupAction(this, "Vertical group");
    auto horizontalToolbarAction    = new HorizontalToolbarAction(this, "Horizontal Toolbar");

    optionAAction->setPlaceHolderString("Select option");
    optionBAction->setPlaceHolderString("Select option");

    connect(datasetPickerAction, &DatasetPickerAction::datasetPicked, this, [this, dimensionPickerAction, dimensionsPickerAction](Dataset<DatasetImpl> dataset) -> void {
        dimensionPickerAction->setPointsDataset(Dataset<Points>(dataset));
        dimensionsPickerAction->setPointsDataset(Dataset<Points>(dataset));
    });

    horizontalGroupAction->setShowLabels(false);
    horizontalGroupAction->addAction(optionAAction);
    horizontalGroupAction->addAction(stringAction);

    verticalGroupAction->addAction(optionAAction);
    verticalGroupAction->addAction(stringAction);

    auto vectorTypeAction   = new OptionAction(this, "Vector Type", { "2D", "3D" });
    auto vectorXAction      = new DecimalAction(this, "X");
    auto vectorYAction      = new DecimalAction(this, "Y");
    auto vectorAction       = new HorizontalGroupAction(this, "Vector");

    const auto vectorTypeChanged = [vectorTypeAction, vectorYAction]() -> void {
        vectorYAction->setVisible(vectorTypeAction->getCurrentText() == "3D");
    };

    vectorTypeChanged();

    connect(vectorTypeAction, &OptionAction::currentIndexChanged, this, vectorTypeChanged);

    vectorAction->addAction(vectorTypeAction);
    vectorAction->addAction(vectorXAction);
    vectorAction->addAction(vectorYAction);

    colorAction->setConfigurationFlag(WidgetAction::ConfigurationFlag::ForceCollapsedInGroup);
    decimalAction->setConfigurationFlag(WidgetAction::ConfigurationFlag::ForceCollapsedInGroup);

    horizontalToolbarAction->addAction(vectorAction, 1);
    horizontalToolbarAction->addAction(colorAction, 3);
    horizontalToolbarAction->addAction(colorMapAction, 8);
    horizontalToolbarAction->addAction(decimalAction, 1);

    addAction(toggleAction);
    addAction(triggerAction);
    addAction(colorAction);
    addAction(colorMapAction);
    addAction(decimalAction);
    addAction(integralAction);
    addAction(decimalRangeAction);
    addAction(integralRangeAction);
    addAction(decimalRectangleAction);
    addAction(integralRectangleAction);
    addAction(optionAAction);
    addAction(optionBAction);
    addAction(optionsAAction);
    addAction(optionsBAction);
    addAction(stringAction);
    addAction(stringsAction);
    addAction(datasetPickerAction, true, OptionAction::ComboBox | OptionAction::Clearable);
    addAction(dimensionPickerAction);
    addAction(dimensionsPickerAction);
    addAction(imageAction, false);
    addAction(windowLevelAction);
    addAction(versionAction);
    addAction(directoryPickerAction, false);
    addAction(filePickerAction, false);
    addAction(horizontalGroupAction, false);
    addAction(verticalGroupAction, false);
    addAction(horizontalToolbarAction, false);

    for (auto action : _actions)
        groupAction->addAction(action);

    layout->setAlignment(Qt::AlignLeft);
    //layout->addWidget(horizontalToolbarAction->createWidget(&getWidget()));
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
