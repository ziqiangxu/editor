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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QCloseEvent>
#include <QMainWindow>
class QLineEdit;
class QDialog;
class QLabel;

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

    void showFindText();

    void on_action_Find_triggered();

    void on_action_Commit_triggered();

    void on_action_Settings_triggered();

    void on_action_About_triggered();

private:
    Ui::MainWindow *ui;

    bool isUntiled;  //to mark weather the file exist
    bool isSaved;  //to mark weather the file modification saved
    QString curFile;  //save the current file path
    QLineEdit *findLineEdit;
    QDialog *findDlg;
    QLabel *permanentLable;
};

#endif // MAINWINDOW_H
