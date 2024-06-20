// SPDX-License-Identifier: LGPL-3.0-or-later 
// A corresponding LICENSE file is located in the root directory of this source tree 
// Copyright (C) 2023 BioVault (Biomedical Visual Analytics Unit LUMC - TU Delft) 

#pragma once

#include <actions/WidgetAction.h>

/**
 * Example proxy action class
 *
 * Uses target action createExampleWidget(...) in stead of createWidget()
 *
 * @author Thomas Kroes
 */
class ExampleProxyAction : public mv::gui::WidgetAction
{
    Q_OBJECT

protected:

    /**
     * Get widget representation of the proxy action
     * @param parent Pointer to parent widget
     * @param widgetFlags Widget flags for the configuration of the widget (type)
     */
    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override;

public:

    /**
     * Constructor
     * @param parent Pointer to parent object
     * @param title Title of the action
     * @param action Pointer to action
     */
    Q_INVOKABLE ExampleProxyAction(QObject* parent, const QString& title, mv::gui::WidgetAction* action = nullptr);

private:
    mv::gui::WidgetAction* _action;     /** Pointer to target action */

    friend class AbstractActionsManager;
};

Q_DECLARE_METATYPE(ExampleProxyAction)

inline const auto exampleProxyActionMetaTypeId = qRegisterMetaType<ExampleProxyAction*>("ExampleProxyAction");
