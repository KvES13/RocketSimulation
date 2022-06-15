#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>

///
/// \brief Функция для чтения csv файла
/// \param file_path Путь к файлу
/// \param skip_rows Количество пропускаемых строк
/// \return csv данные
///
std::vector<std::vector<double>> LoadCsvLog(std::string file_path, int skip_rows=1);
///
/// \brief Функция деления строки на подстроки
/// \param line Строка
/// \param delimiter Разделитель
/// \return Разделенная строка
///
std::vector<std::string> split(std::string& line, char delimiter);

#endif // CSVREADER_H
