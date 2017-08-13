#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "system.h"

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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isUntiled = true;
    curFile = tr("untitled.txt");
    setWindowTitle(curFile);
    //初始化查找文本操作
    findDlg = new QDialog(this);
    findDlg->setWindowTitle(tr("查找"));
    findLineEdit = new QLineEdit(findDlg);  //接受一个QDialg对话为初始化对象的参数
    QPushButton *btn = new QPushButton(tr("查找下一个"), findDlg);  //在一个对话框上添加一个按钮
    QVBoxLayout *layout = new QVBoxLayout(findDlg);
    layout->addWidget(findLineEdit);
    layout->addWidget(btn);
    connect(btn,&QPushButton::clicked,this,&MainWindow::showFindText);  //查找窗口的信号——槽连接函数

    permanentLable = new QLabel;
    permanentLable->setFrameStyle(QFrame::Box | QFrame::Sunken);
    permanentLable->setText(tr("<a href=\"https://ziqiangxu.github.io\">访问我的博客</a>"));
    permanentLable->setTextFormat(Qt::RichText);
    permanentLable->setOpenExternalLinks(true);
    ui->statusBar->addPermanentWidget(permanentLable);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    if (maybeSave()){  //get weather the file need to save
        isUntiled = true;
        curFile = tr("untiled.txt");
        setWindowTitle(curFile);
        ui->textEdit->clear();
        ui->textEdit->setVisible(true);
        ui->textEdit->setFocus();
    }
}

bool MainWindow::maybeSave()
{
    if (ui->textEdit->document()->isModified())
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

void MainWindow::on_action_Version_triggered()
{
    About *version = new About(this);
    version->show();
    version->setWindowTitle("关于");
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
    QApplication::setOverrideCursor(Qt::WaitCursor);  //阻塞，鼠标设置为等待
    QProcess bash;
    System s;

    QString commit = s.getHomePath() + "/commit.sh";
    QString push = s.getHomePath() + "/push.sh";

    int ResultCode = bash.execute(commit);
    //commit和push分步进行
    if(ResultCode == 0)
    {
            ui->statusBar->showMessage(tr("已提交到本地"),3000);  //3000毫秒之后消失
            if (s.netStatus())
            {
                bash.execute(push);
                QApplication::restoreOverrideCursor();
                ui->statusBar->showMessage(tr("已提交到git"),3000);
            }
            else {
                QApplication::restoreOverrideCursor();
                QMessageBox::warning(this,
                                      tr("提交结果"),
                                      tr("网络连接出错"));
            }
    }
        else {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this,
                              tr("提交结果"),
                              tr("可能产生本警告的原因：\n"
                                 "1.您未对本地仓库中的文件进行修改。"));
        }
}
