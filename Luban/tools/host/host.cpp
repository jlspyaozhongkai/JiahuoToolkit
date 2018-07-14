
#include <QHBoxLayout>
#include <QTabWidget>
#include <QSplitter>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QCursor>
#include <QApplication>
#include <QIcon>
#include <QDebug>
#include <QHeaderView>
#include <QColor>
#include <QFile>
#include <QTime>
#include <QDateTime>
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

    this->m_snap_list = new QTableWidget(this);
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

    this->m_snap_save = new QPushButton("保存", this);
    snap_right_btn_layout->addWidget(this->m_snap_save);

    this->m_snap_cancel = new QPushButton("取消", this);
    snap_right_btn_layout->addWidget(this->m_snap_cancel);

    //设置当前
    this->m_snap_list->setSelectionBehavior(QTableWidget::SelectRows);
    this->m_snap_list->horizontalHeader()->setStretchLastSection(true);
    this->m_snap_list->verticalHeader()->hide();
    this->m_snap_list->horizontalHeader()->hide();
    this->m_snap_list->setShowGrid(false);
    this->m_snap_list->setColumnCount(2);
    this->m_snap_list->setColumnWidth(0, 20);
    this->m_snap_list->setRowCount(1);

    auto prefix_item = new QTableWidgetItem();
    prefix_item->setData(SNAPLIST_DATA, QVariant::fromValue((void *)NULL));
    prefix_item->setFlags(prefix_item->flags() & ~Qt::ItemIsEditable);
    prefix_item->setText(" ");

    auto name_item = new QTableWidgetItem();
    name_item->setData(SNAPLIST_DATA, QVariant::fromValue((void *)NULL));
    name_item->setFlags(prefix_item->flags() & ~Qt::ItemIsEditable);
    name_item->setText("当前");

    this->m_snap_list->setItem(0, 0, prefix_item);
    this->m_snap_list->setItem(0, 1, name_item);

    connect(this->m_snap_list, &QTableWidget::currentCellChanged, [this]
            (int currentRow, int currentColumn,
            int previousRow, int previousColumn){
        Q_UNUSED(currentColumn)
        Q_UNUSED(previousColumn)
        if (currentRow != previousRow) {    //行变化
            this->snapListSelect(currentRow);
        }
    });
    this->m_snap_list->setCurrentItem(name_item);

    connect(this->m_snap_new, &QPushButton::pressed, [this]{
        this->snapNew();
    });

    connect(this->m_snap_del, &QPushButton::pressed, [this]{
        this->snapDel();
    });

    connect(this->m_snap_apply, &QPushButton::pressed, [this]{
        this->snapApply();
    });

    connect(this->m_snap_save, &QPushButton::pressed, [this]{
        this->snapSave();
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

void HostDialog::flushSnap(int row)
{
    auto prefix_item = this->m_snap_list->item(row, 0);
    Q_ASSERT(prefix_item != NULL);

    auto name_item = this->m_snap_list->item(row, 1);
    Q_ASSERT(name_item != NULL);

    HostSnap *snap = (HostSnap *)(name_item->data(SNAPLIST_DATA).value<void *>());
    Q_ASSERT(snap != NULL);

    if (snap->m_editing == snap->m_saving) {
        prefix_item->setText(" ");

        this->m_snap_apply->setEnabled(true);
        this->m_snap_save->setEnabled(false);
        this->m_snap_cancel->setEnabled(false);
    } else {
        prefix_item->setText("*");

        this->m_snap_apply->setEnabled(false);
        this->m_snap_save->setEnabled(true);
        this->m_snap_cancel->setEnabled(true);
    }

    return;
}

void HostDialog::snapListSelect(int row)
{
    if (row == this->m_snap_list->rowCount() - 1) {
        qDebug() << "Snapshot list select now";

        this->m_snap_del->setEnabled(false);
        this->m_snap_apply->setEnabled(false);
        this->m_snap_save->setEnabled(false);
        this->m_snap_cancel->setEnabled(false);

        //读取
        QString txt = this->getHost();

        this->m_snap_edit->blockSignals(true);
        this->m_snap_edit->setText(txt);
        this->m_snap_edit->blockSignals(false);

        return;
    } else {
        qDebug() << "Snapshot list select:" << row;

        this->m_snap_del->setEnabled(true);
        this->m_snap_apply->setEnabled(true);
        this->m_snap_save->setEnabled(true);
        this->m_snap_cancel->setEnabled(true);

        //载入
        auto name_item = this->m_snap_list->item(row, 1);
        Q_ASSERT(name_item != NULL);

        HostSnap *snap = (HostSnap *)(name_item->data(SNAPLIST_DATA).value<void *>());
        this->m_snap_edit->blockSignals(true);
        this->m_snap_edit->setText(snap->m_editing);
        this->m_snap_edit->blockSignals(false);

        flushSnap(row);
        return;
    }
}

void HostDialog::snapContentChanged()
{
    //保存
    auto row = this->m_snap_list->currentRow();

    auto name_item = this->m_snap_list->item(row, 1);
    Q_ASSERT(name_item != NULL);

    HostSnap *snap = (HostSnap *)(name_item->data(SNAPLIST_DATA).value<void *>());
    snap->m_editing = this->m_snap_edit->toPlainText();

    flushSnap(row);
    return;
}

void HostDialog::snapNew()
{
    auto index = this->m_snap_list->rowCount() - 1;

    qDebug() << "Snapshot new at:" << index;

    auto snap = new HostSnap();
    auto datatime_now = QDateTime::currentDateTime();
    snap->m_name = QString("Snapshot_") + datatime_now.toString("yyyy/dd/MM_hh:mm:ss");

    auto prefix_item = new QTableWidgetItem();
    prefix_item->setData(SNAPLIST_DATA, QVariant::fromValue((void *)NULL));
    prefix_item->setFlags(prefix_item->flags() & ~Qt::ItemIsEditable);
    prefix_item->setText(" ");

    auto name_item = new QTableWidgetItem();
    name_item->setData(SNAPLIST_DATA, QVariant::fromValue((void *)snap));
    name_item->setFlags(name_item->flags() | Qt::ItemIsEditable);
    name_item->setText(snap->m_name);

    snap->m_item = name_item;

    //选中
    this->m_snap_list->insertRow(index);
    this->m_snap_list->setItem(index, 0, prefix_item);
    this->m_snap_list->setItem(index, 1, name_item);

    this->m_snap_list->setCurrentItem(name_item);   //选中
    this->m_snap_list->setFocus();

    return;
}

void HostDialog::snapDel()
{
    auto row = this->m_snap_list->currentRow();

    qDebug() << "Snapshot del row:" << row;

    auto name_item = this->m_snap_list->item(row, 1);
    Q_ASSERT(name_item != NULL);

    HostSnap *snap = (HostSnap *)(name_item->data(SNAPLIST_DATA).value<void *>());
    delete snap;

    this->m_snap_list->removeRow(row);    //删除行

    auto next_item = this->m_snap_list->item(row, 1);
    Q_ASSERT(next_item != NULL);

    this->m_snap_list->setCurrentItem(next_item);   //选中
    this->m_snap_list->setFocus();

    return;
}

void HostDialog::snapApply()
{
    auto row = this->m_snap_list->currentRow();

    qDebug() << "Snapshot apply row:" << row;

    auto name_item = this->m_snap_list->item(row, 1);
    Q_ASSERT(name_item != NULL);

    HostSnap *snap = (HostSnap *)(name_item->data(SNAPLIST_DATA).value<void *>());

    this->setHost(snap->m_saving);
    return;
}

void HostDialog::snapSave()
{
    auto row = this->m_snap_list->currentRow();

    qDebug() << "Snapshot save row:" << row;

    auto name_item = this->m_snap_list->item(row, 1);
    Q_ASSERT(name_item != NULL);

    HostSnap *snap = (HostSnap *)(name_item->data(SNAPLIST_DATA).value<void *>());

    snap->m_saving = snap->m_editing;

    flushSnap(row);
    return;
}

void HostDialog::snapCancel()
{
    auto row = this->m_snap_list->currentRow();

    qDebug() << "Snapshot cancel row:" << row;

    auto name_item = this->m_snap_list->item(row, 1);
    Q_ASSERT(name_item != NULL);

    HostSnap *snap = (HostSnap *)(name_item->data(SNAPLIST_DATA).value<void *>());

    snap->m_editing = snap->m_saving;
    this->m_snap_edit->blockSignals(true);
    this->m_snap_edit->setText(snap->m_editing);
    this->m_snap_edit->blockSignals(false);

    flushSnap(row);
    return;
}

QString HostDialog::getHost()
{
    //TODO: 暂时只支持Mac  /private/etc/hosts
    QString host_path = "/private/etc/hosts";

    QString ret_val = "";

    QFile file(host_path);
    if (! file.open(QFile::ReadOnly | QFile::Text)) {
        return ret_val;
    }
    QTextStream in(&file);
    ret_val = in.readAll();
    file.close();

    return ret_val;
}

void HostDialog::setHost(QString txt)
{
    Q_UNUSED(txt);
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


