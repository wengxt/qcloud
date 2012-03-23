#ifndef QCLOUD_ISECURESTOREWIDGET_H
#define QCLOUD_ISECURESTOREWIDGET_H

#include "isecurestore.h"
#include <QWidget>
#include <QLineEdit>
#include <QLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QLabel>

namespace QCloud{
    
class ISecureStoreWidget : public QDialog
{
Q_OBJECT
public:
    ISecureStoreWidget(ISecureStore *storage);
    ~ISecureStoreWidget();
    
protected slots:
    bool SetItem();
    bool GetItem();
    
protected:
    ISecureStore *m_storage;
    QLineEdit *m_keyInput;
    QLineEdit *m_valueInput;
    QLabel *m_statLabel;
    QPushButton *m_GetButton;
    QPushButton *m_SetButton;
};

}

#endif