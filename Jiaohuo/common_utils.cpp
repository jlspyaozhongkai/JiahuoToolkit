
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include "common_utils.h"

bool util_mkdir_p(QString path)
{
    qDebug() << "mkdir -p " << path;

    QDir dir(path);
    if(dir.exists()) {
        return true;
    } else {
       return dir.mkpath(path);
    }
}
