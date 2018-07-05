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
    this->setFixedHeight(400);

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

//为Coder提供大小调整功能
CoderBox::CoderBox(QWidget *parent)
    : QWidget(parent)
{
    this->layout = new QVBoxLayout(this);
    this->setLayout(this->layout);

    //底部线条
    auto borderline = new QWidget(this);
    this->layout->addWidget(borderline);

    //底部线条 样式
    QPalette pal(borderline->palette());
    pal.setColor(QPalette::Background, Qt::black);
    borderline->setPalette(pal);
    borderline->setAutoFillBackground(true);
    borderline->setFixedHeight(3);

    return;
}

CoderBox::~CoderBox()
{
    return;
}

void CoderBox::setWidget(QWidget *widget)
{
    this->layout->insertWidget(0, widget);
    return;
}

//内部为主要实现，外部实现滚动条功能
ConvertInner::ConvertInner(QWidget *parent)
    : QWidget(parent)
{
    auto top_layout = new QVBoxLayout(this);
    this->setLayout(top_layout);

    //TODO
    auto coder = new Coder(this);
    auto box = new CoderBox(this);
    box->setWidget(coder);
    top_layout->addWidget(box);

    coder = new Coder(this);
    box = new CoderBox(this);
    box->setWidget(coder);
    top_layout->addWidget(box);

    return;
}

ConvertInner::~ConvertInner()
{
    return;
}

ConvertDialog::ConvertDialog(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "ConvertDialog::ConvertDialog";

    this->setWindowTitle("Converter");
    this->setMinimumWidth(800);

    //
    auto top_layout = new QVBoxLayout(this);
    this->setLayout(top_layout);

    //
    auto top_scroll = new QScrollArea(this);
    top_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    top_scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    top_layout->addWidget(top_scroll);

    //
    auto top_widget = new ConvertInner(this);
    top_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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

    win->show();
    win->setFocus();

    return;
}
