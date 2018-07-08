#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QSplitter>
#include <QTextEdit>
#include <QTabWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QTreeWidget>
#include <QHeaderView>
#include <QDir>
#include <QStringList>
#include <QLayoutItem>
#include <QTextCodec>
#include <QVariant>
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
    this->tabwidget->addTab(this->hex_widget, "Hex");
    this->tabwidget->setTabToolTip(this->tabwidget->indexOf(this->hex_widget), "Hex");

    auto hex_layout = new QVBoxLayout(this);
    hex_layout->setMargin(1);
    hex_layout->setSpacing(1);
    this->hex_widget->setLayout(hex_layout);

    this->hex_edit = new QTextEdit(this);
    this->hex_edit->setReadOnly(true);
    this->hex_edit->setLineWrapMode(QTextEdit::NoWrap);
    QFont adjust_font(this->hex_edit->font());
    adjust_font.setFamily("Courier");       //等宽字体
    this->hex_edit->setFont(adjust_font);
    hex_layout->addWidget(this->hex_edit);

    //ASCII
    this->ascii_widget = new QWidget(this);
    this->tabwidget->addTab(this->ascii_widget, "ASCII");
    this->tabwidget->setTabToolTip(this->tabwidget->indexOf(this->ascii_widget), "US-ASCII/Latin-1");

    auto ascii_layout = new QVBoxLayout(this);
    ascii_layout->setMargin(1);
    ascii_layout->setSpacing(1);
    this->ascii_widget->setLayout(ascii_layout);

    this->ascii_edit = new QTextEdit(this);
    this->ascii_edit->setReadOnly(true);
    ascii_layout->addWidget(this->ascii_edit);

    //GBK
    this->gbk_widget = new QWidget(this);
    this->tabwidget->addTab(this->gbk_widget, "GBK");
    this->tabwidget->setTabToolTip(this->tabwidget->indexOf(this->gbk_widget), "GBK/GB18030");

    auto gbk_layout = new QVBoxLayout(this);
    gbk_layout->setMargin(1);
    gbk_layout->setSpacing(1);
    this->gbk_widget->setLayout(gbk_layout);

    this->gbk_edit = new QTextEdit(this);
    this->gbk_edit->setReadOnly(true);
    gbk_layout->addWidget(this->gbk_edit);

    //UTF-8
    this->utf8_widget = new QWidget(this);
    this->tabwidget->addTab(this->utf8_widget, "UTF-8");
    this->tabwidget->setTabToolTip(this->tabwidget->indexOf(this->utf8_widget), "UTF-8");

    auto utf8_layout = new QVBoxLayout(this);
    utf8_layout->setMargin(1);
    utf8_layout->setSpacing(1);
    this->utf8_widget->setLayout(utf8_layout);

    this->utf8_edit = new QTextEdit(this);
    this->utf8_edit->setReadOnly(true);
    utf8_layout->addWidget(this->utf8_edit);

    //UTF-16
    this->utf16_widget = new QWidget(this);
    this->tabwidget->addTab(this->utf16_widget, "UTF-16");
    this->tabwidget->setTabToolTip(this->tabwidget->indexOf(this->utf16_widget), "UTF-16");

    auto utf16_layout = new QVBoxLayout(this);
    utf16_layout->setMargin(1);
    utf16_layout->setSpacing(1);
    this->utf16_widget->setLayout(utf16_layout);

    this->utf16_edit = new QTextEdit(this);
    this->utf16_edit->setReadOnly(true);
    utf16_layout->addWidget(this->utf16_edit);

    //UTF-32
    this->utf32_widget = new QWidget(this);
    this->tabwidget->addTab(this->utf32_widget, "UTF-32");
    this->tabwidget->setTabToolTip(this->tabwidget->indexOf(this->utf32_widget), "UTF-32");

    auto utf32_layout = new QVBoxLayout(this);
    utf32_layout->setMargin(1);
    utf32_layout->setSpacing(1);
    this->utf32_widget->setLayout(utf32_layout);

    this->utf32_edit = new QTextEdit(this);
    this->utf32_edit->setReadOnly(true);
    utf32_layout->addWidget(this->utf32_edit);

    //Others
    this->others_widget = new QWidget(this);
    this->tabwidget->addTab(this->others_widget, "Others");
    this->tabwidget->setTabToolTip(this->tabwidget->indexOf(this->others_widget), "Others with options");

    auto others_layout = new QVBoxLayout(this);
    others_layout->setMargin(1);
    others_layout->setSpacing(1);
    this->others_widget->setLayout(others_layout);

    QStringList code_list;
    auto support_codes = QTextCodec::availableCodecs();
    int iloop;
    for (iloop = 0; iloop < support_codes.size(); iloop++) {
        auto support_code = support_codes.at(iloop);
        code_list << QString::fromLatin1(support_code);
    }
    code_list.sort();
    this->others_code = new QComboBox(this);
    this->others_code->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->others_code->addItems(code_list);
    this->others_code->setCurrentText("UTF-8");
    others_layout->addWidget(this->others_code);

    this->others_edit = new QTextEdit(this);
    this->others_edit->setReadOnly(true);
    others_layout->addWidget(this->others_edit);

    connect(this->others_code, &QComboBox::currentTextChanged, [this]{
        this->setData(& this->m_data);
    });

    return;
}

void DataView::setData(CodeData *input)
{
    this->hex_edit->setText("");
    this->ascii_edit->setText("");
    this->gbk_edit->setText("");
    this->utf8_edit->setText("");
    this->utf16_edit->setText("");
    this->utf32_edit->setText("");
    this->others_edit->setText("");

    if (input == NULL) {
        this->m_data.m_buf.clear();
        this->m_data.m_type = CodeData::TYPE_NONE;

        return;
    }

    this->m_data.m_buf = input->m_buf;
    this->m_data.m_type = input->m_type;

    //Hex
    QString lines;      //所有行的集合
    QString line_hex;   //每行的hex部分
    QString line_str;   //后边的字符部分
    int length = this->m_data.m_buf.length();
    int iloop;
    for (iloop = 0; iloop < length; iloop++) {
        uchar ch = this->m_data.m_buf.at(iloop);
        line_hex += QString::asprintf("%02X ", ch);
        QChar qch(ch);
        if (qch.isPrint()) {
            line_str += QString::asprintf("%c", ch);
        } else {
            line_str += QString::asprintf(".");
        }
        if ((iloop + 1) % 16 == 7) {
            line_hex += " ";
        }
        if ((iloop + 1) % 16 == 0 || iloop + 1 == length) { //16个byte一行 一行结束 或者 整个数组结束
            if (iloop + 1 == length && (iloop + 1) % 16 != 0) {
                int remain = 16 - 1 - iloop % 16;
                int jloop;
                for (jloop = 0; jloop < remain; jloop++) {
                    line_hex += "   ";  //"___";
                    line_str += " ";    //"_";
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

    //ASCII
    this->ascii_edit->setText(QString::fromLatin1(this->m_data.m_buf));

    //GBK
    auto gbk_decoder = QTextCodec::codecForName("GB18030");
    this->gbk_edit->setText(gbk_decoder->toUnicode(this->m_data.m_buf));

    //UTF-8
    auto utf8_decoder = QTextCodec::codecForName("UTF-8");
    this->utf8_edit->setText(utf8_decoder->toUnicode(this->m_data.m_buf));

    //UTF-16
    auto utf16_decoder = QTextCodec::codecForName("UTF-16");
    this->utf16_edit->setText(utf16_decoder->toUnicode(this->m_data.m_buf));

    //UTF-32
    auto utf32_decoder = QTextCodec::codecForName("UTF-32");
    this->utf32_edit->setText(utf32_decoder->toUnicode(this->m_data.m_buf));

    //Others
    auto others_cur_text = this->others_code->currentText();
    auto others_decoder = QTextCodec::codecForName(others_cur_text.toLatin1());
    this->others_edit->setText(others_decoder->toUnicode(this->m_data.m_buf));

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

    //TODO:这里有 图方便代码，按照加入tab的顺序识别标签
    connect(inputtab, &QTabWidget::currentChanged, [this](int index){
        if (index == 0) {
            this->flushText();
        } else if (index == 1) {
            this->flushFile();
        }
    });

    //文本变化的时候，刷新文本输入
    connect(this->text_edit, &QTextEdit::textChanged, [this]{
        this->flushText();
    });

    //浏览文件路径
    connect(file_input_browse, &QPushButton::pressed, [this]{
        QString path = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), "File (*.*)");
        this->file_path->setText(path);
    });
    //路径变化时，刷新文件输入
    connect(file_input_reload, &QPushButton::pressed, [this]{
        this->flushFile();
    });
    connect(this->file_path, &QLineEdit::textChanged, [this]{
        this->flushFile();
    });

    return;
}

CodeData CoderInput::flushChain(CodeData input)
{
    Q_UNUSED(input);    //不处理输入
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

    emit this->signalChanged();
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
    if (filepath != "") {
        QFile file(filepath);
        if (! file.open(QFile::ReadOnly)) {
            this->file_err->setText(file.errorString());
            return;
        }
        auto filedata = file.readAll();
        file.close();

        this->m_data.m_buf.append(filedata);
        this->m_data.m_type = CodeData::TYPE_BYTES;
    }

    this->data_view->setData(&this->m_data);

    emit this->signalChanged();
    return;
}

CoderUrlEncode::CoderUrlEncode(QWidget *parent)
    : Coder(parent)
{
    this->setName("URL Encode");
    this->setDesc("URL Encode");

    auto top_layout = new QVBoxLayout(this);
    top_layout->setMargin(1);
    top_layout->setSpacing(1);
    this->setLayout(top_layout);

    this->data_view = new DataView(this);
    top_layout->addWidget(this->data_view);

    return;
}

CodeData CoderUrlEncode::flushChain(CodeData input)
{
    Q_UNUSED(input);
    CodeData xxx;
    return xxx;
}

CoderUrlDecode::CoderUrlDecode(QWidget *parent)
    : Coder(parent)
{
    this->setName("URL Decode");
    this->setDesc("URL Decode");

    auto top_layout = new QVBoxLayout(this);
    top_layout->setMargin(1);
    top_layout->setSpacing(1);
    this->setLayout(top_layout);

    this->data_view = new DataView(this);
    top_layout->addWidget(this->data_view);

    return;
}

CodeData CoderUrlDecode::flushChain(CodeData input)
{
    Q_UNUSED(input);
    CodeData xxx;
    return xxx;
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

    connect(this->addnew, &QPushButton::pressed, [this]{
        qDebug() << "CoderBox::Add";
        emit this->signalAdd();
    });

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

    connect(this->delcur, &QPushButton::pressed, [this]{
        qDebug() << "CoderBox::Del";
        emit this->signalDel();
    });

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

WhichCoder::WhichCoder(QDialog *parent)
    : QDialog(parent)
{
    auto top_layout = new QVBoxLayout(this);
    this->setLayout(top_layout);

    //
    top_layout->addWidget(new QLabel("请选择转换器", this));

    //
    auto coder_tree = new QTreeWidget(this);
    coder_tree->header()->setVisible(false);
    top_layout->addWidget(coder_tree);

    //设置树节点
    auto url_node = new QTreeWidgetItem();
    url_node->setText(0, "URL Encode/Decode");
    url_node->setData(1, 0, QVariant::fromValue((void *)NULL));
    coder_tree->addTopLevelItem(url_node);

    auto url_encode = new QTreeWidgetItem();
    url_encode->setText(0, "URL Encode");
    url_encode->setData(1, 0, QVariant::fromValue((void *)CoderUrlEncode::make));
    url_node->addChild(url_encode);

    auto url_decode = new QTreeWidgetItem();
    url_decode->setText(0, "URL Decode");
    url_decode->setData(1, 0, QVariant::fromValue((void *)CoderUrlDecode::make));
    url_node->addChild(url_decode);

    //
    auto btn_layout = new QHBoxLayout(this);
    top_layout->addLayout(btn_layout);

    btn_layout->addStretch();   //加弹簧

    auto ok_btn = new QPushButton("确定", this);
    ok_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btn_layout->addWidget(ok_btn);

    auto cancel_btn = new QPushButton("取消", this);
    cancel_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btn_layout->addWidget(cancel_btn);

    //
    ok_btn->setEnabled(false);  //默认 disable
    connect(coder_tree, &QTreeWidget::currentItemChanged, [ok_btn](QTreeWidgetItem *current, QTreeWidgetItem *previous){
        Q_UNUSED(previous);
        if (current == NULL) {
            return;
        }
        void *data = current->data(1, 0).value<void *>();
        if (data == NULL) {
            ok_btn->setEnabled(false);
        } else {
            ok_btn->setEnabled(true);
        }
    });

    connect(ok_btn, &QPushButton::pressed, [this, coder_tree, ok_btn]{
        auto current = coder_tree->currentItem();
        if (current == NULL) {
            return;
        }
        void *data = current->data(1, 0).value<void *>();
        if (data == NULL) {
            ok_btn->setEnabled(false);
            return;
        }
        this->m_coder = ((Coder *(*)())data)();
        this->close();
    });

    connect(cancel_btn, &QPushButton::pressed, [this]{
        this->close();
    });

    return;
}

Coder *WhichCoder::getCoder()
{
    qDebug() << "WhichCoder::getCoder start";
    Coder *ret_val = NULL;

    auto whichcoder = new WhichCoder();
    whichcoder->setModal(true);
    whichcoder->exec();     //会等待WhichCoder关闭
    ret_val = whichcoder->m_coder;
    delete whichcoder;

    if (ret_val == NULL) {
        qDebug() << "WhichCoder::getCoder failed";
    } else {
        qDebug() << "WhichCoder::getCoder finish: " << ret_val->name() << " " << ret_val->desc();
    }

    return ret_val;
}

//内部为主要实现，外部实现滚动条功能
ConvertInner::ConvertInner(QWidget *parent)
    : QWidget(parent)
{
    this->m_boxlist = new QVBoxLayout(this);
    this->m_boxlist->setMargin(0);
    this->m_boxlist->setSpacing(0);
    this->setLayout(this->m_boxlist);

    //安装首个节点
    auto coder = new CoderInput(this);
    this->addCoder(0, coder);

    return;
}

void ConvertInner::addCoder(int index, Coder *coder)
{
    auto box = new CoderBox(this);
    box->setCoder(coder);

    this->m_boxlist->insertWidget(index, box);

    //
    connect(box, SIGNAL(signalAdd()), this, SLOT(slotBoxAdd()));
    connect(box, SIGNAL(signalDel()), this, SLOT(slotBoxDel()));

    //
    connect(coder, SIGNAL(signalChanged()), this, SLOT(slotCoderChanged()));

    return;
}

void ConvertInner::slotBoxAdd()
{
    qDebug() << "ConvertInner::slotBoxAdd";

    auto coder = WhichCoder::getCoder();
    if (coder == NULL) {
        qDebug() << "Get coder failed";
        return;
    }

    CoderBox *box = qobject_cast<CoderBox*>(sender());
    if (box == NULL) {
        qDebug() << "Get previous box failed";
        return;
    }

    int box_index = this->m_boxlist->indexOf(box);  //success forever
    qDebug() << "Insert box at" << box_index;

    this->addCoder(box_index + 1, coder);

    this->slotCoderChanged();
    return;
}

void ConvertInner::slotBoxDel()
{
    qDebug() << "ConvertInner::slotBoxDel";

    CoderBox *box = qobject_cast<CoderBox*>(sender());
    if (box == NULL) {
        qDebug() << "Get current box failed";
        return;
    }

    int box_index = this->m_boxlist->indexOf(box);  //success forever
    qDebug() << "Remove box at" << box_index;

    if (box_index == 0) {
        return;
    }

    this->m_boxlist->removeWidget(box);
    box->deleteLater();

    this->slotCoderChanged();
    return;
}

void ConvertInner::slotCoderChanged()
{
    qDebug() << "ConvertInner::slotCoderChanged";
    //任何一个Coder变化了都从头开始刷新

    int count = this->m_boxlist->count();
    int iloop;
    for (iloop = 0; iloop < count; iloop++) {
        auto widget = this->m_boxlist->itemAt(iloop)->widget();
        if (widget == NULL) {
            continue;
        }
        CoderBox *box = qobject_cast<CoderBox*>(widget);
        if (box == NULL) {
            continue;
        }
        Coder *coder = box->getCoder();
        qDebug() << ">>>" << coder->name();
    }

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
