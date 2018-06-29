
#include <QDebug>
#include "ascii.h"

static AsciiWidget *singleton = NULL;

AsciiWidget::AsciiWidget(QDialog *parent)
    : QDialog(parent)
{
    return;
}

AsciiWidget::~AsciiWidget()
{
    return;
}

void AsciiWidget::launch()
{
    if (singleton == NULL) {
        singleton = new AsciiWidget();
    }
    AsciiWidget *win = singleton;

    qDebug() << "AsciiWidget::launch";

    win->setModal(false);
    win->show();
    win->setFocus();
    return;
}
