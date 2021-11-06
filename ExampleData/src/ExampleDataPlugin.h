#pragma once

#include "RawData.h"
#include "Set.h"
#include "CoreInterface.h"

#include <QImage>
#include <QString>
#include <QColor>

using namespace hdps::plugin;

const hdps::DataType ExampleType = hdps::DataType(QString("Example"));

class ExampleDataPlugin : public RawData
{
public:
    ExampleDataPlugin(const PluginFactory* factory) : RawData(factory, ExampleType) { }
    ~ExampleDataPlugin(void) override;
    
    void init() override;

    hdps::DataSet* createDataSet() const override;

    void setData(QImage image);

private:
    std::vector<QColor> data;       /** Data store of the plugin, this is the core of your plugin */
};

// =============================================================================
// Data Set
// =============================================================================

class PixelSet : public hdps::DataSet
{
public:
    PixelSet(hdps::CoreInterface* core, QString dataName);
    ~PixelSet() override;

    /**
     * Create subset
     * @param subsetName Name of the subset
     * @param parentSetName Name of the parent dataset
     * @param visible Whether the subset will be visible in the UI
     */
    QString createSubset(const QString subsetName = "subset", const QString parentSetName = "", const bool& visible = true) const override
    {
        const hdps::DataSet& selection = getSelection();

        return _core->createSubsetFromSelection(selection, *this, subsetName);
    }

    /** Mandatory override for copying of data sets */
    DataSet* copy() const override;

    /** Get icon for the dataset */
    QIcon getIcon() const override;

protected:
    std::vector<unsigned int>   _indices;      /** Indices into the raw data, if this dataset is just a subset */
};

class ExampleDataPluginFactory : public RawDataFactory
{
    Q_INTERFACES(hdps::plugin::RawDataFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.BioVault.ExampleDataPlugin"
                      FILE  "ExampleDataPlugin.json")
    
public:
    ExampleDataPluginFactory() {}
    ~ExampleDataPluginFactory() override {}

    /** Returns the plugin icon */
    QIcon getIcon() const override;

    hdps::plugin::RawData* produce() override;
};
