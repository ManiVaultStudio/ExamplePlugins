#include "ChartWidget.h"
#include "ExampleViewJSPlugin.h"

#include <QDebug>
#include <QString>

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
    _selectedIDsFromJS.clear();

    if (!data.isEmpty())
    {
        // Convert data structure
        // We will get strings in the form "point 2" from this particular library
        // and need to extract only the seclection ID
        std::for_each(data.begin(), data.end(), [this](const auto& dat) {
            _selectedIDsFromJS.push_back(dat.toString().split(" ").takeLast().toInt() - 1);
            });

        qDebug() << "Selected item:" << _selectedIDsFromJS[0]; // in this case we know that it is only one
    }    
    
    // Notify ManiVault core and thereby other plugins about new selection
    emit passSelectionToCore(_selectedIDsFromJS);
}


// =============================================================================
// ChartWidget
// =============================================================================

ChartWidget::ChartWidget(ExampleViewJSPlugin* viewJSPlugin):
    _viewJSPlugin(viewJSPlugin),
    _comObject()
{
    // For more info on drag&drop behavior, see the ExampleViewPlugin project
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
    // This call ensures data chart setup when this view plugin is opened via the context menu of a data set
    _viewJSPlugin->convertDataAndUpdateChart();
}

