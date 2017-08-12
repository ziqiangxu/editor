#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QFileDialog>
#include "about.h"
#include <QDialog>
#include <QLineEdit>
#include <iostream>
#include <QProcess>

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
    connect(btn,&QPushButton::clicked,this,&MainWindow::showFindText);
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
    QString commit = "/home/xu/commit.sh";
    QString push = "/home/xu/push.sh";
    int ResultCode = bash.execute(commit);
    bash.execute(push);
    if(ResultCode == 0){
            setWindowTitle(tr("执行正常"));
        }
        else {
            setWindowTitle(tr("执行异常"));
        }
    QApplication::restoreOverrideCursor();
}
