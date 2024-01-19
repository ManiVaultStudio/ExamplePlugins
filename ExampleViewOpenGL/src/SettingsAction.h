#pragma once

#include <actions/GroupAction.h>
#include <actions/StringAction.h>
#include <actions/DecimalAction.h>
#include <PointData/DimensionPickerAction.h>

using namespace mv::gui;

class ExampleViewGLPlugin;

/**
 * Settings action class
 *
 * Action class for configuring settings
 */
class SettingsAction : public GroupAction
{
public:

    /**
     * Construct with \p parent object and \p title
     * @param parent Pointer to parent object
     * @param title Title
     */
    Q_INVOKABLE SettingsAction(QObject* parent, const QString& title);

public: // Action getters
    
    StringAction& getDatasetNameAction() { return _datasetNameAction; }
    DimensionPickerAction& getXDimensionPickerAction() { return _xDimensionPickerAction; }
    DimensionPickerAction& getYDimensionPickerAction() { return _yDimensionPickerAction; }
    DecimalAction& getPointSizeAction() { return _pointSizeAction; }
    DecimalAction& getPointOpacityAction() { return _pointOpacityAction; }

private:
    ExampleViewGLPlugin*    _exampleViewGLPlugin;       /** Pointer to Example OpenGL Viewer Plugin */
    StringAction            _datasetNameAction;         /** Action for displaying the current data set name */
    DimensionPickerAction   _xDimensionPickerAction;    /** X-dimension picker action */
    DimensionPickerAction   _yDimensionPickerAction;    /** Y-dimension picker action */
    DecimalAction           _pointSizeAction;           /** point size action */
    DecimalAction           _pointOpacityAction;        /** point opacity action */
};
