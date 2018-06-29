
#ifndef TOOL_ASCII_H
#define TOOL_ASCII_H

#include <QWidget>
#include <QDialog>

class AsciiWidget : public QDialog
{
    Q_OBJECT
public:
    static void launch();
public:
    AsciiWidget(QDialog *parent = 0);
    ~AsciiWidget();
private:

};

#endif //TOOL_ASCII_H

