#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>
class AppSettings
{
public:
    AppSettings();
    static QString lastFolder(const QString& key);

    static void setLastFolder(const QString& key,const QString& path);

    static void updateLastFolderFromFile(const QString& key,const QString& filePath);

    static QString lastParam(const QString& key);

    static void setLastParam(const QString& key, const QString & path);

private:
    static QSettings& settings();

    static QString iniFilePath();
};

#endif // APPSETTINGS_H
