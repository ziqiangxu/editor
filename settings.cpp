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
#include <QJsonObject>
#include <QJsonDocument>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    lineEditGetFocus();
    System s;
    workpath = s.getHomePath() + "/.config/git-blog-editor";

    /*非json文件存储数据
    ReadTxt(workpath + "/config", blog_path);
    ui->blogPath->setText(blog_path);

    ReadTxt(workpath + "/defaultFile.txt", default_file);
    ui->defaultFile->setText(default_file);
    */
    QString str;
    ReadTxt(workpath + "/config.json", str);
    QJsonDocument doc = QJsonDocument::fromJson(str.toLatin1());
    QJsonObject obj = doc.object();
    QJsonValue val;

    val = obj.take("blog_path");
    blog_path = val.toString();
    ui->blogPath->setText(blog_path);

    val = obj.take("default_file");
    default_file = val.toString();
    ui->defaultFile->setText(default_file);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::lineEditGetFocus()
{
    ui->blogPath->setFocus();
}

/*非json文件存储数据
void Settings::on_buttonBox_accepted()
{
    //点击OK之后写入配置文件
    blog_path = ui->blogPath->text();
    default_file = ui->defaultFile->text();

    bool write_BolgPath = WriteTxt(workpath + "/config", blog_path);
    bool write_defaultFile = WriteTxt(workpath + "/defaultFile.txt", default_file);
    bool create_commit;
    bool create_push;
    //commit脚本
    QString commit = "#！/bin/bash\n"
                     "cd " + blog_path + "\n"
                     "git add .\n"
                     "git commit -m \"commit by git-blog-editor\"";
    create_commit = WriteTxt(workpath + "/commit.sh",commit);
    //push脚本
    QString push = "#!/bin/bash\n"
                   "cd " + blog_path + "\n"
                   "git push";
    create_push= WriteTxt(workpath + "/push.sh", push);

    if (!((create_commit && create_push) && (write_defaultFile && write_BolgPath)))
    if(create_commit)
    {
        QMessageBox::warning(this,
                             tr("设置失败"),
                             tr("生成git命令脚本失败"));
    }
}
*/
void Settings::on_buttonBox_accepted()
{
    //用json格式保存配置文件
    blog_path = ui->blogPath->text();
    default_file = ui->defaultFile->text();
    QJsonObject json;
    json.insert("blog_path", blog_path);
    json.insert("default_file", default_file);
    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    WriteTxt(workpath + "/config.json", json_str);

    //生成git脚本
    bool create_commit;
    bool create_push;
    //commit脚本
    QString commit = "#！/bin/bash\n"
                     "cd " + blog_path + "\n"
                     "git add .\n"
                     "git commit -m \"commit by git-blog-editor\"";
    create_commit = WriteTxt(workpath + "/commit.sh",commit);
    //push脚本
    QString push = "#!/bin/bash\n"
                   "cd " + blog_path + "\n"
                   "git push";
    create_push= WriteTxt(workpath + "/push.sh", push);
    if (!(create_commit && create_push))
    {
        QMessageBox::warning(this,
                             tr("设置失败"),
                             tr("生成git命令脚本失败"));
    }

}

bool Settings::WriteTxt(QString FilePath, QString Content)
{
    QFile write(FilePath);
    if (write.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream out(&write);
        out << Content;
        write.close();
        return true;
    }
    return false;
}

bool Settings::ReadTxt(QString FilePath, QString& Content)
{

    QFile read(FilePath);
    if (read.open(QFile::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&read);
        Content = in.readAll();
        read.close();
        return true;
    }
    return false;
}
