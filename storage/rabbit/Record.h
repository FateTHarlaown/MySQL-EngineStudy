//
// Created by NorSnow_ZJ on 2018/12/11.
//

#ifndef MYSQL_RECORD_H
#define MYSQL_RECORD_H

#include <string>
#include <vector>

class Record {
public:
    explicit Record(char delimiter = '/');
    Record(const std::string &database, const std::string &table,
           char delimiter = '/');
    void setPrefix(const std::string &prefix);
    std::string getFormatedRow();
    bool setKey(const std::string &keyCol);
    std::string getKey();
    void appendCol(const std::string &col);
    std::vector<std::string> getCols();
    void clearCols();
    bool parseFromFormatedRow(std::string row);

private:
    const char delimiter_;
    std::string key_;
    std::string prefix_;
    std::vector<std::string> cols_;
};


#endif //MYSQL_RECORD_H
