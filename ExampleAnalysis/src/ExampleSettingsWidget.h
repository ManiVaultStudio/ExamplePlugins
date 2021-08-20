#pragma once

#include "actions/Actions.h"

#include <QComboBox>

#include <vector>

using namespace hdps::gui;

/**
 * Main settings widget, inherits from base SettingsWidget class
 */
class ExampleSettingsWidget : public hdps::gui::WidgetAction
{
    Q_OBJECT

public:
    ExampleSettingsWidget();

    QString currentData();

signals:
    void dataSetPicked(QString);

public:
    QComboBox dataOptions;
};
