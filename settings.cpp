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
#include "settings.h"
#include "ui_settings.h"
#include "system.h"

#include <iostream>
#include <QAbstractButton>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    lineEditGetFocus();
    ui->blogPath->setText(getBlogPath());
}

Settings::~Settings()
{
    delete ui;
}

void Settings::lineEditGetFocus()
{
    ui->blogPath->setFocus();
}


void Settings::on_buttonBox_accepted()
{
    QString BlogPath = ui->blogPath->text();
    //点击OK之后写入配置文件
    System s;
    //Settings w;
    QString WorkPath = s.getHomePath() + "/.config/git-blog-editor";
    bool write_BolgPath = WriteTxt(WorkPath + "/config", BlogPath);
    if (write_BolgPath)
    {
        //生成git脚本
        bool create_commit;
        bool create_push;
        //commit
        QString commit = "#！/bin/bash\n"
                         "cd " + BlogPath + "\n"
                         "git add .\n"
                         "git commit -m \"commit by git-blog-editor\"";
        create_commit = WriteTxt(WorkPath + "/commit.sh",commit);
        //push
        QString push = "#!/bin/bash\n"
                       "cd " + BlogPath + "\n"
                       "git push";
        create_push= WriteTxt(WorkPath + "/push.sh", push);

        if (!(create_commit && create_push))
        {
            QMessageBox::warning(this,
                                 tr("设置失败"),
                                 tr("生成git命令脚本失败"));
        }

    }
    else {
        QMessageBox::warning(this,
                             tr("设置失败"),
                             tr("写入配置文件失败"));
    }
}

QString Settings::getBlogPath()
{
    System s;
    QString WorkPath = s.getHomePath() + "/.config/git-blog-editor";
    QFile config(WorkPath + "/config");
    if (config.open(QFile::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&config);
        QString BlogPath = in.readLine();
        return BlogPath;
    }
    return "Have no blog";
}

bool Settings::WriteTxt(QString FilePath, QString Content)
{
    QFile write(FilePath);
    if(write.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream out(&write);
        out << Content;
        return true;
    }
    return false;
}
