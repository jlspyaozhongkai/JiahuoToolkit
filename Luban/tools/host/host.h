
#ifndef TOOL_HOST_H
#define TOOL_HOST_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>

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
    QPushButton *m_snap_list_apply = NULL;

    QTextEdit *m_snap_edit = NULL;
    QPushButton *m_snap_edit_ok = NULL;
    QPushButton *m_snap_edit_cancel = NULL;
};

#endif //TOOL_HOST_H
