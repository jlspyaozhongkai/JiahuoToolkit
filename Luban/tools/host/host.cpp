
#include <QHBoxLayout>
#include <QTabWidget>
#include <QSplitter>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QCursor>
#include <QApplication>
#include <QIcon>
#include <QDebug>
#include <QColor>
#include "host.h"

#define SNAPLIST_DATA (Qt::UserRole + 1)

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
    this->m_snap_list->setEditTriggers(QListWidget::SelectedClicked);
    snap_left_layout->addWidget(this->m_snap_list);

    auto snap_left_btn_layout = new QHBoxLayout(this);
    snap_left_layout->addLayout(snap_left_btn_layout);

    this->m_snap_new = new QPushButton("新建", this);
    snap_left_btn_layout->addWidget(this->m_snap_new);

    this->m_snap_del = new QPushButton("删除", this);
    snap_left_btn_layout->addWidget(this->m_snap_del);

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

    this->m_snap_apply = new QPushButton("应用", this);
    snap_right_btn_layout->addWidget(this->m_snap_apply);

    this->m_snap_ok = new QPushButton("保存", this);
    snap_right_btn_layout->addWidget(this->m_snap_ok);

    this->m_snap_cancel = new QPushButton("取消", this);
    snap_right_btn_layout->addWidget(this->m_snap_cancel);

    //设置当前
    auto using_item = new QListWidgetItem();
    using_item->setText("当前");
    using_item->setData(SNAPLIST_DATA, QVariant::fromValue((void *)NULL));
    this->m_snap_list->addItem(using_item);

    connect(this->m_snap_list, &QListWidget::currentRowChanged, [this](int row){
        this->snapListSelect(row);
    });
    this->m_snap_list->setCurrentRow(0);

    connect(this->m_snap_new, &QPushButton::pressed, [this]{
        this->snapNew();
    });

    connect(this->m_snap_del, &QPushButton::pressed, [this]{
        this->snapDel();
    });

    connect(this->m_snap_apply, &QPushButton::pressed, [this]{
        this->snapApply();
    });

    connect(this->m_snap_ok, &QPushButton::pressed, [this]{
        this->snapOk();
    });

    connect(this->m_snap_cancel, &QPushButton::pressed, [this]{
        this->snapCancel();
    });

    connect(this->m_snap_edit, &QTextEdit::textChanged, [this]{
        this->snapContentChanged();
    });

    //编辑
    auto edit_widget = new QWidget(this);
    top_tab->addTab(edit_widget, "编辑");

    return;
}

void HostDialog::flushSnap(HostSnap *snap)
{
    if (snap->m_editing == snap->m_saving) {
        snap->m_item->setTextColor(QColor(0, 0, 0));
    } else {
        snap->m_item->setTextColor(QColor(255, 0, 0));
    }

    return;
}

void HostDialog::snapListSelect(int row)
{
    qDebug() << "Snapshot list row changed to:" << row;

    if (row == this->m_snap_list->count() - 1) {
        qDebug() << "Snapshot list select now";

        this->m_snap_del->setEnabled(false);
        this->m_snap_apply->setEnabled(false);
        this->m_snap_ok->setEnabled(false);
        this->m_snap_cancel->setEnabled(false);

        //

        return;
    } else {
        this->m_snap_del->setEnabled(true);
        this->m_snap_apply->setEnabled(true);
        this->m_snap_ok->setEnabled(true);
        this->m_snap_cancel->setEnabled(true);

        //载入
        auto cur_item = this->m_snap_list->currentItem();
        Q_ASSERT(cur_item != NULL);

        HostSnap *snap = (HostSnap *)(cur_item->data(SNAPLIST_DATA).value<void *>());
        this->m_snap_edit->blockSignals(true);
        this->m_snap_edit->setText(snap->m_editing);
        this->m_snap_edit->blockSignals(false);

        flushSnap(snap);
        return;
    }
}

void HostDialog::snapContentChanged()
{
    //保存
    auto cur_item = this->m_snap_list->currentItem();
    Q_ASSERT(cur_item != NULL);

    HostSnap *snap = (HostSnap *)(cur_item->data(SNAPLIST_DATA).value<void *>());
    snap->m_editing = this->m_snap_edit->toPlainText();

    flushSnap(snap);
    return;
}

void HostDialog::snapNew()
{
    qDebug() << "Snapshot new";

    auto snap = new HostSnap();
    snap->m_name = "新快照";

    auto new_item = new QListWidgetItem();
    new_item->setText(snap->m_name);
    new_item->setFlags(new_item->flags() | Qt::ItemIsEditable);
    new_item->setData(SNAPLIST_DATA, QVariant::fromValue((void *)snap));
    new_item->setIcon(QIcon("*"));

    snap->m_item = new_item;

    this->m_snap_list->insertItem(this->m_snap_list->count() - 1, new_item);
    this->m_snap_list->setCurrentItem(new_item);
    this->m_snap_list->setFocus();

    return;
}

void HostDialog::snapDel()
{
    qDebug() << "Snapshot del";

    auto cur_item = this->m_snap_list->currentItem();
    Q_ASSERT(cur_item != NULL);

    this->m_snap_list->removeItemWidget(cur_item);

    HostSnap *snap = (HostSnap *)(cur_item->data(SNAPLIST_DATA).value<void *>());
    delete snap;
    delete cur_item;

    return;
}

void HostDialog::snapApply()
{
    qDebug() << "Snapshot apply";

    return;
}

void HostDialog::snapOk()
{
    qDebug() << "Snapshot ok";

    return;
}

void HostDialog::snapCancel()
{
    qDebug() << "Snapshot ok";

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


