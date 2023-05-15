#pragma once

#include <ViewPlugin.h>

#include <actions/WidgetAction.h>

#include <QWidget>

using namespace hdps::plugin;
using namespace hdps::gui;
using namespace hdps::util;

class QLabel;

/**
 * Example actions view plugin class
 *
 * This view plugin shows the standard actions which are shipped by ManiVault in a list
 *
 * @authors T. Kroes
 */
class ExampleActionsViewPlugin : public ViewPlugin
{
    Q_OBJECT

public:

    /**
     * Constructor
     * @param factory Pointer to the plugin factory
     */
    ExampleActionsViewPlugin(const PluginFactory* factory);

    /** Destructor */
    ~ExampleActionsViewPlugin() override = default;
    
    /** This function is called by the core after the view plugin has been created */
    void init() override;

public: // Serialization

    /**
     * Load widget action from variant map
     * @param Variant map representation of the widget action
     */
    void fromVariantMap(const QVariantMap& variantMap) override;

    /**
     * Save widget action to variant map
     * @return Variant map representation of the widget action
     */
    QVariantMap toVariantMap() const override;

protected:
    hdps::gui::WidgetActions  _actions;
};

/**
 * Example view plugin factory class
 *
 * Note: Factory does not need to be altered (merely responsible for generating new plugins when requested)
 */
class ExampleActionsViewPluginFactory : public ViewPluginFactory
{
    Q_INTERFACES(hdps::plugin::ViewPluginFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "NL.ManiVault.ExampleActionsViewPlugin"
                      FILE  "ExampleActionsViewPlugin.json")

public:

    /** Default constructor */
    ExampleActionsViewPluginFactory() {}

    /** Destructor */
    ~ExampleActionsViewPluginFactory() override {}
    
    /** Creates an instance of the example view plugin */
    ViewPlugin* produce() override;

    /** Returns the data types that are supported by the example view plugin */
    hdps::DataTypes supportedDataTypes() const override;
};
