#include "ExampleActionsPlugin.h"
#include "ExampleProxyAction.h"

#include <CoreInterface.h>

#include <QDebug>
#include <QSplitter>

Q_PLUGIN_METADATA(IID "studio.manivault.ExampleActionsPlugin")

using namespace mv;

ExampleActionsPlugin::ExampleActionsPlugin(const PluginFactory* factory) :
    ViewPlugin(factory),
    _exampleActionsTreeModel(this),
    _exampleActionsFilterModel(this),
    _examplesTreeAction(this, "Examples tree"),
    _examplesGroupsAction(this, "Examples groups")
{
    _examplesTreeAction.setIconByName("play");

    _examplesTreeAction.setWidgetConfigurationFunction([this](WidgetAction* action, QWidget* widget) -> void {
        auto hierarchyWidget = widget->findChild<HierarchyWidget*>("HierarchyWidget");

        Q_ASSERT(hierarchyWidget);

        if (!hierarchyWidget)
            return;

        auto selectionModel = hierarchyWidget->getTreeView().selectionModel();

        connect(selectionModel, &QItemSelectionModel::selectionChanged, hierarchyWidget, [this, selectionModel](const QItemSelection& selected, const QItemSelection& deselected) -> void {
            _examplesGroupsAction.resetGroupActions();

            QSet<QModelIndex> selectedModelIndexes;

            for (const auto& selectedIndex : selectionModel->selectedRows(static_cast<int>(ExampleActionsTreeModel::Column::ClassName))) {
                const auto nameIndex = selectedIndex.siblingAtColumn(static_cast<int>(ExampleActionsTreeModel::Column::Name));

                if (nameIndex.parent().isValid()) {
                    selectedModelIndexes << selectedIndex;
                }
                else {
                    for (int rowIndex = 0; rowIndex < _exampleActionsFilterModel.rowCount(nameIndex); rowIndex++)
                        selectedModelIndexes << _exampleActionsFilterModel.index(rowIndex, static_cast<int>(ExampleActionsTreeModel::Column::ClassName), nameIndex);
                }
            }

            for (const auto& selectedModelIndex : selectedModelIndexes) {
                auto actionItem             = _exampleActionsTreeModel.itemFromIndex(_exampleActionsFilterModel.mapToSource(selectedModelIndex));
                auto action                 = dynamic_cast<AbstractExampleActionsModel::ExampleActionItem*>(actionItem)->getAction();
                auto name                   = QString("%1/%2").arg(selectedModelIndex.parent().data().toString(), selectedModelIndex.data().toString());
                auto verticalGroupAction    = new VerticalGroupAction(this, name, true);

                verticalGroupAction->setDefaultWidgetFlag(VerticalGroupAction::WidgetFlag::NoMargins);
                verticalGroupAction->setShowLabels(false);
                verticalGroupAction->addAction(new ExampleProxyAction(verticalGroupAction, "", action));

                _examplesGroupsAction.addGroupAction(verticalGroupAction);
            }
        });
    });
}

void ExampleActionsPlugin::init()
{
    auto layout = new QVBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);

    _examplesTreeAction.initialize(&_exampleActionsTreeModel, &_exampleActionsFilterModel, "Example action");

    auto splitter = new QSplitter();

    splitter->setOrientation(Qt::Orientation::Horizontal);

    splitter->addWidget(_examplesTreeAction.createWidget(&getWidget()));
    splitter->addWidget(_examplesGroupsAction.createWidget(&getWidget()));

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    layout->addWidget(splitter);

    getWidget().setLayout(layout);
}

ViewPlugin* ExampleActionsPluginFactory::produce()
{
    return new ExampleActionsPlugin(this);
}
