
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
#include <QJsonObject>
#include <QJsonArray>

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

class SnapListTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    SnapListTableWidget(QWidget *parent = 0)
        : QTableWidget(parent)
    {
        return;
    }
    ~SnapListTableWidget() {}
signals:
    void signalDropRow(int from, int to);
protected:
    void dropEvent(QDropEvent *event);
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
    void flushSnap(int row);
    void insertSnap(int row, HostSnap *snap);
    void snapDropRow(int from, int to);

    void snapListRename(int row);
    void snapListSelect(int row);
    void snapContentChanged();
    void snapLoad(QString name, QString content);
    void snapNew();
    void snapDel();
    void snapApply();
    void snapSave();
    void snapCancel();

    void snapLoadConfig(QJsonArray &json);
    QJsonArray snapSaveConfig();

    QString getHost();
    void setHost(QString txt);

    void LoadConfig();
    void saveConfig();

    SnapListTableWidget *m_snap_list = NULL;
    QTextEdit *m_snap_edit = NULL;

    QPushButton *m_snap_new = NULL;
    QPushButton *m_snap_del = NULL;
    QPushButton *m_snap_apply = NULL;
    QPushButton *m_snap_save = NULL;
    QPushButton *m_snap_cancel = NULL;
};

#endif //TOOL_HOST_H
