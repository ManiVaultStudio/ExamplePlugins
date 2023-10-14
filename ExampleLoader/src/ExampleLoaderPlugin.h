#pragma once

#include <LoaderPlugin.h>

using namespace mv::plugin;

// =============================================================================
// Loader
// =============================================================================

class ExampleLoaderPlugin : public LoaderPlugin
{
    Q_OBJECT
public:
    ExampleLoaderPlugin(const PluginFactory* factory);
    ~ExampleLoaderPlugin(void) override;
    
    void init() override;

    void loadData() Q_DECL_OVERRIDE;

private:
    QString _dataSetName;
};


// =============================================================================
// Factory
// =============================================================================

class ExampleLoaderPluginFactory : public LoaderPluginFactory
{
    Q_INTERFACES(mv::plugin::LoaderPluginFactory mv::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.tudelft.ExampleLoaderPlugin"
                      FILE  "ExampleLoaderPlugin.json")
    
public:
    ExampleLoaderPluginFactory(void) {}
    ~ExampleLoaderPluginFactory(void) override {}

    ExampleLoaderPlugin* produce() override;

    mv::DataTypes supportedDataTypes() const override;
};
