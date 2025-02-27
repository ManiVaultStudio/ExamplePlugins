#pragma once

#include "ExampleDataPlugin_export.h"

#include "RawData.h"
#include "Set.h"

#include <QImage>
#include <QString>
#include <QColor>

using namespace mv::plugin;

const mv::DataType ExampleType = mv::DataType(QString("Example"));

// =============================================================================
// Raw Data
// =============================================================================

class EXAMPLEDATAPLUGIN_EXPORT ExampleDataPlugin : public RawData
{
public:
    ExampleDataPlugin(const PluginFactory* factory) : RawData(factory, ExampleType) { }
    ~ExampleDataPlugin(void) override;
    
    void init() override;

    /**
     * Create dataset for raw data
     * @param guid Globally unique dataset identifier (use only for deserialization)
     * @return Smart pointer to dataset
     */
    mv::Dataset<mv::DatasetImpl> createDataSet(const QString& guid = "") const override;

    std::vector<QColor>& getData();

    void setData(QImage image);

private:
    std::vector<QColor> _data;       /** Data store of the plugin, this is the core of your plugin */
};

// =============================================================================
// Data Set
// =============================================================================

class EXAMPLEDATAPLUGIN_EXPORT PixelSet : public mv::DatasetImpl
{
public:
    PixelSet(QString dataName, const QString& guid = "");
    ~PixelSet() override;

    /**
     * Create subset and specify where the subset will be placed in the data hierarchy
     * @param guiName Name of the subset in the GUI
     * @param parentDataSet Smart pointer to parent dataset in the data hierarchy (default is below the set)
     * @param visible Whether the subset will be visible in the UI
     * @return Smart pointer to the created subset
     */
    mv::Dataset<mv::DatasetImpl> createSubsetFromSelection(const QString& guiName, const mv::Dataset<mv::DatasetImpl>& parentDataSet = mv::Dataset<mv::DatasetImpl>(), const bool& visible = true) const override;

    /** Mandatory override for copying of data sets */
    mv::Dataset<mv::DatasetImpl> copy() const override;

public: // Selection

    /**
     * Get selection indices
     * @return Selection indices
     */
    std::vector<std::uint32_t>& getSelectionIndices() override;

    /**
     * Select by indices
     * @param indices Selection indices
     */
    void setSelectionIndices(const std::vector<std::uint32_t>& indices) override;

    /** Determines whether items can be selected */
    bool canSelect() const override;

    /** Determines whether all items can be selected */
    bool canSelectAll() const override;

    /** Determines whether there are any items which can be deselected */
    bool canSelectNone() const override;

    /** Determines whether the item selection can be inverted (more than one) */
    bool canSelectInvert() const override;

    /** Select all items */
    void selectAll() override;

    /** Deselect all items */
    void selectNone() override;

    /** Invert item selection */
    void selectInvert() override;

protected:
    std::vector<unsigned int>   _indices;      /** Indices into the raw data, if this dataset is just a subset */
};

// =============================================================================
// Factory
// =============================================================================

class ExampleDataPluginFactory : public RawDataFactory
{
    Q_INTERFACES(mv::plugin::RawDataFactory mv::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "studio.manivault.ExampleDataPlugin"
                      FILE  "ExampleDataPlugin.json")
    
public:
    ExampleDataPluginFactory();

    mv::plugin::RawData* produce() override;
};
