#ifndef QCLOUD_ISECURESTOREWIDGET_H
#define QCLOUD_ISECURESTOREWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include "isecurestore.h"
#include "qcloud_global.h"

namespace QCloud
{

class QCLOUD_EXPORT SecureStoreWidget : public QDialog
{
    Q_OBJECT
public:
    SecureStoreWidget (ISecureStore* storage);
    ~SecureStoreWidget();

protected slots:
    bool SetItem();
    bool GetItem();

protected:
    ISecureStore* m_storage;
    QLineEdit* m_keyInput;
    QLineEdit* m_valueInput;
    QLabel* m_statLabel;
    QPushButton* m_GetButton;
    QPushButton* m_SetButton;
};

}

#endif
