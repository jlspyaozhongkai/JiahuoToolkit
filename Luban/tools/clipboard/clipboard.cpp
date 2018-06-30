
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QClipboard>
#include <QLabel>
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

    //
    QClipboard *board = QApplication::clipboard();
    connect(board, &QClipboard::changed, [this]{this->flush();});

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
