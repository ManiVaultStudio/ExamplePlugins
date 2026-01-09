#pragma once

#include "ActionsFilterModel.h"

#include <QApplication>
#include <QHash>
#include <QHeaderView>
#include <QLineEdit>
#include <QScrollArea>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>

#include <functional>

/* Custom roles for action items */
namespace Roles {
    static constexpr int NodeType = Qt::UserRole + 1;  // int
    static constexpr int ActionId = Qt::UserRole + 2;  // QString
}

/* Node types for items in the model */
enum NodeType { CategoryNode = 0, ActionNode = 1 };

/* Factory function type for creating detail widgets */
using Factory = std::function<QWidget*(QWidget* parent)>;

class ActionsWidget final : public QWidget
{
    Q_OBJECT
public:

    /**
     * Construct with optional parent.
     * @param parent The parent QWidget.
     */
    explicit ActionsWidget(QWidget* parent = nullptr);

    /**
     * Add an action to the widget.
     * @param category The category of the action.
     * @param text The display text of the action.
     * @param actionId The unique identifier of the action.
     * @param factory The factory function to create the detail widget.
     * @param icon The optional icon for the action.
     * @param toolTip The optional tooltip for the action.
     */
    void addAction(const QString& category, const QString& text, const QString& actionId, Factory factory, const QIcon& icon = {}, const QString& toolTip = {});

private slots:

    /**
     * Slot called when the current selection changes in the tree view.
     * @param current The new current index.
     * @param previous The previous current index (unused).
     */
    void onCurrentChanged(const QModelIndex& current, const QModelIndex& previous);

private:

    /*
     * Ensure that a category item exists in the model
     * @param category The category name
     * @return The QStandardItem representing the category
     */
    QStandardItem* ensureCategory(const QString& category);

    /** Clear the details area  */
    void clearDetails();

private:
    QStandardItemModel              _model;             /** The underlying model */
    ActionsFilterModel              _proxy;             /** The filter proxy model */
    QVBoxLayout                     _mainLayout;        /** The main layout */
    QSplitter                       _splitter;          /** The main splitter */
    QWidget                         _leftPanelWidget;   /** The left panel widget */
    QVBoxLayout                     _leftPanelLayout;   /** The left panel layout */
    QWidget                         _rightPanelWidget;  /** The right panel widget */
    QVBoxLayout                     _rightPanelLayout;  /** The right panel layout */
    QLineEdit                       _search;            /** The search box */
    QTreeView                       _tree;              /** The tree view */
    QScrollArea                     _detailsArea;       /** The details scroll area */
    QWidget                         _detailsHost;       /** The details host widget */
    QVBoxLayout                     _detailsLayout;     /** The details layout */
    QHash<QString, QStandardItem*>  _categories;        /** The category items */
    QHash<QString, Factory>         _factories;         /** The action detail widget factories */
};
