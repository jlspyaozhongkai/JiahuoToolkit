#include "mainwindow.h"

#include <QDebug>
#include <QString>
#include <QMenu>
#include <QMenuBar>

#include "tools/ascii/ascii.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupMenu();

    return;
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupMenu()
{
    QMenuBar *menubar = menuBar();

    QMenu *tool_menu = new QMenu("工具");
    menubar->addMenu(tool_menu);

    tool_menu->addAction("ASCII码表", []{AsciiDialog::launch();});

    return;
}
