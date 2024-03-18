#pragma once

#include <TransformationPlugin.h>

#include <QMap>
#include <QString>

/** All plugin related classes are in the ManiVault plugin namespace */
using namespace mv::plugin;

/**
 * Example transformation plugin class
 *
 * This transformation plugin class provides skeleton code that shows how to develop 
 * an transformation plugin in ManiVault.
 * 
 * In contrast to analysis plugins, transformation do not create an output data set by default,
 * but operate on the input data set. In this example, we provide to transformation options,
 * either simply taking the absolute value of each data point or raising it to the power of 2.
 *
 * To see the plugin in action, please follow the steps below:
 *
 * 1. This plugin works on points datasets and is created by right-clicking a points
 * dataset in the data hierarchy viewer and choosing Transform >> Example transformation.
 * 2. Chose the transformation option
 * 
 */
class ExampleTransformationPlugin : public TransformationPlugin
{
Q_OBJECT

public:

    /** Define the transformation options */
    enum class Type {
        Abs,       /** abosulte value */
        Pow2       /** value squared */
    };

    static const QMap<Type, QString> types;

public:

    /**
     * Constructor
     * @param factory Pointer to the plugin factory
     */
    ExampleTransformationPlugin(const PluginFactory* factory);

    /** Destructor */
    ~ExampleTransformationPlugin() override = default;

    /** Initialization is called when the plugin is first instantiated. */
    void init() override {};

    /** Performs the data transformation */
    void transform() override;

    /**
     * Get transformation type
     * @return transformation type
     */
    Type getType() const;

    /**
     * Set transformation type
     * @param type transformation type
     */
    void setType(const Type& type);

    /**
     * Get string representation of transformation type enum
     * @param type transformation type
     * @return Type name
     */
    static QString getTypeName(const Type& type);

private:
    Type    _type;      /** Data conversion type */

};

/**
 * Example transform plugin factory class
 *
 * Note: Factory does not need to be altered (merely responsible for generating new plugins when requested)
 */
class ExampleTransformationPluginFactory : public TransformationPluginFactory
{
    Q_INTERFACES(mv::plugin::TransformationPluginFactory mv::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "studio.manivault.ExampleTransformationPlugin"
                      FILE  "ExampleTransformationPlugin.json")

public:

    /** Default constructor */
    ExampleTransformationPluginFactory() {}

    /** Destructor */
    ~ExampleTransformationPluginFactory() override {}

    /** Creates an instance of the example transform plugin */
    ExampleTransformationPlugin* produce() override;

    /** Returns the data types that are supported by the example transformation plugin */
    mv::DataTypes supportedDataTypes() const override;

    /** Enable right-click on data set to execute transformation */
    mv::gui::PluginTriggerActions getPluginTriggerActions(const mv::Datasets& datasets) const override;

};
