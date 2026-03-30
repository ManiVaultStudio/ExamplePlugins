#include "ActionsFilterModel.h"

ActionsFilterModel::ActionsFilterModel(QObject* parent) :
    QSortFilterProxyModel(parent)
{
}

void ActionsFilterModel::setSearchRoles(QVector<int> roles)
{
	searchRoles = std::move(roles);
}

bool ActionsFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    const QModelIndex idx = sourceModel()->index(sourceRow, 0, sourceParent);

    if (!idx.isValid())
        return false;

    if (rowMatches(idx))
        return true;

    const auto childCount = sourceModel()->rowCount(idx);

    for (int i = 0; i < childCount; ++i)
    {
        if (filterAcceptsRow(i, idx))
            return true;
    }

    return false;
}

bool ActionsFilterModel::rowMatches(const QModelIndex& idx) const
{
    const auto re = filterRegularExpression();
    if (!re.isValid() || re.pattern().isEmpty())
        return true;

    // Always include DisplayRole at minimum
    const QString display = sourceModel()->data(idx, Qt::DisplayRole).toString();
    if (display.contains(re))
        return true;

    for (int r : searchRoles) {
        const auto string = sourceModel()->data(idx, r).toString();

    	if (!string.isEmpty() && string.contains(re))
            return true;
    }

    return false;
}
