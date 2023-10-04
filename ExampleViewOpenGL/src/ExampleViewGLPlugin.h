#pragma once

#include <ViewPlugin.h>
#include <Dataset.h>
#include <widgets/DropWidget.h>
#include <PointData/PointData.h>

#include "SettingsAction.h"

#include <QWidget>

/** All plugin related classes are in the HDPS plugin namespace */
using namespace hdps::plugin;

/** Drop widget used in this plugin is located in the HDPS gui namespace */
using namespace hdps::gui;

/** Dataset reference used in this plugin is located in the HDPS util namespace */
using namespace hdps::util;

class ExampleGLWidget;

/**
 * Example view plugin class
 *
 * This plugin showcases how a OpenGL-based visualization can be included in ManiVault.
 * We are implemented a bare-bone scatterplot using ManiVault's point renderer.
 * 
 * This project:
 *  - Sets up a QOpenGLWidget, which is used to render OpenGL graphics
 *  - Uses ManiVault's DropWidget to drag&drop data sets into the view
 * 
 * To keep this example concise this projects does not implement selections.
 *
 * @authors J. Thijssen & T. Kroes & A. Vieth
 */
class ExampleViewGLPlugin : public ViewPlugin
{
    Q_OBJECT

public:

    /**
     * Constructor
     * @param factory Pointer to the plugin factory
     */
    ExampleViewGLPlugin(const PluginFactory* factory);

    /** Destructor */
    ~ExampleViewGLPlugin() override = default;
    
    /** This function is called by the core after the view plugin has been created */
    void init() override;

    /** Store a private reference to the data set that should be displayed */
    void loadData(const hdps::Datasets& datasets) override;

    /** Retrieves data to be shown and updates the OpenGL plot */
    void updatePlot();

private:
    /** We create and publish some data in order to provide an self-contained example project */
    void createData();

    QString getCurrentDataSetID() const;

protected:
    DropWidget*                 _dropWidget;            /** Widget for drag and drop behavior */
    ExampleGLWidget*            _exampleGLWidget;       /** The OpenGL widget */
    SettingsAction              _settingsAction;        /** Settings action */
    hdps::Dataset<Points>       _currentDataSet;        /** Points smart pointer */
    std::vector<unsigned int>   _currentDimensions;     /** Stores which dimensions of the current data are shown */
};

/**
 * Example view plugin factory class
 *
 * Note: Factory does not need to be altered (merely responsible for generating new plugins when requested)
 */
class ExampleViewGLPluginFactory : public ViewPluginFactory
{
    Q_INTERFACES(hdps::plugin::ViewPluginFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.BioVault.ExampleViewGLPlugin"
                      FILE  "ExampleViewGLPlugin.json")

public:

    /** Default constructor */
    ExampleViewGLPluginFactory() {}

    /** Destructor */
    ~ExampleViewGLPluginFactory() override {}
    
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
