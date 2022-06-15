#ifndef JSONWRAPPER_H
#define JSONWRAPPER_H

#include <QString>
#include <QJsonObject>

///
/// \brief Класс для работы с JSON
///
class JsonWrapper
{
public:
    ///
    /// \brief Констурктор класса
    /// \param jsonFilePath Путь к файлу
    ///
    JsonWrapper(QString jsonFilePath);
    ///
    /// \brief Констурктор копирования
    /// \param obj
    ///
    JsonWrapper(QJsonObject obj);
    ///
    /// \brief Метод получения строки по ключу
    /// \param key Ключ
    /// \return Строка
    ///
    QString getString(const QString& key);
    ///
    /// \brief Метод получения целого числа по ключу
    /// \param key Ключ
    /// \return Целое число
    ///
    int getInt(const QString& key);
    ///
    /// \brief Метод получения вещественного числа по ключу
    /// \param key Ключ
    /// \return Вещественное число
    ///
    double getDouble(const QString& key);
    ///
    /// \brief Метод получения булевого значения по ключу
    /// \param key Ключ
    /// \return Булевое значения
    ///
    bool getBool(const QString& key);
    ///
    /// \brief Метод получения подмножества по ключу
    /// \param key Ключ
    /// \return
    ///
    JsonWrapper getSubItem(const QString& key);
private:
    ///
    /// \brief json объект
    ///
    QJsonObject jsObj;

};

#endif // JSONWRAPPER_H
