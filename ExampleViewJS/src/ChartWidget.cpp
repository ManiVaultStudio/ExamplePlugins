#include "ChartWidget.h"
#include "ExampleViewJSPlugin.h"

#include <QDebug>

using namespace hdps;
using namespace hdps::gui;

// =============================================================================
// ChartCommObject
// =============================================================================

ChartCommObject::ChartCommObject() :
    _selectedIDsFromJS()
{
}

void ChartCommObject::js_qt_passSelectionToQt(QVariantList data){
    // convert data structure
    _selectedIDsFromJS.clear();
    std::for_each(data.begin(), data.end(), [this](const auto &dat) {_selectedIDsFromJS.push_back(dat.toUInt()); });
    
    // notify ManiVault core and thereby other plugins about new selection
    emit passSelectionToCore(_selectedIDsFromJS);
}


// =============================================================================
// ChartWidget
// =============================================================================

ChartWidget::ChartWidget(ExampleViewJSPlugin* viewJSPlugin):
    _viewJSPlugin(viewJSPlugin),
    _comObject()
{
    // See the ExampleViewPlugin project for more info on drag&drop behavior
    setAcceptDrops(true);

    // Ensure linking to the resources defined in res/example_chart.qrc
    Q_INIT_RESOURCE(example_chart);

    // ManiVault and Qt create a "QtBridge" object on the js side which represents _comObject
    // there, we can connect the signals qt_js_* and call the slots js_qt_* from our communication object
    init(&_comObject);

    layout()->setContentsMargins(0, 0, 0, 0);
}

void ChartWidget::initWebPage()
{
    qDebug() << "ChartWidget: WebChannel bridge is available.";
    // call to data load, used when plugin is opened via right-clicking data set
    _viewJSPlugin->convertDataAndUpdateChart();
}

