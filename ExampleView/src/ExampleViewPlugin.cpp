#include "ExampleViewPlugin.h"

#include "PointData.h"

#include <QtCore>
#include <QtDebug>

Q_PLUGIN_METADATA(IID "nl.tudelft.ExampleViewPlugin")

using namespace hdps;

// =============================================================================
// View
// =============================================================================

ExampleViewPlugin::~ExampleViewPlugin(void)
{
    
}

void ExampleViewPlugin::init()
{
    // Widgets can be freely added to the view plugin via the addWidget() function.
    ExampleWidget* exampleWidget = new ExampleWidget();
    addWidget(exampleWidget);
}

/**
 * Callback which gets triggered when a dataset is added.
 * The name of the dataset which was added is given.
 * The added dataset can be retrieved through _core->requestSet(name);
 * and casting it to the appropriate high-level set.
 */
void ExampleViewPlugin::dataAdded(const QString name)
{
    const IndexSet& addedSet = _core->requestSet<IndexSet>(name);
}

/**
 * Callback which gets triggered when a dataset changes.
 * The name of the dataset which was changed is given.
 * The changed dataset can be retrieved through _core->requestSet(name);
 * and casting it to the appropriate high-level set.
 */
void ExampleViewPlugin::dataChanged(const QString name)
{
    const IndexSet& changedSet = _core->requestSet<IndexSet>(name);
}

/**
 * Callback which gets triggered when a dataset gets removed.
 * The name of the dataset which was removed is given.
 */
void ExampleViewPlugin::dataRemoved(const QString name)
{
    const IndexSet& removedSet = dynamic_cast<const IndexSet&>(_core->requestSet(name));
}

/**
 * Callback which gets triggered when any plugin calls notifySelectionChanged().
 * The name of the data whose selection was changed is given and the new selection
 * can be retrieved through _core->requestSelection(dataName);
 */
void ExampleViewPlugin::selectionChanged(const QString dataName)
{
    const hdps::Set& selectionSet = _core->requestSelection(dataName);
}

QStringList ExampleViewPlugin::supportedDataKinds()
{
    QStringList supportedKinds;
    supportedKinds << "Points";
    return supportedKinds;
}

// =============================================================================
// Factory DOES NOT NEED TO BE ALTERED
// Merely responsible for generating new plugins when requested
// =============================================================================

ViewPlugin* ExampleViewPluginFactory::produce()
{
    return new ExampleViewPlugin();
}
