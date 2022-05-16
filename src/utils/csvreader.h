#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>

std::vector<std::vector<double>> LoadCsvLog(std::string file_path, int skip_rows=1);
std::vector<std::string> split(std::string& line, char delimiter);

#endif // CSVREADER_H
