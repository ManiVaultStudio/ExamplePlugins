#pragma once

#include "ActionsFilterModel.h"

#include "actions/HorizontalGroupAction.h"
#include "actions/OptionsAction.h"

#include <QApplication>
#include <QHash>
#include <QLineEdit>
#include <QScrollArea>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>

#include <functional>

using namespace mv::gui;

/* Custom roles for action items */
namespace Roles {
    static constexpr int NodeType               = Qt::UserRole + 1;  // int
    static constexpr int ActionId               = Qt::UserRole + 2;  // QString
    static constexpr int WidgetFlags            = Qt::UserRole + 4;  // QVariantMap
    static constexpr int SelectedWidgetFlags    = Qt::UserRole + 5;  // QVariantMap
}

/* Node types for items in the model */
enum NodeType { CategoryNode = 0, ActionNode = 1 };

/* Factory function type for creating detail widgets */
using Factory = std::function<WidgetAction*(QWidget* parent)>;

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
     * Add an action to the widget
     * @param category The category of the action
     * @param text The display text of the action
     * @param factory The factory function to create the detail widget
     * @param icon The optional icon for the action
     * @param toolTip The optional tooltip for the action.
     */
    void addAction(const QString& category, const QString& text, Factory factory, const QVariantMap& widgetFlags = {}, const QStringList& selectedWidgetFlags = {}, const QIcon & icon = {}, const QString & toolTip = {});

private slots:

    /**
     * Slot called when the current selection changes in the tree view.
     */
    void requestUpdateWidget();

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
    QStandardItemModel              _model;                                 /** The underlying model */
    ActionsFilterModel              _proxy;                                 /** The filter proxy model */
    QVBoxLayout                     _mainLayout;                            /** The main layout */
    QSplitter                       _splitter;                              /** The main splitter */
    QWidget                         _leftPanelWidget;                       /** The left panel widget */
    QVBoxLayout                     _leftPanelLayout;                       /** The left panel layout */
    QWidget                         _rightPanelWidget;                      /** The right panel widget */
    QVBoxLayout                     _rightPanelLayout;                      /** The right panel layout */
    QLineEdit                       _search;                                /** The search box */
    QTreeView                       _tree;                                  /** The tree view */
    HorizontalGroupAction           _widgetSettingsAction;                  /** The widget settings action */
	OptionsAction                   _widgetFlagsAction;                     /** The options action for widget flags */
    ToggleAction                    _widgetEnabledAction;                   /** The toggle action for enabling/disabling the widget */
    VerticalGroupAction             _detailsGroupAction;                    /** The details group action */    
    QScrollArea                     _detailsArea;                           /** The details scroll area */
    QWidget                         _detailsHost;                           /** The details host widget */
    QVBoxLayout                     _detailsLayout;                         /** The details layout */
    QHash<QString, QStandardItem*>  _categories;                            /** The category items */
    QHash<QString, Factory>         _factories;                             /** The action detail widget factories */
    bool                            _ignoreRequestUpdateWidget = false;     /** Flag to ignore update requests */
};
