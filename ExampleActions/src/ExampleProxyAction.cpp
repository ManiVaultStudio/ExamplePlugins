// SPDX-License-Identifier: LGPL-3.0-or-later 
// A corresponding LICENSE file is located in the root directory of this source tree 
// Copyright (C) 2023 BioVault (Biomedical Visual Analytics Unit LUMC - TU Delft) 

#include "ExampleProxyAction.h"

#include <QHBoxLayout>

using namespace mv::gui;

ExampleProxyAction::ExampleProxyAction(QObject* parent, const QString& title, WidgetAction* action /*= nullptr*/) :
    WidgetAction(parent, title),
    _action(action)
{
}

QWidget* ExampleProxyAction::getWidget(QWidget* parent, const std::int32_t& widgetFlags)
{
    auto widget = new WidgetActionWidget(parent, this);
    auto layout = new QHBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);

    if (_action)
        layout->addWidget(_action->createExampleWidget(widget));

    widget->setLayout(layout);

    return widget;
}
