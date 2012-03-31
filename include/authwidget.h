#ifndef QCLOUD_AUTHWIDGET_H
#define QCLOUD_AUTHWIDGET_H
#include <QtGui/QDialog>
#include <QtCloud/IBackend>
#include "qcloud_global.h"

namespace QCloud
{
class QCLOUD_EXPORT AuthWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AuthWidget (QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~AuthWidget();

public slots:
    virtual void startAuth() = 0;

signals:
    void authFinished (bool success);
};
}

#endif
