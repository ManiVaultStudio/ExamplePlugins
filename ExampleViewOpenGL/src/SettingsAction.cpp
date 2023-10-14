#include "SettingsAction.h"

#include "ExampleViewGLPlugin.h"

#include <QHBoxLayout>

using namespace mv::gui;

SettingsAction::SettingsAction(QObject* parent, const QString& title) :
    WidgetAction(parent, title),
    _exampleViewGLPlugin(dynamic_cast<ExampleViewGLPlugin*>(parent)),
    _datasetNameAction(this, "Dataset Name"),
    _xDimensionPickerAction(this, "X"),
    _yDimensionPickerAction(this, "Y"),
    _pointSizeAction(this, "Point Size", 1, 50, 10),
    _pointOpacityAction(this, "Opacity", 0.f, 1.f, 0.75f, 2)
{
    setText("Settings");

    _datasetNameAction.setToolTip("Name of currently shown dataset");
    _xDimensionPickerAction.setToolTip("X dimension");
    _yDimensionPickerAction.setToolTip("Y dimension");
    _pointSizeAction.setToolTip("Size of individual points");
    _pointOpacityAction.setToolTip("Opacity of individual points");

    _datasetNameAction.setEnabled(false);
    _datasetNameAction.setText("Dataset name");
    _datasetNameAction.setString(" (No data loaded yet)");

    connect(&_xDimensionPickerAction, &DimensionPickerAction::currentDimensionIndexChanged, [this](const std::uint32_t& currentDimensionIndex) {
        _exampleViewGLPlugin->updatePlot();
    });

    connect(&_yDimensionPickerAction, &DimensionPickerAction::currentDimensionIndexChanged, [this](const std::uint32_t& currentDimensionIndex) {
        _exampleViewGLPlugin->updatePlot();
    });

    connect(&_pointSizeAction, &DecimalAction::valueChanged, [this](float val) {
        _exampleViewGLPlugin->updatePlot();
    });

    connect(&_pointOpacityAction, &DecimalAction::valueChanged, [this](float val) {
        _exampleViewGLPlugin->updatePlot();
    });

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

    layout->addWidget(settingsAction->getPointOpacityAction().createLabelWidget(this));
    layout->addWidget(settingsAction->getPointOpacityAction().createWidget(this));

    setLayout(layout);
}