
#ifndef TOOL_CLIPBOARD_H
#define TOOL_CLIPBOARD_H

#include <QWidget>
#include <QDialog>

class ClipBoardDialog : public QDialog
{
    Q_OBJECT
public:
    static void launch();
public:
    ClipBoardDialog(QDialog *parent = 0);
    ~ClipBoardDialog();
private:

};

#endif //CLIPBOARD

