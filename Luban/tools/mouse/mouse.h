
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
    ~MouseDialog() {
        if (this->m_timer != NULL) {
            delete this->m_timer;
            this->m_timer = NULL;
        }
    }
private:
    QLineEdit *m_showx = NULL;
    QLineEdit *m_showy = NULL;

    QTimer *m_timer = NULL;
};

#endif //TOOL_MOUSE_H


