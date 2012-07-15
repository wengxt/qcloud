#include <QIcon>
#include <QFileInfo>
#include <QFile>
#include <QDebug>

#include "entryinfomodel.h"

EntryInfoModel::EntryInfoModel (QObject* parent) : QAbstractListModel (parent)
{
}

QModelIndex EntryInfoModel::index (int row, int column, const QModelIndex& parent) const
{
    return createIndex (row, column, (row >= 0 && row < m_entryInfoList.count() ? (void*) &m_entryInfoList.at (row) : 0));
}


QVariant EntryInfoModel::data (const QModelIndex& index, int role) const
{
    const QCloud::EntryInfo* entry = (QCloud::EntryInfo*) index.internalPointer();
    if (!entry)
        return QVariant();

    switch (role) {
        case Qt::DisplayRole:
            return m_parentPath.relativeFilePath(entry->path());
        case Qt::DecorationRole:
            return QIcon::fromTheme (entry->icon());
        default:
            return QVariant();
    }
}

int EntryInfoModel::rowCount (const QModelIndex& parent) const
{
    return m_entryInfoList.size();
}

void EntryInfoModel::setEntryInfoList (const QString& parentPath,QCloud::EntryInfoList infoList)
{
    qDebug() << "The current parent Dir is : " << parentPath;
    m_parentPath.setPath(parentPath);
    beginRemoveRows(QModelIndex(), 0, m_entryInfoList.size());
    m_entryInfoList.clear();
    endRemoveRows();
    beginInsertRows(QModelIndex(), 0, infoList.count() - 1);
    m_entryInfoList = infoList;
    endInsertRows();
}
