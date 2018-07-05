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
    m_name = "Test1";
    m_desc = "Desc1";

    this->setMinimumHeight(400);

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
    auto top_layout = new QVBoxLayout(this);
    top_layout->setMargin(0);     //不留边距
    top_layout->setSpacing(0);    //控件间也没距离
    this->setLayout(top_layout);

    //Title
    auto title_layout = new QHBoxLayout(this);
    title_layout->setSpacing(0);
    top_layout->addLayout(title_layout);

    this->title_label = new QLabel(this);
    title_layout->addWidget(this->title_label, 0, Qt::AlignHCenter);

    //Content
    this->content_layout = new QVBoxLayout(this);
    this->content_layout->setMargin(0);     //不留边距
    this->content_layout->setSpacing(0);    //控件间也没距离
    top_layout->addLayout(this->content_layout);

    //底部线条 & 样式
    auto borderline = new QWidget(this);
    top_layout->addWidget(borderline);

    QPalette pal(borderline->palette());
    pal.setColor(QPalette::Background, Qt::black);
    borderline->setPalette(pal);
    borderline->setAutoFillBackground(true);
    borderline->setFixedHeight(3);

    return;
}

void CoderBox::setCoder(Coder *coder)
{
    if (this->coder == NULL) {
        return;
    }
    this->coder = coder;

    this->content_layout->addWidget(coder);
    this->title_label->setText(coder->name());
    this->title_label->setToolTip(coder->desc());

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
    auto box = new CoderBox(this);
    top_layout->addWidget(box);

    auto coder = new Coder(this);
    box->setCoder(coder);

    //
    box = new CoderBox(this);
    top_layout->addWidget(box);

    coder = new Coder(this);
    box->setCoder(coder);

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
