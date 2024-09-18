#pragma once

#include "actions/GroupAction.h"
#include "actions/TriggerAction.h"

using namespace mv::gui;

class SettingsAction : public GroupAction
{
public:

    SettingsAction(QObject* parent = nullptr);

public:
    TriggerAction& getStartAnalysisAction() { return _startAnalysisAction; }

public:
    TriggerAction   _startAnalysisAction;
};
