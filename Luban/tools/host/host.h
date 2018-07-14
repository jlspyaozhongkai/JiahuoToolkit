
#ifndef TOOL_HOST_H
#define TOOL_HOST_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QTimer>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    MyListWidget(QWidget *parent = 0)
        : QListWidget(parent)
    {
        return;
    }
    ~MyListWidget() {
        return;
    }
protected:
    bool event(QEvent *event) {
        qDebug() << "List Event:" << event << event->type();
        return QListWidget::event(event);
    }

    //void mouseMoveEvent(QMouseEvent *event)
    //{
    //    return QListWidget::mouseMoveEvent(event);
    //}
    void mousePressEvent(QMouseEvent *event)
    {
        qDebug() << "List mousePressEvent:" << event << event->type();
        return QListWidget::mousePressEvent(event);
    }
    void mouseReleaseEvent(QMouseEvent *event)
    {
        qDebug() << "List mouseReleaseEvent:" << event << event->type();
        return QListWidget::mouseReleaseEvent(event);
    }
};

class HostDialog : public QWidget
{
    Q_OBJECT
public:
    static void launch();
public:
    HostDialog(QWidget *parent = 0);
    ~HostDialog() {}
private:
    void snapSelect(int row);
    void snapNew();
    void snapDel();
    void snapApply();

    MyListWidget *m_snap_list = NULL;
    QPushButton *m_snap_list_new = NULL;
    QPushButton *m_snap_list_del = NULL;
    QPushButton *m_snap_list_apply = NULL;

    QTextEdit *m_snap_edit = NULL;
    QPushButton *m_snap_edit_ok = NULL;
    QPushButton *m_snap_edit_cancel = NULL;
};

#endif //TOOL_HOST_H
