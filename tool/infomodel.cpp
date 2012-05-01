#include <QIcon>

#include "infomodel.h"

InfoModel::InfoModel (QObject* parent) : QAbstractListModel (parent)
{
}

QModelIndex InfoModel::index (int row, int column, const QModelIndex& parent) const
{
    return createIndex (row, column, (row >= 0 && row < m_infoList.count() ? (void*) &m_infoList.at (row) : 0));
}


QVariant InfoModel::data (const QModelIndex& index, int role) const
{
    const QCloud::Info* app = (QCloud::Info*) index.internalPointer();
    if (!app)
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return app->displayName();
    case Qt::DecorationRole:
        return QIcon::fromTheme (app->iconName());
    default:
        return QVariant();
    }
}

int InfoModel::rowCount (const QModelIndex& parent) const
{
    return m_infoList.size();
}

void InfoModel::setInfoList (QCloud::InfoList infoList)
{
    beginRemoveRows(QModelIndex(), 0, m_infoList.size());
    m_infoList.clear();
    endRemoveRows();
    beginInsertRows(QModelIndex(), 0, infoList.count() - 1);
    m_infoList = infoList;
    endInsertRows();
}
