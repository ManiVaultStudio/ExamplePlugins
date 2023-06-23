#pragma once 

#include "widgets/WebWidget.h"

#include <QVariantList>

class ExampleViewJSPlugin;

// =============================================================================
// ParlCoorCommunicationObject
// =============================================================================

class ChartCommObject : public hdps::gui::WebCommunicationObject
{
    Q_OBJECT
public:
    ChartCommObject();

signals:
    // Signals from Qt to JS side
    // This specific signal is used to transfer data from ManiVault to the D3 plot
    // But other communication like messaging selection IDs can be handled the same
    void qt_js_setDataAndPlotInJS(QVariantList& data);

    // Signals Qt internal
    // Used to inform the plugin about new selection: the plugin class then updates ManiVault's core
    void passSelectionToCore(const std::vector<unsigned int>& selectionIDs);

public slots:
    // Invoked from JS side 
    // Used to receive selection IDs from the D3 plot, will emit passSelectionToCore
    void js_qt_passSelectionToQt(QVariantList data);

private:
    std::vector<unsigned int> _selectedIDsFromJS;   // Used for converting incoming selection IDs from the js side
};


// =============================================================================
// ChartWidget
// =============================================================================

class ChartWidget : public hdps::gui::WebWidget
{
    Q_OBJECT
public:
    ChartWidget(ExampleViewJSPlugin* viewJSPlugin);

    ChartCommObject& getCommunicationObject() { return _comObject; };

private slots:
    /** Is invoked when the js side calls js_available of the hdps::gui::WebCommunicationObject (ChartCommObject) 
        js_available emits notifyJsBridgeIsAvailable, which is conencted to this slot in WebWidget.cpp*/
    void initWebPage() override;

private:
    ExampleViewJSPlugin*  _viewJSPlugin;    // Pointer to the main plugin class
    ChartCommObject       _comObject;       // Communication Object between Qt (cpp) and JavaScript
};
