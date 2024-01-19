#include "GlobalSettingsAction.h"

#include <QHBoxLayout>

using namespace mv;
using namespace mv::gui;

GlobalSettingsAction::GlobalSettingsAction(QObject* parent, const plugin::PluginFactory* pluginFactory) :
    PluginGlobalSettingsGroupAction(parent, pluginFactory),
    _defaultPointSizeAction(this, "Default point Size", 1, 50, 10)
{
    _defaultPointSizeAction.setToolTip("Default size of individual points");

    // The add action automatically assigns a settings prefix to _pointSizeAction so there is no need to do this manually
    addAction(&_defaultPointSizeAction);
}
