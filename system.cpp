/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Author:     Ziqiang Xu <ziqiang_xu@yeah.net>
 * Maintainer: Ziqiang Xu <ziqiang_xu@yeah.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
