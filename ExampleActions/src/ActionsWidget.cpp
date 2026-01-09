#include "ActionsWidget.h"

#include <QUuid>
#include <QHeaderView>

ActionsWidget::ActionsWidget(QWidget* parent):
	QWidget(parent),
    _model(this),
	_proxy(this),
	_splitter(Qt::Horizontal, this),
    _widgetSettingsAction(this, "Widget settings"),
    _widgetFlagsAction(this, "Widget flags"),
    _widgetEnabledAction(this, "Enabled"),
    _detailsGroupAction(this, "Details")
{
	_proxy.setSourceModel(&_model);
	_proxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
	_proxy.setSearchRoles({Qt::ToolTipRole, Roles::ActionId});

    _mainLayout.addWidget(&_splitter);

    setLayout(&_mainLayout);

    _leftPanelWidget.setLayout(&_leftPanelLayout);
    _rightPanelWidget.setLayout(&_rightPanelLayout);

    _splitter.addWidget(&_leftPanelWidget);
    _splitter.addWidget(&_rightPanelWidget);

    _splitter.setStretchFactor(0, 0);
    _splitter.setStretchFactor(1, 1);

    _splitter.setSizes({ 320, 680 });

    _leftPanelLayout.setContentsMargins(0, 0, 0, 0);
    _leftPanelLayout.addWidget(&_search);
    _leftPanelLayout.addWidget(&_tree);

    _rightPanelLayout.addWidget(_widgetSettingsAction.createWidget(this));
    _rightPanelLayout.addWidget(&_detailsArea);
    _rightPanelLayout.setContentsMargins(0, 0, 0, 0);

	_search.setPlaceholderText("Search actions");

	_tree.setModel(&_proxy);
	_tree.setHeaderHidden(true);
	_tree.setUniformRowHeights(true);
	_tree.setEditTriggers(QAbstractItemView::NoEditTriggers);
	_tree.setSelectionMode(QAbstractItemView::SingleSelection);
	_tree.setSelectionBehavior(QAbstractItemView::SelectRows);

	_tree.header()->setStretchLastSection(true);

	_detailsArea.setWidgetResizable(true);
	_detailsArea.setWidget(&_detailsHost);

    _detailsHost.setLayout(&_detailsLayout);

	connect(&_search, &QLineEdit::textChanged, this, [this](const QString& text) {
		_proxy.setFilterRegularExpression(QRegularExpression(QRegularExpression::escape(text), QRegularExpression::CaseInsensitiveOption));

		if (!text.isEmpty())
			_tree.expandAll();
		else
			_tree.collapseAll();
	});

	connect(_tree.selectionModel(), &QItemSelectionModel::currentChanged, this, &ActionsWidget::requestUpdateWidget);
    connect(&_widgetFlagsAction, &OptionsAction::selectedOptionsChanged, this, [this](const QStringList& selectedOptions) -> void {
        const auto current = _tree.selectionModel()->currentIndex();

        if (!current.isValid())
            return;

        const auto sourceIndex = _proxy.mapToSource(current);

        _model.setData(sourceIndex, selectedOptions, Roles::SelectedWidgetFlags);

        requestUpdateWidget();
    });

    _widgetSettingsAction.addAction(&_widgetFlagsAction);
    _widgetSettingsAction.addAction(&_widgetEnabledAction);
}

void ActionsWidget::addAction(const QString& category, const QString& text, Factory factory, const QVariantMap& widgetFlags, const QStringList& selectedWidgetFlags, const QIcon& icon, const QString& toolTip)
{
	const auto cat      = category.trimmed().isEmpty() ? QStringLiteral("Uncategorized") : category.trimmed();
    const auto actionId = QUuid::createUuid().toString(QUuid::WithoutBraces);

	auto categoryItem   = ensureCategory(cat);
	auto actionItem     = new QStandardItem(icon, text);

	actionItem->setToolTip(toolTip);
	actionItem->setData(ActionNode, Roles::NodeType);
	actionItem->setData(actionId, Roles::ActionId);

    if (!widgetFlags.isEmpty())
        actionItem->setData(widgetFlags, Roles::WidgetFlags);
    
	if (!selectedWidgetFlags.isEmpty())
		actionItem->setData(selectedWidgetFlags, Roles::SelectedWidgetFlags);

	categoryItem->appendRow(actionItem);

	_factories.insert(actionId, std::move(factory));
}

void ActionsWidget::requestUpdateWidget()
{
    if (_ignoreRequestUpdateWidget)
        return;

	clearDetails();

    const auto current = _tree.selectionModel()->currentIndex();

	if (!current.isValid())
		return;

	const auto sourceIndex = _proxy.mapToSource(current);

    auto item = _model.itemFromIndex(sourceIndex);

	if (!item)
		return;

	const auto type = item->data(Roles::NodeType).toInt();

	if (type != ActionNode)
		return; // category selected: leave details empty (or show placeholder)

	const auto actionId = item->data(Roles::ActionId).toString();

	auto it = _factories.find(actionId);

	if (it == _factories.end())
		return;

    const auto widgetFlagsMap       = item->data(Roles::WidgetFlags).toMap();
    const auto selectedWidgetFlags  = item->data(Roles::SelectedWidgetFlags).toStringList();

    _widgetFlagsAction.setEnabled(!widgetFlagsMap.isEmpty());

    _ignoreRequestUpdateWidget = true;
    {
        _widgetFlagsAction.initialize(widgetFlagsMap.keys(), selectedWidgetFlags);
    }
    _ignoreRequestUpdateWidget = false;

	auto action = it.value()(&_detailsHost);

	if (!action)
		return;

    int defaultWidgetFlags = 0;

    for (const auto& selectedWidgetFlag : selectedWidgetFlags)
        defaultWidgetFlags |= widgetFlagsMap.value(selectedWidgetFlag).toInt();

    if (!widgetFlagsMap.isEmpty())
		action->setDefaultWidgetFlags(defaultWidgetFlags);

    _detailsGroupAction.addAction(action);
    _detailsGroupAction.addStretch();
}

QStandardItem* ActionsWidget::ensureCategory(const QString& category)
{
	if (auto it = _categories.find(category); it != _categories.end())
		return it.value();

	auto catItem = new QStandardItem(category);

	catItem->setData(CategoryNode, Roles::NodeType);
	catItem->setSelectable(true);

	_model.appendRow(catItem);
	_categories.insert(category, catItem);

	return catItem;
}

void ActionsWidget::clearDetails()
{
    while (QLayoutItem* child = _detailsLayout.takeAt(0))
    {
        if (QWidget* w = child->widget())
            w->deleteLater();
        delete child;
    }

    _detailsGroupAction.clear();

    _detailsLayout.addWidget(_detailsGroupAction.createWidget(this));
}
