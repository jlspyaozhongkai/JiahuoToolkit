#include "mainwindow.h"

#include <QDebug>
#include <QString>
#include <QMenu>
#include <QMenuBar>

#include "tools/ascii/ascii.h"
#include "tools/mouse/mouse.h"
#include "tools/clipboard/clipboard.h"
#include "tools/convert/convert.h"
#include "tools/host/host.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupMenu();

    this->setWindowTitle("Main window");
    this->setMinimumSize(QSize(800, 600));

    return;
}

MainWindow::~MainWindow()
{
    return;
}

void MainWindow::setupMenu()
{
    QMenuBar *menubar = menuBar();

    QMenu *tool_menu = new QMenu("工具");
    menubar->addMenu(tool_menu);

    tool_menu->addAction("ASCII码表", []{AsciiDialog::launch();});
    tool_menu->addAction("鼠标查看", []{MouseDialog::launch();});
    tool_menu->addAction("剪切板查看", []{ClipBoardDialog::launch();});
    tool_menu->addAction("编码转换", []{ConvertDialog::launch();});
    tool_menu->addAction("Host文件", []{HostDialog::launch();});

    return;
}
