#include "SettingsAction.h"

#include "ExampleViewGLPlugin.h"

#include <QMenu>
#include <QHBoxLayout>

using namespace hdps::gui;

SettingsAction::SettingsAction(QObject* parent, const QString& title) :
    WidgetAction(parent, title),
    _exampleViewGLPlugin(dynamic_cast<ExampleViewGLPlugin*>(parent)),
    _datasetNameAction(this, "Dataset Name"),
    _xDimensionPickerAction(this, "X"),
    _yDimensionPickerAction(this, "Y"),
    _pointSizeAction(this, "Point Size", 1, 50, 10)
{
    setText("Settings");

    _datasetNameAction.setToolTip("Name of currently shown dataset");
    _xDimensionPickerAction.setToolTip("X dimension");
    _yDimensionPickerAction.setToolTip("Y dimension");
    _pointSizeAction.setToolTip("Size of individual points");

    _datasetNameAction.setEnabled(false);
    _datasetNameAction.setText("Dataset name");
    _datasetNameAction.setString(" (No data loaded yet)");

    connect(&_xDimensionPickerAction, &DimensionPickerAction::currentDimensionIndexChanged, [this](const std::uint32_t& currentDimensionIndex) {
        _exampleViewGLPlugin->updateData();
    });

    connect(&_yDimensionPickerAction, &DimensionPickerAction::currentDimensionIndexChanged, [this](const std::uint32_t& currentDimensionIndex) {
        _exampleViewGLPlugin->updateData();
    });

    connect(&_pointSizeAction, &DecimalAction::valueChanged, [this](float val) {
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

SettingsAction::Widget::Widget(QWidget* parent, SettingsAction* settingsAction) :
    WidgetActionWidget(parent, settingsAction)
{
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    auto layout = new QHBoxLayout();

    const int margin = 5;
    layout->setContentsMargins(margin, margin, margin, margin);
    layout->setSpacing(2);

    layout->addWidget(settingsAction->getDatasetNameAction().createLabelWidget(this));
    layout->addWidget(settingsAction->getDatasetNameAction().createWidget(this));

    layout->addWidget(settingsAction->getXDimensionPickerAction().createLabelWidget(this));
    layout->addWidget(settingsAction->getXDimensionPickerAction().createWidget(this));

    layout->addWidget(settingsAction->getYDimensionPickerAction().createLabelWidget(this));
    layout->addWidget(settingsAction->getYDimensionPickerAction().createWidget(this));

    layout->addWidget(settingsAction->getPointSizeAction().createLabelWidget(this));
    layout->addWidget(settingsAction->getPointSizeAction().createWidget(this));

    setLayout(layout);
}