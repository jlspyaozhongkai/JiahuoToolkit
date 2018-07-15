
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
#include <QTemporaryFile>
#include <QProcess>
#include <QMessageBox>
#include <QJsonDocument>
#include <QFileInfo>
#include "define.h"
#include "common_utils.h"
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

    connect(this->m_snap_list, &QTableWidget::cellChanged, [this](int row, int column){
        if (column == 1) {
            this->snapListRename(row);
        }
    });

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

    this->LoadConfig();
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

void HostDialog::snapListRename(int row)
{
    qDebug() << "Snapshot rename at:" << row;

    auto name_item = this->m_snap_list->item(row, 1);
    Q_ASSERT(name_item != NULL);

    HostSnap *snap = (HostSnap *)(name_item->data(SNAPLIST_DATA).value<void *>());
    Q_ASSERT(snap != NULL);

    snap->m_name = name_item->text();

    this->saveConfig();
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
    auto row = this->m_snap_list->currentRow();

    qDebug() << "Snapshot text changed at:" << row;

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
    snap->m_saving = this->getHost();
    snap->m_editing = snap->m_saving;

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

    this->saveConfig();
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

    this->saveConfig();
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
    this->saveConfig();
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

void HostDialog::snapLoadConfig(QJsonObject *json)
{

    return;
}

QJsonArray HostDialog::snapSaveConfig()
{
    QJsonArray json_host_snap_array;
    int count = this->m_snap_list->rowCount() - 1;  //最好一个是当前，不算！
    int row;
    for (row = 0; row < count; row++) {

        auto name_item = this->m_snap_list->item(row, 1);
        Q_ASSERT(name_item != NULL);

        HostSnap *snap = (HostSnap *)(name_item->data(SNAPLIST_DATA).value<void *>());

        QJsonObject json_host_snap_node;
        json_host_snap_node.insert("name", snap->m_name);
        json_host_snap_node.insert("content", snap->m_saving);

        json_host_snap_array.append(json_host_snap_node);
    }

    return json_host_snap_array;
}

void HostDialog::LoadConfig()
{
    qDebug() << "Load host config";

    //----------------------------------------------------------
    //Root
    QString path = CONFIG_ROOT "host_config.json";

    QFile file(path);
    if (! file.open(QFile::ReadOnly)) {
        QString errmsg = QString("Open file ") + path + " failed, error:" + file.errorString();
        qDebug() << errmsg;
        return;
    }
    QTextStream in(&file);
    QString json_txt = in.readAll();
    file.close();

    qDebug() << "Load json: " << json_txt;

    //----------------------------------------------------------

    return;
}

void HostDialog::saveConfig()
{
    qDebug() << "Save host config";
    //----------------------------------------------------------
    //Host
    QJsonObject json_host_obj;
    json_host_obj.insert("snaps", this->snapSaveConfig());

    //......
    //----------------------------------------------------------
    //Root
    QJsonObject json_root_obj;
    json_root_obj.insert("host", json_host_obj);

    QJsonDocument json_doc;
    json_doc.setObject(json_root_obj);
    qDebug() << "Json:" << json_doc.toJson();

    QString path = CONFIG_ROOT "host_config.json";
    QFileInfo path_info(path);

    util_mkdir_p(path_info.absolutePath());

    QFile file(path);
    if (! file.open(QFile::WriteOnly | QFile::Truncate)) {
        QString errmsg = QString("Open file ") + path + " failed, error:" + file.errorString();
        QMessageBox::critical(this, "写入配置错误", errmsg);
        return;
    }
    QTextStream out(&file);
    out << json_doc.toJson();
    out.flush();
    file.close();

    return;
}

#if defined(Q_OS_MACX)
static QString host_path = "/private/etc/hosts";

QString HostDialog::getHost()
{
    QString ret_val = "";
    QFile file(host_path);
    if (! file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Open file " << host_path << " failed.";
        ret_val = QString("#!!Read file ") + host_path + " failed, error :" + file.errorString() + ".!!!";
        return ret_val;
    }
    QTextStream in(&file);
    ret_val = in.readAll();
    file.close();
    return ret_val;
}

void HostDialog::setHost(QString txt)
{
    QTemporaryFile data_file("hosts");
    if (! data_file.open()) {
        qDebug() << "Open temporary data file failed.";
        return;
    }
    QTextStream data_out(&data_file);
    data_out << txt;
    data_out.flush();
    data_file.close();

    //cp -f /xxxxx /private/etc/hosts
    //osascript -e "do shell script \"cp -f /xxxxx /private/etc/hosts\" with administrator privileges"
    QProcess run;
    QStringList run_args;
    run_args << "-e";
    run_args << QString("do shell script \"")
                + "cp -f " + data_file.fileName() + " " + host_path
                + QString("\" with administrator privileges");
    qDebug() << "Run:" << "osascript" << " " << run_args;
    run.start("osascript", run_args);
    run.waitForFinished();

    return;
}

#elif defined(Q_OS_LINUX)

#elif defined(Q_OS_WIN)

#else

#endif

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
