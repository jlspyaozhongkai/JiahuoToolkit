
#ifndef TOOL_CLIPBOARD_H
#define TOOL_CLIPBOARD_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>

class ClipBoardDialog : public QDialog
{
    Q_OBJECT
public:
    static void launch();
public:
    ClipBoardDialog(QDialog *parent = 0);
    ~ClipBoardDialog();
public:
    void flush();    //刷新
private:
    QLabel *textAvalable;
    QTextEdit *textShow;
};

#endif //CLIPBOARD

