#pragma once

#include <ViewPlugin.h>

#include <Dataset.h>
#include <PointData/PointData.h>
#include <widgets/DropWidget.h>

#include <QWidget>

/** All plugin related classes are in the ManiVault plugin namespace */
using namespace mv::plugin;

/** Drop widget used in this plugin is located in the ManiVault gui namespace */
using namespace mv::gui;

/** Dataset reference used in this plugin is located in the ManiVault util namespace */
using namespace mv::util;

class ChartWidget;

/**
 * Example view JS plugin class
 * 
 * This plugin showcases how a JavaScript-based visualization can be included in ManiVault 
 * Here, we use a D3 library, but other libraries like Vega-Lite follow the same pattern
 * 
 * This project:
 *  - Sets up a WebWidget, which displays an HTML webpage
 *  - Connects selections made a D3 plot with ManiVault
 * 
 * This projects does not implement selections from ManiVault to the D3 plot,
 * but such implementation follows the same form as the data-values communication
 * between cpp and JavaScript that is used here.
 *
 * @authors J. Thijssen & T. Kroes & A. Vieth
 */
class ExampleViewJSPlugin : public ViewPlugin
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param factory Pointer to the plugin factory
     */
    ExampleViewJSPlugin(const PluginFactory* factory);

    /** Destructor */
    ~ExampleViewJSPlugin() override = default;
    
    /** This function is called by the core after the view plugin has been created */
    void init() override;

    /** Store a private reference to the data set that should be displayed */
    void loadData(const mv::Datasets& datasets) override;

public slots:
    /** Converts ManiVault's point data to a json-like data structure that Qt can pass to the JS code */
    void convertDataAndUpdateChart();

private:
    /** Published selections received from the JS side to ManiVault's core */
    void publishSelection(const std::vector<unsigned int>& selectedIDs);

    QString getCurrentDataSetID() const;

    /** We create and publish some data in order to provide an self-contained example project */
    void createData();

private:
    ChartWidget*            _chartWidget;       // WebWidget that sets up the HTML page
    DropWidget*             _dropWidget;        // Widget for drag and drop behavior
    mv::Dataset<Points>   _currentDataSet;    // Reference to currently shown data set
};

/**
 * Example view plugin factory class
 *
 * Note: Factory does not need to be altered (merely responsible for generating new plugins when requested)
 */
class ExampleViewJSPluginFactory : public ViewPluginFactory
{
    Q_INTERFACES(mv::plugin::ViewPluginFactory mv::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.BioVault.ExampleViewJSPlugin"
                      FILE  "ExampleViewJSPlugin.json")

public:

    /** Default constructor */
    ExampleViewJSPluginFactory() {}

    /** Destructor */
    ~ExampleViewJSPluginFactory() override {}
    
    /** Get plugin icon */
    QIcon getIcon(const QColor& color = Qt::black) const override;

    /** Creates an instance of the example view plugin */
    ViewPlugin* produce() override;

    /** Returns the data types that are supported by the example view plugin */
    mv::DataTypes supportedDataTypes() const override;

    /**
     * Get plugin trigger actions given \p datasets
     * @param datasets Vector of input datasets
     * @return Vector of plugin trigger actions
     */
    PluginTriggerActions getPluginTriggerActions(const mv::Datasets& datasets) const override;
};
