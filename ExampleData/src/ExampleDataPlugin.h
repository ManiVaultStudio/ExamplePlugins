#pragma once

#include "RawData.h"
#include "Set.h"
#include "CoreInterface.h"

#include <QImage>
#include <QString>
#include <QColor>

using namespace hdps::plugin;

// =============================================================================
// Data Type
// =============================================================================

const hdps::DataType ExampleType = hdps::DataType(QString("Example"));

// =============================================================================
// Raw Data
// =============================================================================

class ExampleDataPlugin : public RawData
{
public:
    ExampleDataPlugin(const PluginFactory* factory) : RawData(factory, ExampleType) { }
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

    // Create a subset of the data from selected indices
    QString createSubset(const QString parentSetName, const bool& visible) const override
    {
        const hdps::DataSet& selection = getSelection();

        return _core->createSubsetFromSelection(selection, *this, "Subset");
    }

    DataSet* copy() const override;

    /** Get icon for the dataset */
    QIcon getIcon() const override;

    // Indices into the raw data, if this dataset is just a subset
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
    
    hdps::plugin::RawData* produce() override;
};
