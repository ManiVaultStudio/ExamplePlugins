#pragma once

#include <LoaderPlugin.h>

using namespace hdps::plugin;

// =============================================================================
// Loader
// =============================================================================

class ExampleLoaderPlugin : public LoaderPlugin
{
    Q_OBJECT
public:
    ExampleLoaderPlugin(const PluginFactory* factory) : LoaderPlugin(factory) { }
    ~ExampleLoaderPlugin(void) override;
    
    void init() override;

    void loadData() Q_DECL_OVERRIDE;

private:
    unsigned int _numDimensions;

    QString _dataSetName;
};


// =============================================================================
// Factory
// =============================================================================

class ExampleLoaderPluginFactory : public LoaderPluginFactory
{
    Q_INTERFACES(hdps::plugin::LoaderPluginFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.tudelft.ExampleLoaderPlugin"
                      FILE  "ExampleLoaderPlugin.json")
    
public:
    ExampleLoaderPluginFactory(void) {}
    ~ExampleLoaderPluginFactory(void) override {}

    ExampleLoaderPlugin* produce() override;

    hdps::DataTypes supportedDataTypes() const override;
};
