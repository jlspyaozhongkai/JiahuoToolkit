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

    //this->setMinimumHeight(400);

    auto toplayout = new QVBoxLayout(this);
    toplayout->setMargin(1);
    toplayout->setSpacing(1);
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

    //方框
    auto context_box = new QWidget(this);
    context_box->setStyleSheet("border:1px solid red");
    top_layout->addWidget(context_box);

    //添加
    this->addnew = new QPushButton("+", this);
    top_layout->addWidget(this->addnew);

    //方框内
    auto content_layout = new QVBoxLayout(this);
    content_layout->setMargin(0);
    content_layout->setSpacing(0);
    context_box->setLayout(content_layout);

    //Title
    this->title_label = new QLabel("", this);
    content_layout->addWidget(this->title_label, 0, Qt::AlignHCenter);

    //Title 下方
    auto del_coder_layout = new QHBoxLayout(this);
    del_coder_layout->setMargin(0);
    del_coder_layout->setSpacing(0);
    content_layout->addLayout(del_coder_layout);

    //左面 删除按钮
    this->delcur = new QPushButton("Del", this);
    this->delcur->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    del_coder_layout->addWidget(this->delcur);

    //右边 Box
    this->coder_layout = new QHBoxLayout(this);
    this->coder_layout->setMargin(0);
    this->coder_layout->setSpacing(0);
    del_coder_layout->addLayout(this->coder_layout);

    return;
}

void CoderBox::setCoder(Coder *coder)
{
    if (this->coder != NULL) {
        Q_ASSERT(true);
        return;
    }
    this->coder = coder;

    this->coder_layout->addWidget(coder);
    this->title_label->setText(coder->name());
    this->title_label->setToolTip(coder->desc());

    return;
}

//内部为主要实现，外部实现滚动条功能
ConvertInner::ConvertInner(QWidget *parent)
    : QWidget(parent)
{
    auto top_layout = new QVBoxLayout(this);
    top_layout->setMargin(0);
    top_layout->setSpacing(0);
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
