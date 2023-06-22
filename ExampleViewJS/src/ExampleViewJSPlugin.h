#pragma once

#include <ViewPlugin.h>

#include <Dataset.h>
#include <PointData/PointData.h>
#include <widgets/DropWidget.h>

#include <QWidget>

/** All plugin related classes are in the HDPS plugin namespace */
using namespace hdps::plugin;

/** Drop widget used in this plugin is located in the HDPS gui namespace */
using namespace hdps::gui;

/** Dataset reference used in this plugin is located in the HDPS util namespace */
using namespace hdps::util;

class ChartWidget;

/**
 * Example view JS plugin class
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
    void loadData(const hdps::Datasets& datasets) override;

signals:
    void dataSetChanged();

public slots:
    /** Converts ManiVault's point data to a json-like data structure that Qt can pass to the JS code */
    void convertDataAndUpdateChart();

private:
    /** Published selections received from the JS side to ManiVault's core */
    void publishSelection(const std::vector<unsigned int>& selectedIDs);

    QString getCurrentDataSetGuid() const;

    /** Dummy function to create data, instead of loading it from the core */
    void createData();

private:
    ChartWidget*            _chartWidget;       // WebWidget that sets up the HTML page
    DropWidget*             _dropWidget;        // Widget for drag and drop behavior
    hdps::Dataset<Points>   _currentDataSet;    // Reference to currently shown data set
};

/**
 * Example view plugin factory class
 *
 * Note: Factory does not need to be altered (merely responsible for generating new plugins when requested)
 */
class ExampleViewJSPluginFactory : public ViewPluginFactory
{
    Q_INTERFACES(hdps::plugin::ViewPluginFactory hdps::plugin::PluginFactory)
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
    hdps::DataTypes supportedDataTypes() const override;

    /**
     * Get plugin trigger actions given \p datasets
     * @param datasets Vector of input datasets
     * @return Vector of plugin trigger actions
     */
    PluginTriggerActions getPluginTriggerActions(const hdps::Datasets& datasets) const override;
};
