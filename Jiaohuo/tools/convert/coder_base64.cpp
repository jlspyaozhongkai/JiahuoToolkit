
#include <QDebug>
#include "convert.h"
#include "coder_base64.h"

CoderBase64Encode::CoderBase64Encode(QWidget *parent)
    : Coder(parent)
{
    this->setName("Base64 Encode");
    this->setDesc("Base64 Encode");

    auto top_layout = new QVBoxLayout(this);
    top_layout->setMargin(1);
    top_layout->setSpacing(1);
    this->setLayout(top_layout);

    this->data_view = new DataView(this);
    top_layout->addWidget(this->data_view);

    return;
}

CodeData CoderBase64Encode::flushChain(CodeData &input)
{
    CodeData ret_val;
    ret_val.m_type = CodeData::TYPE_BYTES;
    ret_val.m_buf = input.m_buf.toBase64();
    this->data_view->setData(&ret_val);
    return ret_val;
}


CoderBase64Decode::CoderBase64Decode(QWidget *parent)
    : Coder(parent)
{
    this->setName("Base64 Decode");
    this->setDesc("Base64 Decode");

    auto top_layout = new QVBoxLayout(this);
    top_layout->setMargin(1);
    top_layout->setSpacing(1);
    this->setLayout(top_layout);

    this->data_view = new DataView(this);
    top_layout->addWidget(this->data_view);

    return;
}

CodeData CoderBase64Decode::flushChain(CodeData &input)
{
    CodeData ret_val;
    ret_val.m_type = CodeData::TYPE_BYTES;
    ret_val.m_buf = QByteArray::fromBase64(input.m_buf);
    this->data_view->setData(&ret_val);
    return ret_val;
}
