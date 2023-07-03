#pragma once

#include <WriterPlugin.h>

using namespace hdps::plugin;
using namespace hdps::gui;

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
    Q_INTERFACES(hdps::plugin::WriterPluginFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.tudelft.ExampleWriterPlugin"
                      FILE  "ExampleWriterPlugin.json")
    
public:
    ExampleWriterPluginFactory(void) {}
    ~ExampleWriterPluginFactory(void) override {}

    /** Creates an instance of the example writer plugin */
    ExampleWriterPlugin* produce() override;

    /** Returns the data types that are supported by the example writer plugin */
    hdps::DataTypes supportedDataTypes() const override;

    /** Enable right-click on data set to execute transformation */
    PluginTriggerActions getPluginTriggerActions(const hdps::Datasets& datasets) const override;

};
