

#ifndef TOOL_CONVERT_H
#define TOOL_CONVERT_H

#include <QDialog>

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
