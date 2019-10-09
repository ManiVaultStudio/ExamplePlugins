#include "ExampleSettingsWidget.h"

ExampleSettingsWidget::ExampleSettingsWidget() {
    // Set the width of the settings widget
    setFixedWidth(200);

    // Set the width of the data selection combobox
    dataOptions.setFixedWidth(100);

    // Connect selecting an option from the data combobox to emitting a dataSetPicked signal
    connect(&dataOptions, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(dataSetPicked(QString)));

    // Add all the parts of the settings widget together
    addWidget(&dataOptions);
}

QString ExampleSettingsWidget::currentData()
{
    return dataOptions.currentText();
}
