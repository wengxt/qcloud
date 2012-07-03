#ifndef ENTRYINFOMODEL_H
#define ENTRYINFOMODEL_H
#include <QAbstractItemModel>
#include <QDir>
#include "entryinfo.h"

class EntryInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit EntryInfoModel (QObject* parent = 0);
    virtual QModelIndex index (int row, int column = 0, const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    void setEntryInfoList(const QString& parent,QCloud::EntryInfoList infoList);
private:
    QCloud::EntryInfoList m_entryInfoList;
    QDir m_parentPath;
};

#endif // INFOMODEL_H