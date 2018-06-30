
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QClipboard>
#include <QLabel>
#include <QScrollArea>
#include "clipboard.h"

static ClipBoardDialog *singleton = NULL;

ClipBoardDialog::ClipBoardDialog(QDialog *parent)
    : QDialog(parent)
{
    qDebug() << "ClipBoardDialog::ClipBoardDialog";

    this->setWindowTitle("ClipBoard");
    this->setMinimumWidth(800);

    QVBoxLayout *toplayout = new QVBoxLayout(this);
    this->setLayout(toplayout);

    //
    auto flush_btn = new QPushButton("刷新:本窗体在最前是会自动刷新", this);
    toplayout->addWidget(flush_btn);
    flush_btn->setMaximumWidth(300);
    connect(flush_btn, &QPushButton::pressed, [this]{this->flush();});

    //文本
    auto layout = new QHBoxLayout(this);
    toplayout->addLayout(layout);
    layout->addWidget(new QLabel("文本", this));
    this->textAvalable = new QLabel(this);
    this->textAvalable->setFixedWidth(40);
    this->textAvalable->setAutoFillBackground(true);
    layout->addWidget(this->textAvalable);
    layout->addStretch();

    this->textShow = new QTextEdit(this);
    toplayout->addWidget(this->textShow);
    this->textShow->setReadOnly(true);
    this->textShow->setFixedHeight(80);

    //Pixmap
    layout = new QHBoxLayout(this);
    toplayout->addLayout(layout);
    layout->addWidget(new QLabel("Pixmap", this));
    this->pixmapAvalable = new QLabel(this);
    this->pixmapAvalable->setFixedWidth(40);
    this->pixmapAvalable->setAutoFillBackground(true);
    layout->addWidget(this->pixmapAvalable);
    layout->addStretch();

    QScrollArea *scroll = new QScrollArea(this);
    toplayout->addWidget(scroll);
    scroll->setFixedHeight(100);

    this->pixmapShow = new QLabel(this);
    scroll->setWidget(this->pixmapShow);
    this->pixmapShow->setFixedWidth(100);
    this->pixmapShow->setFixedHeight(100);

    //

    //
    QClipboard *board = QApplication::clipboard();
    connect(board, &QClipboard::changed, [this]{qDebug() << "QClipboard::changed"; this->flush();});

    return;
}

ClipBoardDialog::~ClipBoardDialog()
{
    return;
}

void ClipBoardDialog::flush()
{
    qDebug() << "ClipBoardDialog::flush";

    QPalette is_avalable;
    is_avalable.setColor(QPalette::Background, QColor(0, 255, 0));

    QPalette not_avalable;
    not_avalable.setColor(QPalette::Background, QColor(255, 0, 0));

    QClipboard *board = QApplication::clipboard();

    auto text = board->text();
    if (text.isEmpty()) {
        this->textAvalable->setPalette(not_avalable);
        this->textShow->setText("");
    } else {
        this->textAvalable->setPalette(is_avalable);
        this->textShow->setText(text);
    }

    auto pixmap = board->pixmap();
    if (pixmap.isNull()) {
        this->pixmapAvalable->setPalette(not_avalable);
        this->pixmapShow->setPixmap(pixmap);
    } else {
        this->pixmapAvalable->setPalette(is_avalable);
        this->pixmapShow->setPixmap(pixmap);
        this->pixmapShow->setFixedSize(pixmap.size());
    }

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
