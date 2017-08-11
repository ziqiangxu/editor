#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QCloseEvent>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void newFile();  //create a new file
    bool maybeSave();  //wether need to save
    bool save();  //save your modification
    bool saveAs();  //save the file as
    bool saveFile(const QString &fileName);  //save the file
    bool loadFile(const QString &fileName);  //load a file

private slots:
    void on_action_New_triggered();

    void on_action_Save_triggered();

    void on_action_SaveAs_triggered();

    void on_action_Open_triggered();

    void on_action_Close_triggered();

    void on_action_Exit_triggered();

    void on_action_Undo_triggered();

    void on_action_Cut_triggered();

    void on_action_Copy_triggered();

    void on_action_Paste_triggered();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    bool isUntiled;  //to mark weather the file be saved
    QString curFile;  //save the current file path
};

#endif // MAINWINDOW_H
