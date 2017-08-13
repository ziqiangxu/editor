#ifndef SYSTEM_H
#define SYSTEM_H
class QString;

class System
{
public:
    System();
    QString getHomePath();
    bool netStatus();
};

#endif // SYSTEM_H
