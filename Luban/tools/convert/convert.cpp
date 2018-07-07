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
    toplayout->setMargin(1);
    toplayout->setSpacing(1);
    this->setLayout(toplayout);

    //加入tabbar
    this->tabwidget = new QTabWidget(this);
    toplayout->addWidget(this->tabwidget);

    //HexView
    this->hex_widget = new QWidget(this);
    this->tabwidget->addTab(hex_widget, "Hex");

    auto hex_layout = new QVBoxLayout(this);
    hex_layout->setMargin(1);
    hex_layout->setSpacing(1);
    hex_widget->setLayout(hex_layout);

    this->hex_edit = new QTextEdit(this);
    this->hex_edit->setReadOnly(true);
    this->hex_edit->setLineWrapMode(QTextEdit::NoWrap);
    QFont adjust_font(this->hex_edit->font());
    this->hex_edit->setFont(adjust_font);
    hex_layout->addWidget(this->hex_edit);

    //UTF-8
    this->utf8_widget = new QWidget(this);
    this->tabwidget->addTab(utf8_widget, "UTF-8");

    auto utf8_layout = new QVBoxLayout(this);
    utf8_layout->setMargin(1);
    utf8_layout->setSpacing(1);
    utf8_widget->setLayout(utf8_layout);

    this->utf8_edit = new QTextEdit(this);
    this->utf8_edit->setReadOnly(true);
    utf8_layout->addWidget(this->utf8_edit);

    return;
}

void DataView::setData(CodeData *data)
{
    if (data == NULL) {
        this->hex_edit->setText("");
        this->utf8_edit->setText("");
        return;
    }

    //Hex
    QString lines;      //所有行的集合
    QString line_hex;   //每行的hex部分
    QString line_str;   //后边的字符部分
    int length = data->m_buf.length();
    int iloop;
    for (iloop = 0; iloop < length; iloop++) {
        char ch = data->m_buf.at(iloop);
        line_hex += QString::asprintf("%02X ", ch);
        QChar qch(ch);
        if (qch.isPrint()) {
            line_str += QString::asprintf("%c", ch);
        } else {
            line_str += QString::asprintf(".");
        }
        if ((iloop + 1) % 8 == 0) {
            line_hex += " ";
        }
        if ((iloop + 1) % 16 == 0 || iloop + 1 == length) { //16个byte一行 一行结束 或者 整个数组结束
            if (iloop + 1 == length && (iloop + 1) % 16 != 0) {
                int remain = 16 - 1 - iloop % 16;
                int jloop;
                for (jloop = 0; jloop < remain; jloop++) {
                    line_hex += "  ";
                    line_str += " ";
                }
                if (remain > 8) {
                    line_hex += " ";
                }
            }
            lines += QString::asprintf("%08X    ", iloop / 16 * 16);
            lines += line_hex;
            lines += "   ";
            lines += QString("|") + line_str + "|\n";

            line_hex.clear();
            line_str.clear();
        }
    }
    this->hex_edit->setText(lines);

    //UTF-8
    this->utf8_edit->setText(QString::fromUtf8(data->m_buf));

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

    auto file_input_top_layout = new QVBoxLayout(this);
    file_input_top_layout->setSpacing(0);
    file_input_widget->setLayout(file_input_top_layout);

    auto file_input_layout = new QHBoxLayout(this);
    file_input_layout->setMargin(0);
    file_input_layout->setSpacing(0);
    file_input_top_layout->addLayout(file_input_layout);

    file_input_layout->addWidget(new QLabel("File Path:", this));

    this->file_path = new QLineEdit(this);
    file_input_layout->addWidget(this->file_path);

    auto file_input_browse = new QPushButton("Browse", this);
    file_input_browse->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    file_input_layout->addWidget(file_input_browse);

    auto file_input_reload = new QPushButton("Reload", this);
    file_input_reload->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    file_input_layout->addWidget(file_input_reload);

    this->file_err = new QLabel("", this);
    this->file_err->setObjectName("file_input_err");
    this->file_err->setStyleSheet("#file_input_err{color:red;}");
    file_input_top_layout->addWidget(this->file_err);

    //下部显示区 (用现成的 DataView)
    this->data_view = new DataView(this);
    top_split->addWidget(this->data_view);

    //文本变化的时候，刷新文本输入
    connect(this->text_edit, &QTextEdit::textChanged, [this]{
        this->flushText();
    });

    //路径变化（包括browse）或者reload的时候，刷新文件输入
    connect(file_input_browse, &QPushButton::pressed, [this]{
        QString path = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), "File (*.*)");
        this->file_path->setText(path);
    });
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
    Q_UNUSED(input);
    return this->m_data;
}

void CoderInput::flushText()
{
    qDebug() << "CoderInput::flushText";

    //文本统一使用utf8
    this->m_data.m_buf.clear();
    this->m_data.m_buf.append(this->text_edit->toPlainText().toUtf8());
    this->m_data.m_type = CodeData::TYPE_TEXT_UTF8;

    this->data_view->setData(&this->m_data);
    return;
}

void CoderInput::flushFile()
{
    qDebug() << "CoderInput::flushFile";

    this->file_err->setText("");
    this->m_data.m_buf.clear();
    this->m_data.m_type = CodeData::TYPE_NONE;
    this->data_view->setData(&this->m_data);

    //加载文件
    auto filepath = this->file_path->text();
    if (filepath == "") {
        return;
    }
    QFile file(filepath);
    if (! file.open(QFile::ReadOnly)) {
        this->file_err->setText(file.errorString());
        return;
    }
    auto filedata = file.readAll();
    file.close();

    this->m_data.m_buf.append(filedata);
    this->m_data.m_type = CodeData::TYPE_BYTES;

    this->data_view->setData(&this->m_data);

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
