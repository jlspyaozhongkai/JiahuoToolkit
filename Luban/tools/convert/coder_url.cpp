
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

    auto select_layout = new QHBoxLayout(this);
    top_layout->addLayout(select_layout);

    this->encode_all = new QRadioButton("对所有字符编码", this);
    select_layout->addWidget(this->encode_all);

    this->encode_rfc3986 = new QRadioButton("RFC3986编码", this);
    this->encode_rfc3986->setToolTip("RFC3986中规定 a-z A-Z 0-9 和四个字符-_.~ 以外的其他字符都需要编码，通过选项控制以下分隔符是否编码 !*'();:@&=+$,/?#[] ");
    select_layout->addWidget(this->encode_rfc3986);

    this->encode_noprint = new QRadioButton("对不可打印字符编码", this);
    select_layout->addWidget(this->encode_noprint);

    select_layout->addStretch();

    this->encode_rfc3986->setChecked(true);

    //
    auto option_layout = new QHBoxLayout(this);
    top_layout->addLayout(option_layout);

    this->space_to_plus = new QCheckBox("空格编码为\"+\"", this);
    this->space_to_plus->setCheckState(Qt::Checked);
    option_layout->addWidget(this->space_to_plus);

    option_layout->addSpacing(30);

    this->encode_separator = new QCheckBox("对分隔符编码", this);
    this->encode_separator->setChecked(false);
    this->encode_separator->setEnabled(true);
    option_layout->addWidget(this->encode_separator);

    option_layout->addStretch();

    this->data_view = new DataView(this);
    top_layout->addWidget(this->data_view);

    //
    connect(this->encode_rfc3986, &QRadioButton::toggled, [this]{
        if (this->encode_rfc3986->isChecked()) {
            this->encode_separator->setEnabled(true);
        } else {
            this->encode_separator->setEnabled(false);
        }
    });

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

    auto options_layout = new QHBoxLayout(this);
    top_layout->addLayout(options_layout);

    this->plus_to_space = new QCheckBox("字符\"+\"解码为空格", this);
    this->plus_to_space->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->plus_to_space->setCheckState(Qt::Checked);
    options_layout->addWidget(this->plus_to_space);
    options_layout->addStretch();

    this->data_view = new DataView(this);
    top_layout->addWidget(this->data_view);

    //
    connect(this->plus_to_space, &QCheckBox::stateChanged, [this]{
        emit this->signalChanged();
    });

    return;
}

CodeData CoderUrlDecode::flushChain(CodeData &input)
{
    CodeData ret_val;
    ret_val.m_type = CodeData::TYPE_BYTES;

    bool plus_to_space = (this->plus_to_space->checkState() == Qt::Checked);

    //扫描输入
    int count = input.m_buf.size();
    int iloop;
    for (iloop = 0; iloop < count; iloop++) {
        uchar ch = input.m_buf.at(iloop);
        if (ch == '+' && plus_to_space) {
            ret_val.m_buf.append(' ');
            continue;
        } else if (ch == '%' && iloop + 2 < count) {
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
