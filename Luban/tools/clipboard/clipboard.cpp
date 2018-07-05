
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QClipboard>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QTabWidget>
#include <QImage>
#include <QMimeData>
#include "clipboard.h"

static ClipBoardDialog *singleton = NULL;

ClipBoardDialog::ClipBoardDialog(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "ClipBoardDialog::ClipBoardDialog";

    this->setWindowTitle("ClipBoard");
    this->setMinimumWidth(800);

    QVBoxLayout *toplayout = new QVBoxLayout(this);
    this->setLayout(toplayout);

    //顶上按钮
    auto flush_btn = new QPushButton("刷新:本窗体在最前是会自动刷新", this);
    flush_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(flush_btn, &QPushButton::pressed, [this]{this->flush();});
    toplayout->addWidget(flush_btn);

    //下部tab
    this->main_tab = new QTabWidget(this);
    toplayout->addWidget(this->main_tab);

    //文本
    this->textTab = new QWidget(this);
    this->main_tab->addTab(this->textTab, "Text");

    auto layout = new QVBoxLayout(this);
    this->textTab->setLayout(layout);

    this->textShow = new QTextEdit(this);
    this->textShow->setReadOnly(true);
    this->textShow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    layout->addWidget(this->textShow);

    //Pixmap
    this->pixmapTab = new QWidget(this);
    this->main_tab->addTab(this->pixmapTab, "PixMap");

    layout = new QVBoxLayout(this);
    this->pixmapTab->setLayout(layout);

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    layout->addWidget(scroll);

    this->pixmapShow = new QLabel(this);
    scroll->setWidget(this->pixmapShow);

    //Image
    this->imageTab = new QWidget(this);
    this->main_tab->addTab(this->imageTab, "Image");

    layout = new QVBoxLayout(this);
    this->imageTab->setLayout(layout);

    scroll = new QScrollArea(this);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    layout->addWidget(scroll);

    this->imageShow = new QLabel(this);
    scroll->setWidget(this->imageShow);

    //Mime
    this->mimeTab = new QWidget(this);
    this->main_tab->addTab(this->mimeTab, "Mime");

    layout = new QVBoxLayout(this);
    this->mimeTab->setLayout(layout);

    this->mimeShow = new QTextEdit(this);
    this->mimeShow->setReadOnly(true);
    layout->addWidget(this->mimeShow);

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
        this->main_tab->setTabEnabled(this->main_tab->indexOf(this->textTab), false);
        this->textShow->setText("");
    } else {
        this->main_tab->setTabEnabled(this->main_tab->indexOf(this->textTab), true);
        this->main_tab->setCurrentWidget(this->textTab);
        this->textShow->setText(text);
    }

    auto pixmap = board->pixmap();
    if (pixmap.isNull()) {
        this->main_tab->setTabEnabled(this->main_tab->indexOf(this->pixmapTab), false);
        this->pixmapShow->setPixmap(pixmap);
    } else {
        this->main_tab->setTabEnabled(this->main_tab->indexOf(this->pixmapTab), true);
        this->main_tab->setCurrentWidget(this->pixmapTab);
        this->pixmapShow->setPixmap(pixmap);
        this->pixmapShow->setFixedSize(pixmap.size());
    }

    auto image = board->image();
    if (image.isNull()) {
        this->main_tab->setTabEnabled(this->main_tab->indexOf(this->imageTab), false);
        QPixmap pixmap;
        this->imageShow->setPixmap(pixmap);
    } else {
        this->main_tab->setTabEnabled(this->main_tab->indexOf(this->imageTab), true);
        this->main_tab->setCurrentWidget(this->imageTab);
        QPixmap pixmap;
        pixmap.convertFromImage(image);
        this->imageShow->setPixmap(pixmap);
        this->imageShow->setFixedSize(pixmap.size());
    }

    auto mime = board->mimeData();
    if (mime == NULL) {
        this->main_tab->setTabEnabled(this->main_tab->indexOf(this->mimeTab), false);
        this->mimeShow->setText("");
    } else {
        this->main_tab->setTabEnabled(this->main_tab->indexOf(this->mimeTab), true);
        this->mimeShow->setText("Not support now.");
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

    win->show();
    win->setFocus();
    win->flush();
    return;
}
