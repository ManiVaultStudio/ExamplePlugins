#include "SettingsAction.h"

#include "ExampleViewGLPlugin.h"

#include <QMenu>

using namespace hdps::gui;

SettingsAction::SettingsAction(QObject* parent, const QString& title) :
    VerticalGroupAction(parent, title),
    _exampleViewGLPlugin(dynamic_cast<ExampleViewGLPlugin*>(parent)),
    _datasetNameAction(this, "Dataset Name"),
    _xDimensionPickerAction(this, "X"),
    _yDimensionPickerAction(this, "Y")
{
    setIcon(hdps::Application::getIconFont("FontAwesome").getIcon("ruler-combined"));
    setLabelSizingType(LabelSizingType::Auto);

    addAction(&_datasetNameAction);
    addAction(&_xDimensionPickerAction);
    addAction(&_yDimensionPickerAction);

    _datasetNameAction.setToolTip("Name of currently shown dataset");
    _xDimensionPickerAction.setToolTip("X dimension");
    _yDimensionPickerAction.setToolTip("Y dimension");

    _datasetNameAction.setEnabled(false);
    _datasetNameAction.setText("[No dataset loaded yet]");

    connect(&_xDimensionPickerAction, &DimensionPickerAction::currentDimensionIndexChanged, [this](const std::uint32_t& currentDimensionIndex) {
        _exampleViewGLPlugin->updateData();
    });

    connect(&_yDimensionPickerAction, &DimensionPickerAction::currentDimensionIndexChanged, [this](const std::uint32_t& currentDimensionIndex) {
        _exampleViewGLPlugin->updateData();
    });
}

QMenu* SettingsAction::getContextMenu(QWidget* parent /*= nullptr*/)
{
    auto menu = new QMenu("Position", parent);

    auto xDimensionMenu = new QMenu("X dimension");
    auto yDimensionMenu = new QMenu("Y dimension");

    xDimensionMenu->addAction(&_xDimensionPickerAction);
    yDimensionMenu->addAction(&_yDimensionPickerAction);

    menu->addMenu(xDimensionMenu);
    menu->addMenu(yDimensionMenu);

    return menu;
}
