#pragma once

#include <AnalysisPlugin.h>

#include "SettingsAction.h"

using namespace mv::plugin;
using namespace mv;

/**
 * Example dependencies plugin class
 *
 * 
 * @authors 
 */
class ExampleDependenciesPlugin : public AnalysisPlugin
{
    Q_OBJECT

public:

    ExampleDependenciesPlugin(const PluginFactory* factory);
    ~ExampleDependenciesPlugin() override = default;

    void init() override;

private:
    void compute();

private:
    SettingsAction      _settingsAction;    /** The place where settings are stored (more info in SettingsAction.h) */
    
};

/**
 * Example analysis plugin factory class
 *
 * Note: Factory does not need to be altered (merely responsible for generating new plugins when requested)
 */
class ExampleDependenciesPluginFactory : public AnalysisPluginFactory
{
    Q_INTERFACES(mv::plugin::AnalysisPluginFactory mv::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "studio.manivault.ExampleDependenciesPlugin"
                      FILE  "ExampleDependenciesPlugin.json")

public:

    /** Default constructor */
    ExampleDependenciesPluginFactory() {}

    /** Destructor */
    ~ExampleDependenciesPluginFactory() override {}

    /** Creates an instance of the example analysis plugin */
    AnalysisPlugin* produce() override;

    /** Returns the data types that are supported by the example analysis plugin */
    mv::DataTypes supportedDataTypes() const override;

    /** Enable right-click on data set to open analysis */
    PluginTriggerActions getPluginTriggerActions(const mv::Datasets& datasets) const override;
};
