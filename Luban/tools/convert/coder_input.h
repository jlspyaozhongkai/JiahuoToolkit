
#ifndef TOOL_CONVERT_CODER_INPUT_H
#define TOOL_CONVERT_CODER_INPUT_H

#include "convert.h"

//Coder子类，CoderInput
class CoderInput : public Coder
{
    Q_OBJECT
public:
    CoderInput(QWidget *parent = 0);
    ~CoderInput() {}

    CodeData flushChain(CodeData &input);
    static Coder* make() {return new CoderInput();}
private:
    CodeData m_data;

    QTextEdit *text_edit = NULL;

    QLineEdit *file_path = NULL;
    QLabel *file_err = NULL;

    DataView *data_view = NULL;

    void flushText();
    void flushFile();
};

#endif //TOOL_CONVERT_CODER_INPUT_H
