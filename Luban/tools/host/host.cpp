
#include <QHBoxLayout>
#include <QTabWidget>
#include <QSplitter>
#include <QDebug>
#include "host.h"

static HostDialog *singleton = NULL;

HostDialog::HostDialog(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "HostDialog::HostDialog";

    this->setWindowTitle("HOST 文件");
    this->setMinimumSize(QSize(800, 600));

    auto top_layout = new QHBoxLayout(this);
    top_layout->setMargin(1);
    top_layout->setSpacing(1);
    this->setLayout(top_layout);

    auto top_tab = new QTabWidget(this);
    top_layout->addWidget(top_tab);

    //快照
    auto snap_widget = new QWidget(this);
    top_tab->addTab(snap_widget, "快照");

    auto snap_top_layout = new QHBoxLayout(this);
    snap_top_layout->setMargin(1);
    snap_top_layout->setSpacing(1);
    snap_widget->setLayout(snap_top_layout);

    auto snap_top_split = new QSplitter(Qt::Horizontal, this);
    snap_top_layout->addWidget(snap_top_split);

    auto snap_left_widget = new QWidget(this);
    snap_top_split->addWidget(snap_left_widget);

    auto snap_left_layout = new QVBoxLayout(this);
    snap_left_layout->setMargin(1);
    snap_left_layout->setSpacing(1);
    snap_left_widget->setLayout(snap_left_layout);

    this->m_snap_list = new QListWidget(this);
    snap_left_layout->addWidget(this->m_snap_list);

    auto snap_left_btn_layout = new QHBoxLayout(this);
    snap_left_layout->addLayout(snap_left_btn_layout);

    this->m_snap_list_new = new QPushButton("新建", this);
    snap_left_btn_layout->addWidget(this->m_snap_list_new);

    this->m_snap_list_del = new QPushButton("删除", this);
    snap_left_btn_layout->addWidget(this->m_snap_list_del);

    this->m_snap_list_apply = new QPushButton("应用", this);
    snap_left_btn_layout->addWidget(this->m_snap_list_apply);

    snap_left_btn_layout->addStretch();

    auto snap_rigth_widget = new QWidget(this);
    snap_top_split->addWidget(snap_rigth_widget);

    auto snap_rigth_layout = new QVBoxLayout(this);
    snap_rigth_layout->setMargin(1);
    snap_rigth_layout->setSpacing(1);
    snap_rigth_widget->setLayout(snap_rigth_layout);

    this->m_snap_edit = new QTextEdit(this);
    snap_rigth_layout->addWidget(this->m_snap_edit);

    auto snap_right_btn_layout = new QHBoxLayout(this);
    snap_rigth_layout->addLayout(snap_right_btn_layout);

    snap_right_btn_layout->addStretch();

    this->m_snap_edit_ok = new QPushButton("保存", this);
    snap_right_btn_layout->addWidget(this->m_snap_edit_ok);

    this->m_snap_edit_cancel = new QPushButton("取消", this);
    snap_right_btn_layout->addWidget(this->m_snap_edit_cancel);

    //设置当前
    auto using_item = new QListWidgetItem();
    using_item->setText("当前");
    this->m_snap_list->addItem(using_item);

    connect(this->m_snap_list, &QListWidget::currentRowChanged, [this](int row){
        this->snapSelect(row);
    });
    this->m_snap_list->setCurrentRow(0);
    this->m_snap_list->setEditTriggers(QListWidget::SelectedClicked);

    connect(this->m_snap_list_new, &QPushButton::pressed, [this]{
        this->snapNew();
    });

    connect(this->m_snap_list_del, &QPushButton::pressed, [this]{
        this->snapDel();
    });

    connect(this->m_snap_list_apply, &QPushButton::pressed, [this]{
        this->snapApply();
    });

    //编辑
    auto edit_widget = new QWidget(this);
    top_tab->addTab(edit_widget, "编辑");

    return;
}

void HostDialog::snapSelect(int row)
{
    qDebug() << "Snapshot list row changed:" << row;

    if (row == this->m_snap_list->count() - 1) {
        qDebug() << "Snap list select now";

        this->m_snap_list_apply->setEnabled(false);
        this->m_snap_list_del->setEnabled(false);
        this->m_snap_edit_ok->setEnabled(false);
        this->m_snap_edit_cancel->setEnabled(false);
        return;
    } else {
        this->m_snap_list_apply->setEnabled(true);
        this->m_snap_list_del->setEnabled(true);
        this->m_snap_edit_ok->setEnabled(true);
        this->m_snap_edit_cancel->setEnabled(true);
    }

    return;
}

void HostDialog::snapNew()
{
    qDebug() << "Snapshot new";

    auto new_one = new QListWidgetItem();
    new_one->setText("新快照");
    new_one->setFlags(new_one->flags() | Qt::ItemIsEditable);
    this->m_snap_list->insertItem(this->m_snap_list->count() - 1, new_one);
    this->m_snap_list->setCurrentItem(new_one);

    //再释放一个鼠标单击事件到这个item就进入编辑了

    return;
}

void HostDialog::snapDel()
{
    qDebug() << "Snapshot del";

    return;
}

void HostDialog::snapApply()
{
    qDebug() << "Snapshot apply";

    return;
}

void HostDialog::launch()
{
    qDebug() << "HostDialog::launch";

    if (singleton == NULL) {
        singleton = new HostDialog();
    }
    HostDialog *win = singleton;

    win->show();
    win->setFocus();
    return;
}


