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
#include "git.h"
#include "system.h"

#include <QProcess>
#include <QApplication>
#include <QMainWindow>

Git::Git()
{
    System s;
    WorkPath =s.getHomePath() + "/.config/git-blog-editor";
}

bool Git::commit()
{
    QProcess bash;
    QString commit = WorkPath + "/commit.sh";
    return (!bash.execute(commit)) ? true : false;  //命令正常执行返回的状态码为零
}

bool Git::push()
{
    QProcess bash;
    QString push = WorkPath + "/push.sh";
    return (!bash.execute(push)) ? true : false;
}
