#ifndef SETTINGS_H
#define SETTINGS_H

#include <QStringList>



class Settings
{
public:
    Settings();
    ~Settings();
    QStringList projects;
    int currentProjectIndex;
private:
    void read();
    void write();
};

#endif // SETTINGS_H
