#include "qcloud_utils.h"
#include <QDateTime>
#include <QStringList>

namespace QCloud
{

QUrl customCallbackUrl()
{
    return QUrl ("http://qcloud-custom-url");
}

bool isCustomCallbackUrl (const QUrl& url)
{
    return (url.scheme() == "http"  && url.host() == "qcloud-custom-url");
}

QDateTime parseRFC2822Date(const QString& string)
{
    QStringList monthNames = QStringList() << "jan" << "feb" << "mar" << "apr"
                            << "may" << "jun" << "jul" << "aug" << "sep" << "oct" << "nov" << "dec";

    do {
        QRegExp rx(QString("^(?:\\s*([A-Z][a-z]+)\\s*,\\s*)?"   // date-of-week
                    "(\\d{1,2})\\s+(%1)\\s+(\\d{2,4})" // date
                        "\\s+(\\d{2})\\s*:(\\d{2})\\s*(?::\\s*(\\d{2})\\s*)" // time
                        "(\\s+(?:(?:([+-]?)(\\d{2})(\\d{2}))|(UT|GMT|EST|EDT|CST|CDT|MST|MDT|PST|PDT|[A-IK-Za-ik-z])))?" // timezone
                        ).arg(monthNames.join("|")), Qt::CaseInsensitive);
        int pos = rx.indexIn(string);
        if (pos == -1)
            break;
        QStringList list = rx.capturedTexts();
        if (list.size() != 13)
            break;
        int year = list[4].toInt();
        int month = monthNames.indexOf(list[3].toLower()) + 1;
        if (month == 0)
            break;
        int day = list[2].toInt();
        int hours = list[5].toInt();
        int minutes = list[6].toInt();
        int seconds = list[7].toInt();
        int shift = list[10].toInt() * 60 + list[11].toInt();
        if (list[9] == "-")
            shift *= 60;
        else
            shift *= -60;
        if (! list[12].isEmpty()) {
            const QString tz = list[12].toUpper();
            if (tz == "UT" || tz == "GMT")
                shift = 0;
            else if (tz == "EST")
                shift = 5 * 3600;
            else if (tz == "EDT")
                shift = 4 * 3600;
            else if (tz == "CST")
                shift = 6 * 3600;
            else if (tz == "CDT")
                shift = 5 * 3600;
            else if (tz == "MST")
                shift = 7 * 3600;
            else if (tz == "MDT")
                shift = 6 * 3600;
            else if (tz == "PST")
                shift = 8 * 3600;
            else if (tz == "PDT")
                shift = 7 * 3600;
            else if (tz.size() == 1)
                shift = 0;
            else
                break;
        }
        QDateTime date(QDate(year, month, day), QTime(hours, minutes, seconds), Qt::UTC);
        date = date.addSecs(shift);
        return date;
    } while(0);

    return QDateTime();
}

}
