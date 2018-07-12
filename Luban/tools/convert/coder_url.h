
#ifndef TOOL_CONVERT_CODER_URL_H
#define TOOL_CONVERT_CODER_URL_H

#include <QCheckBox>
#include <QRadioButton>

#include "convert.h"

//Coder子类，CoderUrlEncode
class CoderUrlEncode : public Coder
{
    Q_OBJECT
public:
    CoderUrlEncode(QWidget *parent = 0);
    ~CoderUrlEncode() {}

    CodeData flushChain(CodeData &input);
    static Coder* make() {qDebug() << "CoderUrlEncode::make";return new CoderUrlEncode();}
private:
    DataView *data_view = NULL;

    QRadioButton *encode_all = NULL;
    QRadioButton *encode_rfc3986 = NULL;
    QRadioButton *encode_noprint = NULL;

    QCheckBox *encode_separator = NULL;  //avaliable on rfc3986
    QCheckBox *space_to_plus = NULL;
};

//Coder子类，CoderUrlDecode
class CoderUrlDecode : public Coder
{
    Q_OBJECT
public:
    CoderUrlDecode(QWidget *parent = 0);
    ~CoderUrlDecode() {}

    CodeData flushChain(CodeData &input);
    static Coder* make() {qDebug() << "CoderUrlDecode::make";return new CoderUrlDecode();}
private:
    DataView *data_view = NULL;

    QCheckBox *plus_to_space = NULL;
};

#endif //TOOL_CONVERT_CODER_URL_H

