// SPDX-License-Identifier: LGPL-3.0-or-later 
// A corresponding LICENSE file is located in the root directory of this source tree 
// Copyright (C) 2023 BioVault (Biomedical Visual Analytics Unit LUMC - TU Delft) 

#pragma once

#include <models/StandardItemModel.h>

#include <QStandardItem>

namespace mv::gui {
    class WidgetAction;
}

/**
 * Abstract example actions model class
 *
 * @author Thomas Kroes
 */
class AbstractExampleActionsModel : public mv::StandardItemModel
{
protected:

    /** Base standard model item class for example widget action */
    class ExampleActionItem : public QStandardItem, public QObject {
    public:

        /**
         * Construct with pointer to \p action
         * @param title Item title
         * @param action Pointer to action to display item for
         */
        ExampleActionItem(const QString& title, mv::gui::WidgetAction* action);

        /**
         * Get action
         * return Pointer to action to display item for
         */
        mv::gui::WidgetAction* getAction() const;

    private:
        mv::gui::WidgetAction*  _action;        /** Pointer to action to display item for */
    };

public:

    /** No need for custom constructor */
    using mv::StandardItemModel::StandardItemModel;

    friend class ExampleActionStyledItemDelegate;
    friend class ExampleActionsPlugin;
};
