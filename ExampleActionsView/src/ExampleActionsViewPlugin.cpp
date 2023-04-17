#include "ExampleActionsViewPlugin.h"

#include <event/Event.h>

#include <QDebug>
#include <QMimeData>

Q_PLUGIN_METADATA(IID "NL.ManiVault.ExampleActionsViewPlugin")

using namespace hdps;

ExampleActionsViewPlugin::ExampleActionsViewPlugin(const PluginFactory* factory) :
    ViewPlugin(factory)
{
}

void ExampleActionsViewPlugin::init()
{
    auto layout = new QVBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);
}

ViewPlugin* ExampleActionsViewPluginFactory::produce()
{
    return new ExampleActionsViewPlugin(this);
}

hdps::DataTypes ExampleActionsViewPluginFactory::supportedDataTypes() const
{
    return DataTypes();
}
