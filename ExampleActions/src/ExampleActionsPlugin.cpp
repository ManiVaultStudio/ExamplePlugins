#include "ExampleActionsPlugin.h"

#include <CoreInterface.h>

#include <QDebug>

Q_PLUGIN_METADATA(IID "studio.manivault.ExampleActionsPlugin")

using namespace mv;

ExampleActionsPlugin::ExampleActionsPlugin(const PluginFactory* factory) :
    ViewPlugin(factory)
{
}

void ExampleActionsPlugin::init()
{
    // Create layout
    auto layout = new QVBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);

    // Apply the layout
    getWidget().setLayout(layout);
}

ViewPlugin* ExampleActionsPluginFactory::produce()
{
    return new ExampleActionsPlugin(this);
}
