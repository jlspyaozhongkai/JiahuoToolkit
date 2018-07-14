
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
#include <QList>
#include <QMap>

class HostSnap {
public:
    HostSnap() {}
    ~HostSnap() {
        qDebug() << "delete HostSnap";
    }

    QListWidgetItem *m_item = NULL;

    QString m_name;
    QString m_editing;
    QString m_saving;
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

    QListWidget *m_snap_list = NULL;
    QPushButton *m_snap_list_new = NULL;
    QPushButton *m_snap_list_del = NULL;


    QTextEdit *m_snap_edit = NULL;
    QPushButton *m_snap_edit_apply = NULL;
    QPushButton *m_snap_edit_ok = NULL;
    QPushButton *m_snap_edit_cancel = NULL;
};

#endif //TOOL_HOST_H
