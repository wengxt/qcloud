#include "securestorewidget.h"

namespace QCloud
{
SecureStoreWidget::SecureStoreWidget (ISecureStore* storage)
{
    m_storage = storage;
    setLayout (new QVBoxLayout);
    QLabel* explanation = new QLabel (
        "Press 'Get' to get value of the key you input.\n"
        "Or press 'Set' to set the key and value."
    );
    m_statLabel = new QLabel ("Opening storage...", this);
    m_statLabel->setAlignment (Qt::AlignCenter);
    m_keyInput = new QLineEdit (this);
    m_valueInput = new QLineEdit (this);
    m_GetButton = new QPushButton ("Get!", this);
    m_SetButton = new QPushButton ("Set!", this);

    layout()->addWidget (explanation);
    layout()->addWidget (m_statLabel);
    layout()->addWidget (m_keyInput);
    layout()->addWidget (new QLabel ("key", this));
    layout()->addWidget (m_valueInput);
    layout()->addWidget (new QLabel ("value", this));
    layout()->addWidget (m_GetButton);
    layout()->addWidget (m_SetButton);

    connect (m_GetButton, SIGNAL (clicked()), SLOT (GetItem()));
    connect (m_SetButton, SIGNAL (clicked()), SLOT (SetItem()));

    if (storage == NULL || (!storage->isAvailable()))
        m_statLabel->setText ("Failed opening wallet!");
    else
        m_statLabel->setText ("Succeeded in opening wallet!");
}

bool SecureStoreWidget::GetItem()
{
    QByteArray value;
    bool ret = m_storage->readItem ("test", m_keyInput->text(), value);
    if (ret) {
        m_valueInput->setText (QString::fromUtf8(value));
        m_statLabel->setText ("Succeeded in getting value!");
    } else
        m_statLabel->setText ("Failed getting value!");
    return ret;
}

bool SecureStoreWidget::SetItem()
{
    bool ret = m_storage->writeItem ("test", m_keyInput->text(), m_valueInput->text().toUtf8());
    if (ret)
        m_statLabel->setText ("Succeed in setting value!");
    else
        m_statLabel->setText ("Failed settings value!");
    return ret;
}

SecureStoreWidget::~SecureStoreWidget()
{
}

}
