#ifndef JSONWRAPPER_H
#define JSONWRAPPER_H

#include <QString>
#include <QJsonObject>


class JsonWrapper
{
public:
    JsonWrapper(QString jsonFilePath);
    JsonWrapper(QJsonObject obj);
    QString getString(const QString& key);
    int getInt(const QString& key);
    double getDouble(const QString& key);
    bool getBool(const QString& key);
    JsonWrapper getSubItem(const QString& key);
private:
    QJsonObject jsObj;

};

#endif // JSONWRAPPER_H
