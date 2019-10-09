#pragma once

#include "RawData.h"
#include "Set.h"
#include "CoreInterface.h"

#include <QImage>
#include <QString>
#include <QColor>

using namespace hdps::plugin;

// =============================================================================
// Raw Data
// =============================================================================

class ExampleDataPlugin : public RawData
{
public:
    ExampleDataPlugin() : RawData("Example Data") { }
    ~ExampleDataPlugin(void) override;
    
    void init() override;

    hdps::Set* createSet() const override;

    void setData(QImage image);
private:
    // Data store of the plugin, this is the core of your plugin
    std::vector<QColor> data;
};

// =============================================================================
// Data Set
// =============================================================================

class PixelSet : public hdps::Set
{
public:
    PixelSet(hdps::CoreInterface* core, QString dataName) : Set(core, dataName) { }
    ~PixelSet() override { }

    ExampleDataPlugin& getData() const
    {
        return dynamic_cast<ExampleDataPlugin&>(_core->requestData(getDataName()));
    }

    Set* copy() const override;

    std::vector<unsigned int> indices;
};


// =============================================================================
// Factory
// =============================================================================

class ExampleDataPluginFactory : public RawDataFactory
{
    Q_INTERFACES(hdps::plugin::RawDataFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.tudelft.ExampleDataPlugin"
                      FILE  "ExampleDataPlugin.json")
    
public:
    ExampleDataPluginFactory(void) {}
	~ExampleDataPluginFactory(void) override {}
    
    RawData* produce() override;
};
