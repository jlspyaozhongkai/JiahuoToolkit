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
    this->layout->setMargin(0);     //不留边距
    this->layout->setSpacing(0);    //控件间也没距离
    this->setLayout(this->layout);

    //控件在setWidget的时候会插进来

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
    top_layout->setMargin(0);       //不留边距
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

ConvertDialog::ConvertDialog(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "ConvertDialog::ConvertDialog";

    //
    this->setWindowTitle("Converter");
    this->setMinimumWidth(800); //设定了最小的宽度

    //
    auto top_layout = new QVBoxLayout(this);
    top_layout->setMargin(0);   //只有这一个控件，占满整个窗体，所以不留边距
    this->setLayout(top_layout);

    //
    auto top_scroll = new QScrollArea(this);
    top_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    top_scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);  //把QScrollArea撑满整个ConvertDialog
    top_scroll->setWidgetResizable(true);   //里边的ConvertInner会自动撑大
    top_layout->addWidget(top_scroll);

    //
    auto top_widget = new ConvertInner(this);
    top_scroll->setWidget(top_widget);

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
