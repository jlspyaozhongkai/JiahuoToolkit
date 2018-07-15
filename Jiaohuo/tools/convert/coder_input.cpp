
#include <QFileDialog>
#include <QDebug>
#include "convert.h"
#include "coder_input.h"

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

CodeData CoderInput::flushChain(CodeData &input)
{
    //CoderInput节点特殊，节点将输入的数据缓存，然后不处理输入，直接从数据中输出
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
