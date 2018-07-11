
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
    ret_val.m_type = CodeData::TYPE_BYTES;

    //扫描输入
    int count = input.m_buf.size();
    int iloop;
    for (iloop = 0; iloop < count; iloop++) {
        uchar ch = input.m_buf.at(iloop);
        if (ch == '%' && iloop + 2 < count) {
                bool wrong = false;

                uchar d_high = input.m_buf.at(iloop + 1);
                if (d_high >= '0' && d_high <= '9') {
                    d_high = d_high - '0';
                } else if (d_high >= 'A' && d_high <= 'F') {
                    d_high = d_high - 'A' + 10;
                } else if (d_high >= 'a' && d_high <= 'f') {
                    d_high = d_high - 'a' + 10;
                } else {
                    wrong = true;
                }

                uchar d_low = input.m_buf.at(iloop + 2);
                if (d_low >= '0' && d_low <= '9') {
                    d_low = d_low - '0';
                } else if (d_low >= 'A' && d_low <= 'F') {
                    d_low = d_low - 'A' + 10;
                } else if (d_low >= 'a' && d_low <= 'f') {
                    d_low = d_low - 'a' + 10;
                } else {
                    wrong = true;
                }

                if (! wrong) {
                    uchar data = d_high * 16 + d_low;
                    ret_val.m_buf.append(data);    //解码字符
                    iloop = iloop + 2;             //iloop + 2已经被处理
                    continue;
                }
        }
        ret_val.m_buf.append(ch);   //普通字符
    }

    this->data_view->setData(&ret_val);
    return ret_val;
}
