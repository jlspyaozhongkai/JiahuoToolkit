

#ifndef TOOL_CONVERT_H
#define TOOL_CONVERT_H

#include <QByteArray>
#include <QVBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTabWidget>
#include <QTextEdit>
#include <QLineEdit>

//数据缓冲区和数据类型描述，设计上允许拷贝（效率问题不大）
class CodeData {
public:
    CodeData() {
        return;
    }
    CodeData(const CodeData& from) {
        this->m_buf.clear();
        this->m_buf.append(from.m_buf);
        return;
    }
    ~CodeData() {
        return;
    }
private:
    QByteArray m_buf;

};

class DataView : public QWidget
{
    Q_OBJECT
public:
    DataView(QWidget *parent = 0);
    ~DataView() {}

private:
    QTabWidget *tabwidget = NULL;
    //Hex
    QWidget *hex_widget = NULL;
};

//每个节点都是一个Coder的子类
class Coder :public QWidget
{
    Q_OBJECT
public:
    Coder(QWidget *parent = 0);
    ~Coder() {}

    QString name() {return m_name;}
    QString desc() {return m_desc;}
    void setName(QString name) {this->m_name = name;}
    void setDesc(QString desc) {this->m_desc = desc;}
private:
    QString m_name = "";
    QString m_desc = "";
};

//Coder子类，CoderInput
class CoderInput : public Coder
{
    Q_OBJECT
public:
    CoderInput(QWidget *parent = 0);
    ~CoderInput() {}
private:
    QTextEdit *text_edit = NULL;

    QLineEdit *file_path = NULL;

    DataView *data_view = NULL;

    void flushText();
    void flushFile();
};

//为Coder及其子类提供下边缘调整大小高矮功能
class CoderBox :public QWidget
{
    Q_OBJECT
public:
    CoderBox(QWidget *parent = 0);
    ~CoderBox() {}

    void setCoder(Coder *coder);
private:
    QLabel *title_label = NULL;
    QPushButton *delcur = NULL;
    QHBoxLayout *coder_layout = NULL;
    Coder *coder = NULL;
    QPushButton *addnew = NULL;
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
