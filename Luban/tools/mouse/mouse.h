
#ifndef TOOL_MOUSE_H
#define TOOL_MOUSE_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QTimer>

class MouseDialog : public QWidget
{
    Q_OBJECT
public:
    static void launch();
public:
    MouseDialog(QWidget *parent = 0);
    ~MouseDialog() {}
private:
    QLineEdit *m_showx = NULL;
    QLineEdit *m_showy = NULL;
};

#endif //TOOL_MOUSE_H


