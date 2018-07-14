
#ifndef TOOL_HOST_H
#define TOOL_HOST_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QTableWidget>
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

    QTableWidgetItem *m_item = NULL;

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
    void snapListSelect(int row);
    void snapContentChanged();
    void snapNew();
    void snapDel();
    void snapApply();
    void snapOk();
    void snapCancel();

    void flushSnap(int row);

    QString getHost();
    void setHost(QString txt);

    QTableWidget *m_snap_list = NULL;
    QTextEdit *m_snap_edit = NULL;

    QPushButton *m_snap_new = NULL;
    QPushButton *m_snap_del = NULL;
    QPushButton *m_snap_apply = NULL;
    QPushButton *m_snap_ok = NULL;
    QPushButton *m_snap_cancel = NULL;
};

#endif //TOOL_HOST_H
