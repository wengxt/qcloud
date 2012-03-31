#ifndef QCLOUD_AUTHDIALOG_H
#define QCLOUD_AUTHDIALOG_H
#include <QDialog>
#include "qcloud_global.h"

namespace QCloud
{

class AuthWidget;
class QCLOUD_EXPORT AuthDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AuthDialog (AuthWidget* widget, QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~AuthDialog();

protected slots:
    void authFinished (bool success);
};
}

#endif
