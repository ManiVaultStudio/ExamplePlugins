#pragma once

#include <ViewPlugin.h>

#include <QWidget>

/** All plugin related classes are in the ManiVault plugin namespace */
using namespace mv::plugin;

/** Drop widget used in this plugin is located in the ManiVault gui namespace */
using namespace mv::gui;

/** Dataset reference used in this plugin is located in the ManiVault util namespace */
using namespace mv::util;

class QLabel;

/**
 * Example view plugin class
 *
 * @authors T. Kroes
 */
class ExampleActionsPlugin : public ViewPlugin
{
    Q_OBJECT

public:

    /**
     * Constructor
     * @param factory Pointer to the plugin factory
     */
    ExampleActionsPlugin(const PluginFactory* factory);

    /** Destructor */
    ~ExampleActionsPlugin() override = default;
    
    /** This function is called by the core after the view plugin has been created */
    void init() override;

private:
};

/**
 * Example actions view plugin factory class
 *
 * Note: Factory does not need to be altered (merely responsible for generating new plugins when requested)
 */
class ExampleActionsPluginFactory : public ViewPluginFactory
{
    Q_INTERFACES(mv::plugin::ViewPluginFactory mv::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "studio.manivault.ExampleActionsPlugin"
                      FILE  "PluginInfo.json")

public:

    /** Default constructor */
    ExampleActionsPluginFactory();

    /** Creates an instance of the example view plugin */
    ViewPlugin* produce() override;

    /** Returns the data types that are supported by the example view plugin */
    mv::DataTypes supportedDataTypes() const override;
};
