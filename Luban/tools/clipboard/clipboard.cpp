
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include "clipboard.h"

static ClipBoardDialog *singleton = NULL;

ClipBoardDialog::ClipBoardDialog(QDialog *parent)
    : QDialog(parent)
{
    qDebug() << "ClipBoardDialog::ClipBoardDialog";

    this->setWindowTitle("ClipBoard");
    this->setMinimumSize(QSize(800, 600));

    QVBoxLayout *toplayout = new QVBoxLayout(this);
    this->setLayout(toplayout);

    //
    auto flush_btn = new QPushButton("刷新", this);
    toplayout->addWidget(flush_btn);
    flush_btn->setMaximumWidth(50);
    connect(flush_btn, &QPushButton::pressed, [this]{this->flush();});

    //

    return;
}

ClipBoardDialog::~ClipBoardDialog()
{
    return;
}

void ClipBoardDialog::flush()
{
    qDebug() << "ClipBoardDialog::flush";
    return;
}

void ClipBoardDialog::launch()
{
    qDebug() << "ClipBoardDialog::launch";

    if (singleton == NULL) {
        singleton = new ClipBoardDialog();
    }
    ClipBoardDialog *win = singleton;

    win->setModal(false);
    win->show();
    win->setFocus();
    win->flush();
    return;
}
