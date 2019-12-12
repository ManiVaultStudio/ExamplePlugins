#pragma once

#include "RawData.h"
#include "Set.h"
#include "CoreInterface.h"

#include <QImage>
#include <QString>
#include <QColor>

// =============================================================================
// Data Type
// =============================================================================

const hdps::DataType ExampleType = hdps::DataType(QString("Example"));

// =============================================================================
// Raw Data
// =============================================================================

class ExampleDataPlugin : public hdps::RawData
{
public:
    ExampleDataPlugin() : hdps::RawData("Example Data", ExampleType) { }
    ~ExampleDataPlugin(void) override;
    
    void init() override;

    hdps::DataSet* createDataSet() const override;

    void setData(QImage image);
private:
    // Data store of the plugin, this is the core of your plugin
    std::vector<QColor> data;
};

// =============================================================================
// Data Set
// =============================================================================

class PixelSet : public hdps::DataSet
{
public:
    PixelSet(hdps::CoreInterface* core, QString dataName) : DataSet(core, dataName) { }
    ~PixelSet() override { }

    void createSubset() const override
    {
        const hdps::DataSet& selection = _core->requestSelection(getDataName());

        _core->createSubsetFromSelection(selection, getDataName(), "Subset");
    }

    DataSet* copy() const override;

    std::vector<unsigned int> indices;
};


// =============================================================================
// Factory
// =============================================================================

class ExampleDataPluginFactory : public hdps::plugin::RawDataFactory
{
    Q_INTERFACES(hdps::plugin::RawDataFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.tudelft.ExampleDataPlugin"
                      FILE  "ExampleDataPlugin.json")
    
public:
    ExampleDataPluginFactory(void) {}
    ~ExampleDataPluginFactory(void) override {}
    
    hdps::RawData* produce() override;
};
