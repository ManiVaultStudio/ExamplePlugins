#include "ActionsWidget.h"

ActionsWidget::ActionsWidget(QWidget* parent):
	QWidget(parent),
    _model(this),
	_proxy(this),
	_splitter(Qt::Horizontal, this)
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

    _leftPanelLayout.addWidget(&_search);
    _leftPanelLayout.addWidget(&_tree);

    _rightPanelLayout.addWidget(&_detailsArea);

	_search.setPlaceholderText("Search actions");

	_tree.setModel(&_proxy);
	_tree.setHeaderHidden(true);
	_tree.setUniformRowHeights(true);
	_tree.setEditTriggers(QAbstractItemView::NoEditTriggers);
	_tree.setSelectionMode(QAbstractItemView::SingleSelection);
	_tree.setSelectionBehavior(QAbstractItemView::SelectRows);
	_tree.header()->setStretchLastSection(true);
	
	_detailsLayout.setContentsMargins(0, 0, 0, 0);

	_detailsArea.setWidgetResizable(true);
	_detailsArea.setWidget(&_detailsHost);

	connect(&_search, &QLineEdit::textChanged, this, [this](const QString& text) {
		_proxy.setFilterRegularExpression(QRegularExpression(QRegularExpression::escape(text), QRegularExpression::CaseInsensitiveOption));

		// UX: when searching, expand all; when empty, collapse to top-level
		if (!text.isEmpty())
			_tree.expandAll();
		else
			_tree.collapseAll();
	});

	connect(_tree.selectionModel(), &QItemSelectionModel::currentChanged, this, &ActionsWidget::onCurrentChanged);
}

void ActionsWidget::addAction(const QString& category, const QString& text, const QString& actionId, Factory factory, const QIcon& icon, const QString& toolTip)
{
	const auto cat = category.trimmed().isEmpty() ? QStringLiteral("Uncategorized") : category.trimmed();

	auto categoryItem = ensureCategory(cat);

	auto actionItem = new QStandardItem(icon, text);

	actionItem->setToolTip(toolTip);
	actionItem->setData(ActionNode, Roles::NodeType);
	actionItem->setData(actionId, Roles::ActionId);

	categoryItem->appendRow(actionItem);

	_factories.insert(actionId, std::move(factory));
}

void ActionsWidget::onCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	clearDetails();

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

	auto actionWidget = it.value()(&_detailsHost);

	if (!actionWidget)
		return;

	_detailsLayout.addWidget(actionWidget);
	_detailsLayout.addStretch(1);
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
	while (auto child = _detailsLayout.takeAt(0))
	{
		if (auto actionWidget = child->widget())
			actionWidget->deleteLater();

		delete child;
	}
}
