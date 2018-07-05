

#ifndef TOOL_CONVERT_H
#define TOOL_CONVERT_H

#include <QVBoxLayout>
#include <QDialog>
#include <QScrollArea>

//每个节点都是一个Coder的子类
class Coder :public QWidget
{
    Q_OBJECT
public:
    Coder(QWidget *parent = 0);
    ~Coder();
};

//为Coder及其子类提供下边缘调整大小高矮功能
class CoderBox :public QWidget
{
    Q_OBJECT
public:
    CoderBox(QWidget *parent = 0);
    ~CoderBox() {}

    void setWidget(QWidget *widget);
private:
    QVBoxLayout *layout;
};

//ConvertInner为主要实现(用QWidget不好使，需要继承实现)
class ConvertInner : public QWidget
{
    Q_OBJECT
public:
    ConvertInner(QWidget *parent = 0);
    ~ConvertInner() {}
};

//ConvertDialog为ConvertInner提供滚动条功能
class ConvertDialog : public QWidget
{
    Q_OBJECT
public:
    static void launch();
public:
    ConvertDialog(QWidget *parent = 0);
    ~ConvertDialog() {}
public:
};

#endif //CONVERT
