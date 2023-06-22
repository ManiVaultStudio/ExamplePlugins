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
    void qt_js_setDataInJS(QVariantList& data);
    void qt_js_setSelectionInJS(QVariantList& selection);

    // Signals Qt internal
    void passSelectionToCore(const std::vector<unsigned int>& selectionIDs);

public slots:
    // Invoked from JS side 
    void js_qt_passSelectionToQt(QVariantList data);

private:
    std::vector<unsigned int> _selectedIDsFromJS;

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
    ExampleViewJSPlugin*  _viewJSPlugin;    //
    ChartCommObject       _comObject;       //
};
