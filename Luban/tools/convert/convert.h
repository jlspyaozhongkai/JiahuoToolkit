

#ifndef TOOL_CONVERT_H
#define TOOL_CONVERT_H

#include <QDialog>

//每个节点都是一个Coder的子类
class Coder :public QWidget
{
    Q_OBJECT
public:
    Coder(QWidget *parent = 0);
    ~Coder();
};

class ConvertInner : public QWidget
{
    Q_OBJECT
public:
    ConvertInner(QWidget *parent = 0);
    ~ConvertInner();
};

class ConvertDialog : public QDialog
{
    Q_OBJECT
public:
    static void launch();
public:
    ConvertDialog(QDialog *parent = 0);
    ~ConvertDialog();
public:
};

#endif //CONVERT
