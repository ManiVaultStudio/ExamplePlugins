#pragma once

#include <actions/VerticalGroupAction.h>
#include <actions/StringAction.h>
#include <PointData/DimensionPickerAction.h>

using namespace hdps::gui;

class ExampleViewGLPlugin;
class QMenu;

/**
 * Settings action class
 *
 * Action class for configuring settings
 */
class SettingsAction : public VerticalGroupAction
{
    public:
    
    /**
     * Construct with \p parent object and \p title
     * @param parent Pointer to parent object
     * @param title Title
     */
    Q_INVOKABLE SettingsAction(QObject* parent, const QString& title);

    /**
     * Get action context menu
     * @return Pointer to menu
     */
    QMenu* getContextMenu();

public: // Action getters
    
    StringAction& getDatasetNameAction() { return _datasetNameAction; }
    DimensionPickerAction& getXDimensionPickerAction() { return _xDimensionPickerAction; }
    DimensionPickerAction& getYDimensionPickerAction() { return _yDimensionPickerAction; }

private:
    ExampleViewGLPlugin*    _exampleViewGLPlugin;       /** Pointer to Example OpenGL Viewer Plugin */
    StringAction            _datasetNameAction;         /** Action for displaying the current data set name */
    DimensionPickerAction   _xDimensionPickerAction;    /** X-dimension picker action */
    DimensionPickerAction   _yDimensionPickerAction;    /** Y-dimension picker action */

};
