#include "SettingsAction.h"

SettingsAction::SettingsAction(QObject* parent) :
    GroupAction(parent, "SettingsAction", true),
    _currentIterationAction(this, "Value"),
    _maxVelocityAction(this, "Maximum velocity", 0.001f, 0.1f, 0.02f, 0.02f),
    _numberOfIterationsAction(this, "Number of iterations", 0, 1000, 100),
    _startAnalysisAction(this, "Start analysis")
{
    setText("Example Settings");

    _currentIterationAction.setEnabled(false);

    _currentIterationAction.setToolTip("Current iteration index");
    _maxVelocityAction.setToolTip("The speed at which the points move");
    _numberOfIterationsAction.setToolTip("Number of iterations to compute");
    _startAnalysisAction.setToolTip("Start the analysis");

    addAction(&_currentIterationAction);
    addAction(&_maxVelocityAction);
    addAction(&_numberOfIterationsAction);
    addAction(&_startAnalysisAction);
}
