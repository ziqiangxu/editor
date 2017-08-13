#include "git.h"
#include "system.h"

#include <QProcess>
#include <QApplication>
#include <QMainWindow>

Git::Git()
{
}

bool Git::commit()
{
    QProcess bash;
    System s;
    QString commit = s.getHomePath() + "/commit.sh";
    return (!bash.execute(commit)) ? true : false;  //命令正常执行返回的状态码为零
}

bool Git::push()
{
    QProcess bash;
    System s;
    QString push = s.getHomePath() + "/push.sh";
    return (!bash.execute(push)) ? true : false;
}
