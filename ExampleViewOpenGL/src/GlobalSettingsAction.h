#pragma once

#include <PluginGlobalSettingsGroupAction.h>

#include <actions/DecimalAction.h>

namespace mv {
    namespace plugin {
        class PluginFactory;
    }
}

/**
 * Global settings action class
 *
 * Action class for configuring global settings
 * 
 * This group action (once assigned to the plugin factory, see ExampleViewGLPluginFactory::initialize()) is 
 * added to the global settings panel, accessible through the file > settings menu.
 * 
 */
class GlobalSettingsAction : public mv::gui::PluginGlobalSettingsGroupAction
{
public:

    /**
     * Construct with pointer to \p parent object and \p pluginFactory
     * @param parent Pointer to parent object
     * @param pluginFactory Pointer to plugin factory
     */
    Q_INVOKABLE GlobalSettingsAction(QObject* parent, const mv::plugin::PluginFactory* pluginFactory);

public: // Action getters

    mv::gui::DecimalAction& getDefaultPointSizeAction() { return _defaultPointSizeAction; }

private:
    mv::gui::DecimalAction   _defaultPointSizeAction;   /** Default point size action */
};
