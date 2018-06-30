
#ifndef TOOL_ASCII_H
#define TOOL_ASCII_H

#include <QWidget>
#include <QDialog>

class AsciiDialog : public QDialog
{
    Q_OBJECT
public:
    static void launch();
public:
    AsciiDialog(QDialog *parent = 0);
    ~AsciiDialog();
private:

};

#endif //TOOL_ASCII_H

