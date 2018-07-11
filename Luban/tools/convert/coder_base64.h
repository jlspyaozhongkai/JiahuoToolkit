
#ifndef TOOL_CONVERT_CODER_BASE64_H
#define TOOL_CONVERT_CODER_BASE64_H

#include "convert.h"

//Coder子类，CoderBase64Encode
class CoderBase64Encode : public Coder
{
    Q_OBJECT
public:
    CoderBase64Encode(QWidget *parent = 0);
    ~CoderBase64Encode() {}

    CodeData flushChain(CodeData &input);
    static Coder* make() {qDebug() << "CoderBase64Encode::make";return new CoderBase64Encode();}
private:
    DataView *data_view = NULL;
};

//Coder子类，CoderBase64Decode
class CoderBase64Decode : public Coder
{
    Q_OBJECT
public:
    CoderBase64Decode(QWidget *parent = 0);
    ~CoderBase64Decode() {}

    CodeData flushChain(CodeData &input);
    static Coder* make() {qDebug() << "CoderBase64Decode::make";return new CoderBase64Decode();}
private:
    DataView *data_view = NULL;
};

#endif //TOOL_CONVERT_CODER_BASE64_H

