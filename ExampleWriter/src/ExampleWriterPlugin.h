#pragma once

#include <WriterPlugin.h>

using namespace mv::plugin;
using namespace mv::gui;

// =============================================================================
// Writer
// =============================================================================

class ExampleWriterPlugin : public WriterPlugin
{
    Q_OBJECT
public:
    ExampleWriterPlugin(const PluginFactory* factory);
    ~ExampleWriterPlugin(void) override;
    
    void init() override;

    void writeData() Q_DECL_OVERRIDE;

private:
    QString _dataSetName;
};


// =============================================================================
// Factory
// =============================================================================

class ExampleWriterPluginFactory : public WriterPluginFactory
{
    Q_INTERFACES(mv::plugin::WriterPluginFactory mv::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "studio.manivault.ExampleWriterPlugin"
                      FILE  "PluginInfo.json")
    
public:
    ExampleWriterPluginFactory();

    /** Creates an instance of the example writer plugin */
    ExampleWriterPlugin* produce() override;

    /** Returns the data types that are supported by the example writer plugin */
    mv::DataTypes supportedDataTypes() const override;

    /** Enable right-click on data set to execute transformation */
    PluginTriggerActions getPluginTriggerActions(const mv::Datasets& datasets) const override;

};
