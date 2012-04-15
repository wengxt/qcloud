#ifndef QCLOUD_ACCOUNTINFO_H
#define QCLOUD_ACCOUNTINFO_H
#include <QString>
#include <QVariant>
#include <QtCrypto/QtCrypto>

namespace QCloud {
class AccountInfo
{
public:
    AccountInfo();

private:
    QString m_backend;
    QString m_app;
    QString m_userName;
    QVariantMap m_data;
    QMap<QString, QCA::SecureArray> m_secureData;
};
}

#endif