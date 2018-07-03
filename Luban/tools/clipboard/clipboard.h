
#ifndef TOOL_CLIPBOARD_H
#define TOOL_CLIPBOARD_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QTabWidget>

class ClipBoardDialog : public QWidget
{
    Q_OBJECT
public:
    static void launch();
public:
    ClipBoardDialog(QWidget *parent = 0);
    ~ClipBoardDialog();
public:
    void flush();    //刷新
private:
    QTabWidget *main_tab;

    QWidget *textTab;
    QTextEdit *textShow;

    QWidget *pixmapTab;
    QLabel *pixmapShow;

    QWidget *imageTab;
    QLabel *imageShow;

    QWidget *mimeTab;
    QTextEdit *mimeShow;
};

#endif //CLIPBOARD

