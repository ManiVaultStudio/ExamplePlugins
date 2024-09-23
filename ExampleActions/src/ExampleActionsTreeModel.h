// SPDX-License-Identifier: LGPL-3.0-or-later 
// A corresponding LICENSE file is located in the root directory of this source tree 
// Copyright (C) 2023 BioVault (Biomedical Visual Analytics Unit LUMC - TU Delft) 

#pragma once

#include "AbstractExampleActionsModel.h"

#include <QList>
#include <QStandardItem>

/**
 * Example actions tree model class
 *
 * Example actions grouped by category
 *
 * @author Thomas Kroes
 */
class ExampleActionsTreeModel : public AbstractExampleActionsModel
{
public:

    /** Model columns */
    enum class Column {
        Name,               /** Item name */
        Description,        /** Action/category description */
        ClassName,          /** Action class name */

        Count
    };

protected:

    /** Convenience class for category row and child action items */
    class CategoryRow final : public QList<QStandardItem*>
    {
    public:

        /**
         * Construct with \p name, \p description and \p className
         * @param category Action category
         * @param actions Child actions
         */
        CategoryRow(const QString& category, const QList<QStringList>& actions);
    };

public:

    /**
     * Construct with pointer to \p parent object
     * @param parent Pointer to parent object
     */
    ExampleActionsTreeModel(QObject* parent = nullptr);

    /**
     * Get header data for \p section, \p orientation and display \p role
     * @param section Section
     * @param orientation Orientation
     * @param role Data role
     * @return Header
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    friend class ExampleActionStyledItemDelegate;
};
