#pragma once

#include <QSortFilterProxyModel>
#include <QVector>

/**
 * A QSortFilterProxyModel that filters recursively through all child items.
 * 
 * @author Thomas Kroes
*/
class ActionsFilterModel final : public QSortFilterProxyModel
{
    Q_OBJECT
public:

    /**
     * Construct with optional parent.
     * @param parent The parent QObject.
     */
    explicit ActionsFilterModel(QObject* parent = nullptr);

    // Optional: include tooltips and ids in matching.
    void setSearchRoles(QVector<int> roles);

protected:

    /**
     * Reimplemented to filter recursively.
     * @param sourceRow The row in the source model.
     * @param sourceParent The parent index in the source model.
     * @return True if the row should be included in the filtered model.
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:

    /**
     * Check if a row matches the current filter.
     * @param idx The index of the row to check.
     * @return True if the row matches the filter, false otherwise.
     */
    bool rowMatches(const QModelIndex& idx) const;


private:
    QVector<int> searchRoles;       /** The roles to search in. */
};
