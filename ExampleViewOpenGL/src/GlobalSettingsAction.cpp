#include "GlobalSettingsAction.h"

#include <QHBoxLayout>

using namespace mv;
using namespace mv::gui;

GlobalSettingsAction::GlobalSettingsAction(QObject* parent, const plugin::PluginFactory* pluginFactory) :
    PluginGlobalSettingsGroupAction(parent, pluginFactory),
    _defaultPointSizeAction(this, "Default point Size", 1, 50, 10),
    _defaultPointOpacityAction(this, "Default point opacity", 0.f, 1.f, 0.5f)
{
    _defaultPointSizeAction.setToolTip("Default size of individual points");
    _defaultPointOpacityAction.setToolTip("Default opacity of individual points");

    // The add action automatically assigns a settings prefix to _pointSizeAction so there is no need to do this manually
    addAction(&_defaultPointSizeAction);
    addAction(&_defaultPointOpacityAction);
}
