#include "ExampleAnalysisPlugin.h"

#include "PointData.h"

#include <QtCore>
#include <QtDebug>

Q_PLUGIN_METADATA(IID "nl.tudelft.ExampleAnalysisPlugin")

using namespace hdps;

// =============================================================================
// Analysis
// =============================================================================

ExampleAnalysisPlugin::~ExampleAnalysisPlugin(void)
{
    
}

void ExampleAnalysisPlugin::init()
{
    _settings = std::make_unique<ExampleSettingsWidget>();

    // Connect the dataSetPicked signal from the settings widget to a function which handles it in the plugin
    connect(_settings.get(), &ExampleSettingsWidget::dataSetPicked, this, &ExampleAnalysisPlugin::dataSetPicked);
}

/**
 * Callback which gets triggered when a dataset is added.
 * The name of the dataset which was added is given.
 * The added dataset can be retrieved through _core->requestSet(name);
 * and casting it to the appropriate high-level set.
 */
void ExampleAnalysisPlugin::dataAdded(const QString name)
{
    _settings->dataOptions.addItem(name);
}

/**
 * Callback which gets triggered when a dataset changes.
 * The name of the dataset which was changed is given.
 * The changed dataset can be retrieved through _core->requestSet(name);
 * and casting it to the appropriate high-level set.
 */
void ExampleAnalysisPlugin::dataChanged(const QString name)
{
    const IndexSet& changedSet = _core->requestSet<IndexSet>(name);
}

/**
 * Callback which gets triggered when a dataset gets removed.
 * The name of the dataset which was removed is given.
 */
void ExampleAnalysisPlugin::dataRemoved(const QString name)
{
    const IndexSet& removedSet = _core->requestSet<IndexSet>(name);
}

/**
 * Callback which gets triggered when any plugin calls notifySelectionChanged().
 * The name of the data whose selection was changed is given and the new selection
 * can be retrieved through _core->requestSelection(dataName);
 */
void ExampleAnalysisPlugin::selectionChanged(const QString dataName)
{
    const hdps::Set& selectionSet = _core->requestSelection(dataName);
}

QStringList ExampleAnalysisPlugin::supportedDataKinds()
{
    QStringList supportedKinds;
    supportedKinds << "Points";
    return supportedKinds;
}

// Mandatory function so the core can add the settings widget to the application
SettingsWidget* const ExampleAnalysisPlugin::getSettings()
{
    return _settings.get();
}

/**
 * Gets triggered when you pick a dataset in the settings widget
 */
void ExampleAnalysisPlugin::dataSetPicked(const QString& name)
{
    // Request the dataset from the core
    IndexSet& set = (IndexSet&)_core->requestSet(name);
    // Request the raw data that belongs to the dataset
    PointData& rawData = set.getData<PointData>();
}

// =============================================================================
// Factory DOES NOT NEED TO BE ALTERED
// Merely responsible for generating new plugins when requested
// =============================================================================

AnalysisPlugin* ExampleAnalysisPluginFactory::produce()
{
    return new ExampleAnalysisPlugin();
}
