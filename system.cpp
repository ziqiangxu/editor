/*
用于获取系统的相关信息
*/
#include "system.h"
#include <QString>
#include <QDesktopServices>
#include <QProcess>
#include <unistd.h>
#include <iostream>

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
    QProcess bash;
    //QString ping = getHomePath() + "/net.sh";
    //bash.start(ping);
    //sleep(400);
    //bash.close();
    //if (!bash.exitCode())
    if (!bash.execute("ping www.baidu.com -c 1"))
    {
        std::cout<<"网路通畅";
        return true;
    }
    else{
        return false;
        std::cout<<"网络不通";
    }
}
