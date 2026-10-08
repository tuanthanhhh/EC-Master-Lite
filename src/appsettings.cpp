#include "appsettings.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
AppSettings::AppSettings() {}

QString AppSettings::lastFolder(const QString& key)
{
    return settings().value(key, QDir::homePath()).toString();
}

void AppSettings::setLastFolder(const QString& key,const QString &path)
{
    settings().setValue(key,path);
}

void AppSettings::updateLastFolderFromFile(const QString& key,const QString &filePath)
{
    QFileInfo info(filePath);
    setLastFolder(key,info.absolutePath());
}

QString AppSettings::lastParam(const QString &key)
{
    return settings().value(key, "").toString();
}

void AppSettings::setLastParam(const QString &key, const QString &path)
{
    settings().setValue(key,path);
}

QSettings &AppSettings::settings()
{
    static QSettings s(iniFilePath(),QSettings::IniFormat);
    return s;
}

QString AppSettings::iniFilePath()
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir(configDir);
    if(!dir.exists())
    {
        dir.mkpath(".");
    }
    return configDir + "/config.ini";
}
