#pragma once

#include <ViewPlugin.h>

#include <Dataset.h>
#include <widgets/DropWidget.h>

#include <PointData/PointData.h>

#include <QWidget>

/** All plugin related classes are in the HDPS plugin namespace */
using namespace hdps::plugin;

/** Drop widget used in this plugin is located in the HDPS gui namespace */
using namespace hdps::gui;

/** Dataset reference used in this plugin is located in the HDPS util namespace */
using namespace hdps::util;

class QLabel;

/**
 * Example view plugin class
 *
 * This view plugin class provides skeleton code that shows how to develop
 * a view plugin in HDPS. It shows how to use the built-in drag and drop
 * behavior.
 *
 * To see the plugin in action, please follow the steps below:
 *
 * 1. Go to the visualization menu in HDPS
 * 2. Choose the Example view menu item, the view will be added to the layout
 *
 * @authors J. Thijssen & T. Kroes
 */
class ExampleViewPlugin : public ViewPlugin
{
    Q_OBJECT

public:

    /**
     * Constructor
     * @param factory Pointer to the plugin factory
     */
    ExampleViewPlugin(const PluginFactory* factory);

    /** Destructor */
    ~ExampleViewPlugin() override = default;
    
    /** This function is called by the core after the view plugin has been created */
    void init() override;

    /**
     * Invoked when a data event occurs
     * @param dataEvent Data event which occurred
     */
    void onDataEvent(hdps::DatasetEvent* dataEvent);

protected:
    DropWidget*             _dropWidget;                /** Widget for drag and drop behavior */
    hdps::Dataset<Points>   _points;                    /** Points smart pointer */
    QString                 _currentDatasetName;        /** Name of the current dataset */
    QLabel*                 _currentDatasetNameLabel;   /** Label that show the current dataset name */
};

/**
 * Example view plugin factory class
 *
 * Note: Factory does not need to be altered (merely responsible for generating new plugins when requested)
 */
class ExampleViewPluginFactory : public ViewPluginFactory
{
    Q_INTERFACES(hdps::plugin::ViewPluginFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.BioVault.ExampleViewPlugin"
                      FILE  "ExampleViewPlugin.json")

public:

    /** Default constructor */
    ExampleViewPluginFactory() {}

    /** Destructor */
    ~ExampleViewPluginFactory() override {}
    
    /** Creates an instance of the example view plugin */
    ViewPlugin* produce() override;

    /** Returns the data types that are supported by the example view plugin */
    hdps::DataTypes supportedDataTypes() const override;

    /**
     * Get plugin trigger actions given \p datasets
     * @param datasets Vector of input datasets
     * @return Vector of plugin trigger actions
     */
    PluginTriggerActions getPluginTriggerActions(const hdps::Datasets& datasets) const override;
};
