#pragma once

#include <AnalysisPlugin.h>

#include "SettingsAction.h"

#include "graphics/Vector2f.h"

#include <QRandomGenerator>
#include <QtMath>

/** All plugin related classes are in the ManiVault plugin namespace */
using namespace mv::plugin;

/** Vector classes used in this plugin are in the ManiVault namespace */
using namespace mv;

/**
 * Example analysis plugin class
 *
 * This analysis plugin class provides skeleton code that shows how to develop 
 * an analysis plugin in ManiVault.
 * 
 * The plugin simulates the analysis of points, where point positions are randomly 
 * initialized. During simulation, the points are advanced along a random heading 
 * with a random velocity
 *
 * To see the plugin in action, please follow the steps below:
 *
 * 1. This plugin works on points datasets and is created by right-clicking a points
 * dataset in the data hierarchy viewer and choosing analyze >> Example analysis.
 * 2. Once the plugin is created, an output points dataset is added as a child of 
 * the input dataset (which was right-clicked)
 * 3. Drag the output points dataset to one of the viewers that are able to visualize 
 * points datasets (the stock scatter-plot plugin can be used for instance)
 * 4. Configure the analysis parameters
 * 5. Start the analysis by clicking on the start analysis push button
 * 
 * @authors J. Thijssen & T. Kroes
 */
class ExampleAnalysisPlugin : public AnalysisPlugin
{
Q_OBJECT

    /** Describes a data point (position + heading) */
    struct Point
    {
        /** Constructor */
        Point() :
            _positionX(),
            _positionY(),
            _headingDegrees(),
            _headingX(),
            _headingY(),
            _positionVelocity(),
            _headingVelocity()
        {
            initialize();
        }

        /** Perform random initialization of position and heading */
        void initialize() {

            // Establish random position
            _positionX = rng.bounded(1.0);
            _positionY = rng.bounded(1.0);
            
            // Establish random heading in degrees (radians)
            _headingDegrees = rng.bounded(qDegreesToRadians(360.0));

            // Set heading
            _headingX = qSin(_headingDegrees);
            _headingY = qCos(_headingDegrees);

            // Set random position/heading velocity
            _positionVelocity   = rng.bounded(maximumVelocity);
            _headingVelocity    = qDegreesToRadians(-1.0) + rng.bounded(qDegreesToRadians(1.0));
        }

        /** Move the point */
        void advancePoint() {

            // Advance the heading
            _headingDegrees += qDegreesToRadians(_headingVelocity);

            // Set heading
            _headingX = qSin(_headingDegrees);
            _headingY = qCos(_headingDegrees);

            // Advance the point
            _positionX += _positionVelocity * _headingX;
            _positionY += _positionVelocity * _headingY;
        }

        /** Members */
        float   _positionX;         /** X-position of the data point */
        float   _positionY;         /** Y-position of the data point */
        float   _headingDegrees;    /** Heading in degrees of the data point */
        float   _headingX;          /** X-heading of the data point */
        float   _headingY;          /** Y-heading of the data point */
        float   _positionVelocity;  /** Position velocity of the data point */
        float   _headingVelocity;   /** Heading velocity of the data point */

        /** Statics */
        static QRandomGenerator         rng;                    /** Random number generator for generating random points and headings */
        static std::vector<QString>     dimensionNames;         /** Names of the dimensions as the presented in the GUI */
        static std::uint32_t            numberOfDimensions;     /** Number of dimensions */
        static float                    maximumVelocity;        /** Maximum velocity */
    };

public:

    /**
     * Constructor
     * @param factory Pointer to the plugin factory
     */
    ExampleAnalysisPlugin(const PluginFactory* factory);

    /** Destructor */
    ~ExampleAnalysisPlugin() override = default;

    /**
     * This function is called by the core after the analysis plugin has been created
     *
     * Typical implementations of this function focus on the generation of output data
     * and responding to events which are sent by the core.
    */
    void init() override;

    /**
     * Invoked when a points data event occurs
     * @param dataEvent Data event which occurred
     */
    void onDataEvent(mv::DatasetEvent* dataEvent);

private:
    SettingsAction      _settingsAction;    /** The place where settings are stored (more info in SettingsAction.h) */
    QVector<Point>      _points;            /** The output points as generated by this plugin (contained by _output points dataset) */
    QVector<Vector2f>   _pointHeadings;     /** The start point headings */
    
};

/**
 * Example analysis plugin factory class
 *
 * Note: Factory does not need to be altered (merely responsible for generating new plugins when requested)
 */
class ExampleAnalysisPluginFactory : public AnalysisPluginFactory
{
    Q_INTERFACES(mv::plugin::AnalysisPluginFactory mv::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "studio.manivault.ExampleAnalysisPlugin"
                      FILE  "ExampleAnalysisPlugin.json")

public:

    /** Default constructor */
    ExampleAnalysisPluginFactory();

    /** Creates an instance of the example analysis plugin */
    AnalysisPlugin* produce() override;

    /** Returns the data types that are supported by the example analysis plugin */
    mv::DataTypes supportedDataTypes() const override;

    /** Enable right-click on data set to open analysis */
    PluginTriggerActions getPluginTriggerActions(const mv::Datasets& datasets) const override;
};
