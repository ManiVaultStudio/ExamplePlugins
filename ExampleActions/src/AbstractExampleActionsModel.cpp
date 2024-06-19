// SPDX-License-Identifier: LGPL-3.0-or-later 
// A corresponding LICENSE file is located in the root directory of this source tree 
// Copyright (C) 2023 BioVault (Biomedical Visual Analytics Unit LUMC - TU Delft) 

#include "AbstractExampleActionsModel.h"

#include <actions/WidgetAction.h>

#include <util/Exception.h>

using namespace mv;
using namespace mv::util;
using namespace mv::gui;

#ifdef _DEBUG
    #define ABSTRACT_EXAMPLE_ACTIONS_MODEL_VERBOSE
#endif

AbstractExampleActionsModel::ExampleActionItem::ExampleActionItem(const QString& title, mv::gui::WidgetAction* action) :
    QStandardItem(title),
    QObject(),
    _action(action)
{
    setEditable(false);
    setDropEnabled(false);

    Q_ASSERT(_action != nullptr);

    if (!_action)
        return;
}

mv::gui::WidgetAction* AbstractExampleActionsModel::ExampleActionItem::getAction() const
{
    return _action;
}
