#pragma once

#include <AnalysisPlugin.h>

#include <QPushButton>

#include "ExampleSettingsWidget.h"

using namespace hdps::plugin;

// =============================================================================
// Analysis
// =============================================================================

class ExampleAnalysisPlugin : public QObject, public hdps::plugin::AnalysisPlugin
{
    Q_OBJECT
    
public:
    ExampleAnalysisPlugin(const PluginFactory* factory) : AnalysisPlugin(factory) { }
    ~ExampleAnalysisPlugin(void) override;
    
    void init() override;
    
    void onDataEvent(hdps::DataEvent* dataEvent);

public slots:
    void dataSetPicked(const QString& name);

private:
    std::unique_ptr<ExampleSettingsWidget> _settings;
};


// =============================================================================
// Factory
// =============================================================================

class ExampleAnalysisPluginFactory : public AnalysisPluginFactory
{
    Q_INTERFACES(hdps::plugin::AnalysisPluginFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.tudelft.ExampleAnalysisPlugin"
                      FILE  "ExampleAnalysisPlugin.json")
    
public:
    ExampleAnalysisPluginFactory(void) {}
    ~ExampleAnalysisPluginFactory(void) override {}
    
    AnalysisPlugin* produce() override;

    hdps::DataTypes supportedDataTypes() const override;
};
