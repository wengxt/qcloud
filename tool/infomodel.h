#ifndef INFOMODEL_H
#define INFOMODEL_H
#include <QAbstractItemModel>
#include "info.h"

class InfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit InfoModel (QObject* parent = 0);
    virtual QModelIndex index (int row, int column = 0, const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    void setInfoList(QCloud::InfoList infoList);
private:
    QCloud::InfoList m_infoList;
};

#endif // INFOMODEL_H