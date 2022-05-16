#include "jsonwrapper.h"

JsonWrapper::JsonWrapper(QString jsonFilePath)
{
    QFile file(jsonFilePath);
    if(!file.open(QFile::ReadWrite))
    {
        QMessageBox::warning(0,"Warning","Failed to open\n" +
                             jsonFilePath,QMessageBox::Ok);
        return;
    }
    jsObj = QJsonDocument::fromJson(file.readAll()).object();
    file.close();
}

JsonWrapper::JsonWrapper(QJsonObject obj)
{
    jsObj = obj;
}

QString JsonWrapper::getString(const QString& key)
{
    return jsObj[key].toString();
}

int JsonWrapper::getInt(const QString& key)
{
    return jsObj[key].toInt();
}

double JsonWrapper::getDouble(const QString& key)
{
    return jsObj[key].toDouble();
}

bool JsonWrapper::getBool(const QString& key)
{
    return jsObj[key].toBool();
}

JsonWrapper JsonWrapper::getSubItem(const QString &key)
{
    return JsonWrapper(jsObj[key].toObject());
}
