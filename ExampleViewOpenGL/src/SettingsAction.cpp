#include "SettingsAction.h"
#include "GlobalSettingsAction.h"

#include "ExampleViewGLPlugin.h"

#include <QHBoxLayout>

using namespace mv::gui;

SettingsAction::SettingsAction(QObject* parent, const QString& title) :
    GroupAction(parent, title),
    _exampleViewGLPlugin(dynamic_cast<ExampleViewGLPlugin*>(parent)),
    _datasetNameAction(this, "Dataset Name"),
    _xDimensionPickerAction(this, "X"),
    _yDimensionPickerAction(this, "Y"),
    _pointSizeAction(this, "Point Size", 1, 50, 10),
    _pointOpacityAction(this, "Opacity", 0.f, 1.f, 0.75f, 2)
{
    setText("Settings");

    addAction(&_datasetNameAction);
    addAction(&_xDimensionPickerAction);
    addAction(&_yDimensionPickerAction);
    addAction(&_pointSizeAction);
    addAction(&_pointOpacityAction);

    _datasetNameAction.setToolTip("Name of currently shown dataset");
    _xDimensionPickerAction.setToolTip("X dimension");
    _yDimensionPickerAction.setToolTip("Y dimension");
    _pointSizeAction.setToolTip("Size of individual points");
    _pointOpacityAction.setToolTip("Opacity of individual points");

    _datasetNameAction.setEnabled(false);
    _datasetNameAction.setText("Dataset name");
    _datasetNameAction.setString(" (No data loaded yet)");

    _pointSizeAction.setValue(mv::settings().getPluginGlobalSettingsGroupAction<GlobalSettingsAction>(_exampleViewGLPlugin)->getDefaultPointSizeAction().getValue());
    _pointOpacityAction.setValue(mv::settings().getPluginGlobalSettingsGroupAction<GlobalSettingsAction>(_exampleViewGLPlugin)->getDefaultPointOpacityAction().getValue());

    connect(&_xDimensionPickerAction, &DimensionPickerAction::currentDimensionIndexChanged, _exampleViewGLPlugin, &ExampleViewGLPlugin::updatePlot);
    connect(&_yDimensionPickerAction, &DimensionPickerAction::currentDimensionIndexChanged, _exampleViewGLPlugin, &ExampleViewGLPlugin::updatePlot);
    connect(&_pointSizeAction, &DecimalAction::valueChanged, _exampleViewGLPlugin, &ExampleViewGLPlugin::updatePlot);
    connect(&_pointOpacityAction, &DecimalAction::valueChanged, _exampleViewGLPlugin, &ExampleViewGLPlugin::updatePlot);

}
