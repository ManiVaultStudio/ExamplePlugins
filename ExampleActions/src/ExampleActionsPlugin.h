#pragma once

#include <ViewPlugin.h>

#include <actions/TreeAction.h>
#include <actions/GroupsAction.h>

#include "ExampleActionsTreeModel.h"
#include "ExampleActionsFilterModel.h"

#include <QWidget>
#include <QListView>

/** All plugin related classes are in the ManiVault plugin namespace */
using namespace mv::plugin;

/** Drop widget used in this plugin is located in the ManiVault gui namespace */
using namespace mv::gui;

/** Dataset reference used in this plugin is located in the ManiVault util namespace */
using namespace mv::util;

/**
 * Example actions view plugin class
 *
 * This view plugin class provides skeleton code that shows how to use GUI building
 * blocks (aka actions) to build interfaces.
 *
 * To see the plugin in action, please follow the steps below:
 *
 * 1. Go to the visualization menu in ManiVault
 * 2. Choose the Example actions menu item, the view will be added to the layout
 *
 * @author T. Kroes
 */
class ExampleActionsPlugin : public ViewPlugin
{
    Q_OBJECT

public:

    /**
     * Construct with pointer to plugin \p factory
     * @param factory Pointer to the plugin factory
     */
    ExampleActionsPlugin(const PluginFactory* factory);

    /** Destructor */
    ~ExampleActionsPlugin() override = default;
    
    /** This function is called by the core after the view plugin has been created */
    void init() override;

protected:
    ExampleActionsTreeModel     _exampleActionsTreeModel;       /** Standard item model which stores example actions grouped by category */
    ExampleActionsFilterModel   _exampleActionsFilterModel;     /** Sort/filter proxy model for ExampleActionsPlugin#_exampleActionsTreeModel */
    mv::gui::TreeAction         _examplesTreeAction;            /** For displaying the ExampleActionsPlugin#_exampleActionsFilterModel */
    mv::gui::GroupsAction       _examplesGroupsAction;          /** For displaying the ExampleActionsPlugin#_exampleActionsListModel */

    friend class ExampleActionStyledItemDelegate;
};

/**
 * Example actions plugin factory class
 *
 * Note: Factory does not need to be altered (merely responsible for generating new plugins when requested)
 */
class ExampleActionsPluginFactory : public ViewPluginFactory
{
    Q_INTERFACES(mv::plugin::ViewPluginFactory mv::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "studio.manivault.ExampleActionsPlugin"
                      FILE  "ExampleActionsPlugin.json")

public:

    /** Default constructor */
    ExampleActionsPluginFactory() {}

    /** Destructor */
    ~ExampleActionsPluginFactory() override {}
    
    /** Creates an instance of the example view plugin */
    ViewPlugin* produce() override;
};
