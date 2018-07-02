#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QSplitter>
#include <QTextEdit>
#include <QDebug>
#include "convert.h"

Coder::Coder(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedHeight(700);

    auto toplayout = new QVBoxLayout(this);
    this->setLayout(toplayout);

    auto textedit = new QTextEdit(this);
    toplayout->addWidget(textedit);
    textedit->setText("yyyyyyyyyy");

    return;
}

Coder::~Coder()
{
    return;
}

ConvertInner::ConvertInner(QWidget *parent)
    : QWidget(parent)
{
    auto top_layout = new QVBoxLayout(this);
    this->setLayout(top_layout);

    //
    top_layout->addWidget(new Coder(this));
    top_layout->addWidget(new Coder(this));

    return;
}

ConvertInner::~ConvertInner()
{
    return;
}

ConvertDialog::ConvertDialog(QDialog *parent)
    : QDialog(parent)
{
    qDebug() << "ConvertDialog::ConvertDialog";

    this->setWindowTitle("Converter");
    this->setMinimumWidth(800);

    //
    auto top_layout = new QVBoxLayout(this);
    this->setLayout(top_layout);

    //
    auto top_scroll = new QScrollArea(this);
    top_layout->addWidget(top_scroll);
    top_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //
    auto top_widget = new ConvertInner(this);
    top_scroll->setWidget(top_widget);

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
