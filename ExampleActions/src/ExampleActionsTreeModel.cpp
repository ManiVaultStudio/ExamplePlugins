// SPDX-License-Identifier: LGPL-3.0-or-later 
// A corresponding LICENSE file is located in the root directory of this source tree 
// Copyright (C) 2023 BioVault (Biomedical Visual Analytics Unit LUMC - TU Delft) 

#include "ExampleActionsTreeModel.h"

#include <actions/WidgetAction.h>

#include <util/Exception.h>

using namespace mv;
using namespace mv::util;
using namespace mv::gui;

#ifdef _DEBUG
    #define EXAMPLE_ACTIONS_TREE_MODEL_VERBOSE
#endif

ExampleActionsTreeModel::ExampleActionsTreeModel(QObject* parent /*= nullptr*/) :
    AbstractExampleActionsModel(parent)
{
    setColumnCount(static_cast<int>(Column::Count));

    appendRow(CategoryRow("Color", {
        { "Color action", "For picking colors", "mv::gui::ColorAction" },
        { "1D Color map action", "For configuring one-dimensional color maps", "mv::gui::ColorMap1DAction" },
        { "2D Color map action", "For configuring two-dimensional color maps", "mv::gui::ColorMap2DAction" }
    }));

    appendRow(CategoryRow("File", {
        { "File picker", "For picking file location", "mv::gui::FilePickerAction" },
        { "Directory picker", "For picking directories", "mv::gui::DirectoryPickerAction" }
    }));

    appendRow(CategoryRow("Grouping", {
        { "Horizontal group", "For laying out actions horizontally", "mv::gui::HorizontalGroupAction" },
        { "Vertical group", "For laying out actions vertically", "mv::gui::VerticalGroupAction" },
        { "Groups", "Vertical groups accordion action", "mv::gui::GroupsAction" },
        { "Horizontal toolbar", "Horizontal group action that changes the state of action based on the available width ", "mv::gui::HorizontalToolbarAction" },
        { "Stretch", "Adds stretch to a group action", "mv::gui::StretchAction" }
    }));

    appendRow(CategoryRow("Miscellaneous", {
        { "Dataset picker", "For picking datasets", "mv::gui::DatasetPickerAction" }
    }));

    appendRow(CategoryRow("Numerical", {
        { "Integral", "For configuring an integral value", "mv::gui::IntegralAction" },
        { "IntegralRange", "For configuring an integral range", "mv::gui::IntegralRangeAction" },
        { "Decimal", "For configuring a decimal value", "mv::gui::DecimalAction" },
        { "DecimalRange", "For configuring a decimal range", "mv::gui::DecimalRangeAction" },
    }));
}

QVariant ExampleActionsTreeModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    switch (static_cast<Column>(section))
    {
        case Column::Name:
            return "Name";

        case Column::Description:
            return "Description";

        case Column::ClassName:
            return "Class name";

        default:
            break;
    }

    return {};
}

ExampleActionsTreeModel::CategoryRow::CategoryRow(const QString& category, const QList<QStringList>& actions) : QList<QStandardItem*>()
{
    auto categoryItem = new QStandardItem(category);

    for (const auto& action : actions) {
        QList<QStandardItem*> items;

        try {
            for (const auto& string : action) {

                const auto metaType     = action[2];
                const auto metaTypeId   = QMetaType::type(metaType.toLatin1());
                const auto metaObject   = QMetaType::metaObjectForType(metaTypeId);

                if (!metaObject)
                    throw std::runtime_error(QString("Meta object type '%1' is not known. Did you forget to register the action correctly with Qt meta object system? See ToggleAction.h for an example.").arg(metaType).toLatin1());

                auto metaObjectInstance = metaObject->newInstance(Q_ARG(QObject*, nullptr), Q_ARG(QString, QString("Example%1").arg(metaType)));
                auto exampleAction      = dynamic_cast<WidgetAction*>(metaObjectInstance);

                if (!exampleAction)
                    throw std::runtime_error(QString("Unable to create a new instance of type '%1'").arg(metaType).toLatin1());

                items << new ExampleActionItem(string, exampleAction);
            }
        }
        catch (std::exception& e)
        {
            exceptionMessageBox("Unable to load example action:", e);
        }
        catch (...)
        {
            exceptionMessageBox("Unable to load example action:");
        }

        categoryItem->appendRow(items);
    }

    append(categoryItem);
}
