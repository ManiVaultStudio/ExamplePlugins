#pragma once

#include <AnalysisPlugin.h>

#include <QPushButton>

#include "ExampleSettingsWidget.h"

using namespace hdps::plugin;

// =============================================================================
// Analysis
// =============================================================================

class ExampleAnalysisPlugin : public QObject, public AnalysisPlugin
{
    Q_OBJECT
    
public:
    ExampleAnalysisPlugin() : AnalysisPlugin("Example Analysis") { }
    ~ExampleAnalysisPlugin(void) override;
    
    void init() override;
    
    void dataAdded(const QString name) Q_DECL_OVERRIDE;
    void dataChanged(const QString name) Q_DECL_OVERRIDE;
    void dataRemoved(const QString name) Q_DECL_OVERRIDE;
    void selectionChanged(const QString dataName) Q_DECL_OVERRIDE;
    QStringList supportedDataKinds() Q_DECL_OVERRIDE;

    // Mandatory override so the core can add the settings widget to the application
    SettingsWidget* const getSettings() override;

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
};
