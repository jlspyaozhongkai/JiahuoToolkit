#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QSplitter>
#include <QTextEdit>
#include <QTabWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include "convert.h"

DataView::DataView(QWidget *parent)
    : QWidget(parent)
{
    auto toplayout = new QVBoxLayout(this);
    this->setLayout(toplayout);

    //加入tabbar
    this->tabwidget = new QTabWidget(this);
    toplayout->addWidget(this->tabwidget);

    //HexView
    this->hex_widget = new QWidget(this);
    this->tabwidget->addTab(hex_widget, "Hex");

    return;
}

void DataView::setData(CodeData *data)
{

    return;
}

Coder::Coder(QWidget *parent)
    : QWidget(parent)
{
    return;
}

CoderInput::CoderInput(QWidget *parent)
    : Coder(parent)
{
    this->setName("Input");
    this->setDesc("Text input or file input");

    auto toplayout = new QVBoxLayout(this);
    toplayout->setMargin(1);
    toplayout->setSpacing(1);
    this->setLayout(toplayout);

    auto top_split = new QSplitter(Qt::Vertical);
    toplayout->addWidget(top_split);

    //上部输入选项区（多种输入，使用Tab）
    auto inputtab = new QTabWidget(this);
    top_split->addWidget(inputtab);

    //Text Input
    auto text_input_widget = new QWidget(this);
    inputtab->addTab(text_input_widget, "Text");

    auto text_input_layout = new QHBoxLayout(this);
    text_input_layout->setMargin(1);
    text_input_layout->setSpacing(0);
    text_input_widget->setLayout(text_input_layout);

    this->text_edit = new QTextEdit(this);
    text_input_layout->addWidget(text_edit);

    //File Input
    auto file_input_widget = new QWidget(this);
    inputtab->addTab(file_input_widget, "File");

    auto file_input_layout = new QHBoxLayout(this);
    file_input_layout->setMargin(1);
    file_input_layout->setSpacing(0);
    file_input_widget->setLayout(file_input_layout);

    file_input_layout->addWidget(new QLabel("File Path:", this));

    this->file_path = new QLineEdit(this);
    file_input_layout->addWidget(this->file_path);

    auto file_input_browse = new QPushButton("Browse", this);
    file_input_browse->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    file_input_layout->addWidget(file_input_browse);

    auto file_input_reload = new QPushButton("Reload", this);
    file_input_reload->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    file_input_layout->addWidget(file_input_reload);

    //下部显示区 (用现成的)
    auto dataview = new DataView(this);
    top_split->addWidget(dataview);

    //文本变化的时候，刷新文本输入
    connect(this->text_edit, &QTextEdit::textChanged, [this]{
        this->flushText();
    });

    //路径变化（包括browse）或者reload的时候，刷新文件输入
    connect(file_input_reload, &QPushButton::pressed, [this]{
        this->flushFile();
    });
    connect(this->file_path, &QLineEdit::textChanged, [this]{
        this->flushFile();
    });

    return;
}

CodeData CoderInput::io(CodeData input)
{
    CodeData xxx;
    return xxx;
}

void CoderInput::flushText()
{
    qDebug() << "CoderInput::flushText";
    return;
}

void CoderInput::flushFile()
{
    qDebug() << "CoderInput::flushFile";
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
    auto context_box = new QFrame(this);
    context_box->setObjectName("context_box");
    context_box->setStyleSheet("#context_box{border: 1px solid black;}");
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

    //默认放一个Input节点。
    auto box = new CoderBox(this);
    top_layout->addWidget(box);

    auto coder = new CoderInput(this);
    box->setCoder(coder);

    return;
}

ConvertDialog::ConvertDialog(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "ConvertDialog::ConvertDialog";

    //
    this->setWindowTitle("Converter");
    this->setMinimumWidth(1000); //设定了最小的宽度

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
