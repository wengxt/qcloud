#include <QVBoxLayout>

#include "authdialog.h"
#include "authwidget.h"

namespace QCloud
{
AuthDialog::AuthDialog (AuthWidget* widget, QWidget* parent, Qt::WindowFlags f) : QDialog (parent, f)
{
    widget->setParent (this);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget (widget);
    setLayout (layout);
    connect (widget, SIGNAL (authFinished (bool)), SLOT (authFinished (bool)));

    widget->startAuth();
}

AuthDialog::~AuthDialog()
{

}

void AuthDialog::authFinished (bool success)
{
    if (success)
        accept();
    else
        reject();
}


}
