#include "SettingsAction.h"

SettingsAction::SettingsAction(QObject* parent) :
    GroupAction(parent, "SettingsAction", true),
    _startAnalysisAction(this, "Start analysis")
{
    setText("Example Settings");

    _startAnalysisAction.setToolTip("Start the analysis");

    addAction(&_startAnalysisAction);
}
