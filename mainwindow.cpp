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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "system.h"
#include "git.h"
#include "settings.h"

#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QFileDialog>
#include <QDialog>
#include <QLineEdit>
#include <iostream>
#include <QProcess>
#include <QLabel>
#include <QString>
#include <QDesktopServices>
#include <QDragEnterEvent>
#include <QUrl>
#include <QList>
#include <QMimeData>
#include <QDropEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //load the default file
    Settings set;
    if (set.default_file.isEmpty()){
        isUntiled = true;
        curFile = tr("untitled.txt");
        setWindowTitle(curFile);
    }
    else
    {
        loadFile(set.default_file);
    }

    //to make main window can accept drop event
    setAcceptDrops(true);

    //init the search dialog
    findDlg = new QDialog(this);
    findDlg->setWindowTitle(tr("查找"));
    findLineEdit = new QLineEdit(findDlg);  //接受一个QDialg对话为初始化对象的参数
    QPushButton *btn = new QPushButton(tr("查找下一个"), findDlg);  //在一个对话框上添加一个按钮
    QVBoxLayout *layout = new QVBoxLayout(findDlg);
    layout->addWidget(findLineEdit);
    layout->addWidget(btn);
    connect(btn,&QPushButton::clicked,this,&MainWindow::showFindText);  //查找窗口的信号——槽连接函数

    /*add the permanent lable on the main window
    permanentLable = new QLabel;
    permanentLable->setFrameStyle(QFrame::Box | QFrame::Sunken);
    permanentLable->setText(tr("<a href=\"https://ziqiangxu.github.io\">访问我的博客</a>"));
    permanentLable->setTextFormat(Qt::RichText);
    permanentLable->setOpenExternalLinks(true);
    ui->statusBar->addPermanentWidget(permanentLable);
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    if (maybeSave()){  //get weather the file need to save
        isUntiled = true;
        isSaved = false;
        curFile = tr("untiled.txt");
        setWindowTitle(curFile);
        ui->textEdit->clear();
        ui->textEdit->setVisible(true);
        ui->textEdit->setFocus();
    }
}

bool MainWindow::maybeSave()
{
    //文本框内容与加载文件时发生了变化，而且isSaved值为false则表示未保存
    if (ui->textEdit->document()->isModified() && !isSaved)
    {
        QMessageBox box;
        box.setWindowTitle(tr("警告"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile + tr(" 尚未保存，是否保存？"));
        QPushButton *yesBtn = box.addButton(tr("是(&Y)"),
                                            QMessageBox::YesRole);
        box.addButton(tr("否(&N"), QMessageBox::NoRole);
        QPushButton *cancelBut = box.addButton(tr("取消(&C)"),
                                               QMessageBox::RejectRole);
        box.exec();
        if (box.clickedButton() == yesBtn)
            return save();
        else if (box.clickedButton() == cancelBut)
            return false;
    }
    return true;
}

bool MainWindow::save()
{
if (isUntiled) {
    return saveAs();
}
else {
    return saveFile(curFile);
}
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("另存为"),
                                                    curFile);
    if (fileName.isEmpty()) return false;
    return saveFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,
                             tr("多文档编辑器"),
                             tr("无法写入文件 %1,/n %2").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);  //mouse to be waitting
    out << ui->textEdit->toPlainText();
    QApplication::restoreOverrideCursor();
    isUntiled = false;  //文件已保存在磁盘
    curFile = QFileInfo(fileName).canonicalFilePath();  //get the file's path
    setWindowTitle(curFile);
    ui->statusBar->showMessage(tr("已保存"),3000);
    isSaved = true;  //标记文件已保存
    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,
                             tr("多文档编辑器"),
                             tr("无法读取文件 %1:\n %2")
                             .arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntiled = false;
    isSaved = false;  //打开文件时isSaved置为false
    setWindowTitle(curFile);
    return true;
}

void MainWindow::on_action_New_triggered()
{
    newFile();
}

void MainWindow::on_action_Save_triggered()
{
    save();
}

void MainWindow::on_action_SaveAs_triggered()
{
    saveAs();
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        loadFile(fileName);
        ui->textEdit->setVisible(true);
    }
}

void MainWindow::on_action_Close_triggered()
{
    if (maybeSave())
    {
        ui->textEdit->setVisible(false);
    }
}

void MainWindow::on_action_Exit_triggered()
{
    //先执行关闭操作再退出应用,qApp是指向应用程序的全局指针
    //close the file and quit the application
    on_action_Close_triggered();
    qApp->quit();
}

//用户常点击右上角的按钮退出程序，退出程序前保存文件
void MainWindow::closeEvent(QCloseEvent *event)
{
   if (maybeSave())
    {
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::on_action_Undo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_action_Cut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_action_Copy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_action_Paste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::showFindText()
{
    QString str = findLineEdit->text();
    if (ui->textEdit->find(str,QTextDocument::FindBackward))
    {
        findDlg->setWindowTitle(tr("已找到"));
    }
    else
    {
        findDlg->setWindowTitle("未找到");
    }
}

void MainWindow::on_action_Find_triggered()
{
    findDlg->setWindowTitle(tr("查找字符"));
    findDlg->show();
}

void MainWindow::on_action_Commit_triggered()
{
    Git git;
    System s;

    QApplication::setOverrideCursor(Qt::WaitCursor);  //阻塞，鼠标设置为等待

    bool commit_result = git.commit();
    bool push_result = false;
    if (s.netStatus()) push_result = git.push();
    QApplication::restoreOverrideCursor();

    if (commit_result && push_result)
    {
        ui->statusBar->showMessage(tr("已提交到git"),8000);
        QMessageBox::about(this,
                           tr("温馨提示"),
                           tr("成功提交到git"));
    }
    else if (!commit_result && push_result) {
        QMessageBox::warning(this,
                              tr("提交结果"),
                              tr("只上传了您上一次的更新"));
    }
    else if (!push_result) {
        QMessageBox::warning(this,
                              tr("提交结果"),
                              tr("提交到git失败：\n"
                                 "请检查您的网络！"));
    }
}

void MainWindow::on_action_Settings_triggered()
{
    Settings *setting = new Settings(this);
    setting->show();
    setting->setWindowTitle(tr("设置"));
}

void MainWindow::on_action_About_triggered()
{
    About *about = new About(this);
    about->show();
    about->setWindowTitle(tr("关于"));
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();  //如果拖入含有URL则接受
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mime_data = event->mimeData();
    if (mime_data->hasUrls())
    {
        QList<QUrl> url_list = mime_data->urls();
        QString filename = url_list.at(0).toLocalFile();
        loadFile(filename);
    }
}

void MainWindow::on_action_triggered()
{
    QMessageBox::about(this,tr("检查更新"),tr("在线更新正在开发，暂时请前往"
                                          "https://github.com/ziqiangxu/editor查看"));
}
