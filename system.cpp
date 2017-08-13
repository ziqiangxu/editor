/*
用于获取系统的相关信息
*/
#include "system.h"
#include <QString>
#include <QDesktopServices>

System::System()
{

}

QString System::getHomePath()
{
    QString home_path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return home_path;
}

bool System::netStatus()
{
    return true ? true:false;
}
