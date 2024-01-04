#pragma once

#include "actions/Actions.h"

/** All GUI related classes are in the ManiVault Graphical User Interface namespace */
using namespace mv::gui;

/**
 * ExampleSettingsAction class
 * 
 * Class that houses settings for the example analysis plugin
 *
 * This settings class is derived from the group action class. A group action
 * is a special type of action; when injected into a dataset its user interface
 * becomes available in the data properties widget. The group action list the child
 * actions in a form-like fashion. The order in which they appear corresponds with 
 * the order of declaration.
 *
 * Note: we strongly encourage you to use ManiVault core actions to build the user
 * interface. Actions separate the data and business logic from the user interface.
 * We have standard actions for editing of strings, decimals, integrals, options,
 * color and color maps. With these components, there is no need to write to create 
 * the user interface yourself. The actions will take care of this. For more 
 * information regarding actions, please visit actions/Actions.h
 */
class SettingsAction : public GroupAction
{
public:

    /**
     * Constructor
     * @param parent Pointer to parent object
     */
    SettingsAction(QObject* parent = nullptr);

public: // Action getters

    StringAction& getCurrentIterationAction() { return _currentIterationAction; }
    DecimalAction& getMaxVelocityAction() { return _maxVelocityAction; }
    IntegralAction& getNumberOfIterationsAction() { return _numberOfIterationsAction; }
    TriggerAction& getStartAnalysisAction() { return _startAnalysisAction; }

public:
    StringAction    _currentIterationAction;        /** Current iteration string action from the mv::gui namespace */
    DecimalAction   _maxVelocityAction;             /** Maximum velocity decimal action from the mv::gui namespace */
    IntegralAction  _numberOfIterationsAction;      /** Number of iterations action from the mv::gui namespace */
    TriggerAction   _startAnalysisAction;           /** Start loop  trigger action from the mv::gui namespace */
};
