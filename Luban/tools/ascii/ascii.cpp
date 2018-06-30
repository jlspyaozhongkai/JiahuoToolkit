
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QSize>
#include <QTextEdit>
#include <QTableWidget>
#include <QHeaderView>
#include "ascii.h"

static AsciiDialog *singleton = NULL;

struct AsciiData {
    char index;
    QString control;
    QString desc;
} ascii_datas[] = {
    {0, "NUL", "空字符 Null"},
    {1, "SOH", "标题开始"},
    {2, "STX", "本文开始"},
    {3, "ETX", "本文结束"},
    {4, "EOT", "传输结束"},
    {5, "ENQ", "请求"},
    {6, "ACK", "确认回应"},
    {7, "BEL", "响铃"},
    {8, "BS", "退格"},
    {9, "HT", "水平定位符号"},
    {10, "LF", "换行键"},
    {11, "VT", "垂直定位符号"},
    {12, "FF", "换页键"},
    {13, "CR", "会车键"},
    {14, "SO", "取消变换 Shift out"},
    {15, "SI", "启用变换 Shift in"},
    {16, "DLE", "跳出数据通讯"},
    {17, "DC1", "设备控制1"},
    {18, "DC2", "设备控制2"},
    {19, "DC3", "设备控制3"},
    {20, "DC4", "设备控制4"},
    {21, "NAK", "确认失败回应"},
    {22, "SYN", "同步用暂停"},
    {23, "ETB", "区块传输结束"},
    {24, "CAN", "取消"},
    {25, "EM", "连接介质中断"},
    {26, "SUB", "替换"},
    {27, "ESC", "跳出"},
    {28, "FS", "文件分割符"},
    {29, "GS", "组群分隔符"},
    {30, "RS", "记录分隔符"},
    {31, "US", "单元分隔符"},
    {32, "SP", "空格"},
    {33, "", ""},
    {34, "", ""},
    {35, "", ""},
    {36, "", ""},
    {37, "", ""},
    {38, "", ""},
    {39, "", ""},
    {40, "", ""},
    {41, "", ""},
    {42, "", ""},
    {43, "", ""},
    {44, "", ""},
    {45, "", ""},
    {46, "", ""},
    {47, "", ""},
    {48, "", ""},
    {49, "", ""},
    {50, "", ""},
    {51, "", ""},
    {52, "", ""},
    {53, "", ""},
    {54, "", ""},
    {55, "", ""},
    {56, "", ""},
    {57, "", ""},
    {58, "", ""},
    {59, "", ""},
    {60, "", ""},
    {61, "", ""},
    {62, "", ""},
    {63, "", ""},
    {64, "", ""},
    {65, "", ""},
    {66, "", ""},
    {67, "", ""},
    {68, "", ""},
    {69, "", ""},
    {70, "", ""},
    {71, "", ""},
    {72, "", ""},
    {73, "", ""},
    {74, "", ""},
    {75, "", ""},
    {76, "", ""},
    {77, "", ""},
    {78, "", ""},
    {79, "", ""},
    {80, "", ""},
    {81, "", ""},
    {82, "", ""},
    {83, "", ""},
    {84, "", ""},
    {85, "", ""},
    {86, "", ""},
    {87, "", ""},
    {88, "", ""},
    {89, "", ""},
    {90, "", ""},
    {91, "", ""},
    {92, "", ""},
    {93, "", ""},
    {94, "", ""},
    {95, "", ""},
    {96, "", ""},
    {97, "", ""},
    {98, "", ""},
    {99, "", ""},
    {100, "", ""},
    {101, "", ""},
    {102, "", ""},
    {103, "", ""},
    {104, "", ""},
    {105, "", ""},
    {106, "", ""},
    {107, "", ""},
    {108, "", ""},
    {109, "", ""},
    {110, "", ""},
    {111, "", ""},
    {112, "", ""},
    {113, "", ""},
    {114, "", ""},
    {115, "", ""},
    {116, "", ""},
    {117, "", ""},
    {118, "", ""},
    {119, "", ""},
    {120, "", ""},
    {121, "", ""},
    {122, "", ""},
    {123, "", ""},
    {124, "", ""},
    {125, "", ""},
    {126, "", ""},
    {127, "DEL", "删除"},
};

AsciiDialog::AsciiDialog(QDialog *parent)
    : QDialog(parent)
{
    qDebug() << "AsciiDialog::AsciiDialog";

    this->setWindowTitle("ASCII");
    this->setMinimumSize(QSize(800, 800));

    QVBoxLayout *toplayout = new QVBoxLayout(this);
    this->setLayout(toplayout);

    //
    toplayout->addWidget(new QLabel("ASCII表:", this));

    //
    auto table = new QTableWidget(this);
    toplayout->addWidget(table);
    assert(sizeof(ascii_datas)/sizeof(ascii_datas[0]) == 128);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(QStringList() << "10进制值" << "16进制值" << "控制符" << "说明");
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->hide();
    table->setRowCount(128);
    int iloop;
    for (iloop = 0; iloop < 128; iloop++) {
        QTableWidgetItem *item;
        item = new QTableWidgetItem(QString::asprintf("%d", iloop));
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        table->setItem(iloop, 0, item);
        item = new QTableWidgetItem(QString::asprintf("%02X", iloop));
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        table->setItem(iloop, 1, item);
        if (ascii_datas[iloop].control == "") {
            item = new QTableWidgetItem(QString::asprintf("%c", iloop));
        } else {
            item = new QTableWidgetItem(ascii_datas[iloop].control);
        }
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        table->setItem(iloop, 2, item);
        if (ascii_datas[iloop].desc == "") {
            item = new QTableWidgetItem("");
        } else {
            item = new QTableWidgetItem(ascii_datas[iloop].desc);
        }
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        table->setItem(iloop, 3, item);
    }

    //
    toplayout->addWidget(new QLabel("说明:", this));

    //
    auto notes_edit = new QTextEdit(this);
    toplayout->addWidget(notes_edit);
    QString notes =
    "协议标准 rfc20:<br/>"
    "https://datatracker.ietf.org/doc/rfc20/<br/>"
    "<br/><br/>"
    "重要区间:<br/>"
    "<table border=\"1\">"
    "<tr><td>10进制</td><td>16进制</td><td>字符</td></tr>"
    "<tr><td> 48 - 57 </td><td> 30 - 39 </td><td> 0 - 9 </td></tr>"
    "<tr><td> 65 - 90 </td><td> 41 - 5A </td><td> A - Z </td></tr>"
    "<tr><td> 97 - 122 </td><td> 61 - 7A </td><td> a - z </td></tr>"
    "</table>"
    "<br/><br/>"
    "几个关键字符：<br/>"
    "<table border=\"1\">"
    "<tr><td>格式符</td><td>10进制</td><td>16进制</td></tr>"
    "<tr><td>\t</td><td>0</td><td>09</td></tr>"
    "<tr><td>\r</td><td>13</td><td>0D</td></tr>"
    "<tr><td>\n</td><td>10</td><td>0A</td></tr>"
    "</table>"
    "<br/><br/>";
    notes_edit->setHtml(notes);
    notes_edit->setReadOnly(true);

    return;
}

AsciiDialog::~AsciiDialog()
{
    return;
}

void AsciiDialog::launch()
{
    qDebug() << "AsciiDialog::launch";

    if (singleton == NULL) {
        singleton = new AsciiDialog();
    }
    AsciiDialog *win = singleton;

    win->setModal(false);
    win->show();
    win->setFocus();
    return;
}
