
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include "mouse.h"

static MouseDialog *singleton = NULL;

MouseDialog::MouseDialog(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "MouseDialog::MouseDialog";

    this->setWindowTitle("MOUSE");

    auto top_layout = new QGridLayout(this);
    this->setLayout(top_layout);

    top_layout->addWidget(new QLabel("X:", this), 0, 0, 1, 1);
    this->m_showx = new QLineEdit(this);
    this->m_showx->setReadOnly(true);
    top_layout->addWidget(this->m_showx, 0, 1, 1, 2);

    top_layout->addWidget(new QLabel("Y:", this), 1, 0, 1, 1);
    this->m_showy = new QLineEdit(this);
    this->m_showy->setReadOnly(true);
    top_layout->addWidget(this->m_showy, 1, 1, 1, 2);

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]{
        //qDebug() << "Timeout";
        QPoint pos = QCursor::pos();
        this->m_showx->setText(QString::asprintf("%d", pos.x()));
        this->m_showy->setText(QString::asprintf("%d", pos.y()));
    });
    timer->start(50);

    return;
}

void MouseDialog::launch()
{
    qDebug() << "MouseDialog::launch";

    if (singleton == NULL) {
        singleton = new MouseDialog();
    }
    MouseDialog *win = singleton;

    win->show();
    win->setFocus();
    return;
}
