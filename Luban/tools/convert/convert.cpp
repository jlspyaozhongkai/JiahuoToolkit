
#include <QDebug>
#include "convert.h"

ConvertDialog::ConvertDialog(QDialog *parent)
    : QDialog(parent)
{
    qDebug() << "ConvertDialog::ConvertDialog";

    this->setWindowTitle("Converter");
    this->setMinimumWidth(800);

    return;
}

ConvertDialog::~ConvertDialog()
{
    return;
}

void ConvertDialog::launch()
{
    qDebug() << "ConvertDialog::launch";

    ConvertDialog *win = new ConvertDialog();

    win->setModal(false);
    win->show();
    win->setFocus();

    return;
}
