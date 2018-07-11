
#ifndef TOOL_CONVERT_CODER_URL_H
#define TOOL_CONVERT_CODER_URL_H

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
};

#endif //TOOL_CONVERT_CODER_URL_H

