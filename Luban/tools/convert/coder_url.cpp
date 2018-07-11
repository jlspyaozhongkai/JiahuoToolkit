
#include <QDebug>
#include "convert.h"
#include "coder_url.h"

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

CodeData CoderUrlEncode::flushChain(CodeData &input)
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

CodeData CoderUrlDecode::flushChain(CodeData &input)
{
    CodeData ret_val;

    //

    return ret_val;
}
