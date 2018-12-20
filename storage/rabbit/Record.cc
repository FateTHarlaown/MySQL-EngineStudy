//
// Created by NorSnow_ZJ on 2018/12/11.
//

#include "Record.h"

Record::Record(char delimiter) : delimiter_(delimiter) {
}

Record::Record(const std::string &database, const std::string &table,
               char delimiter) : delimiter_(delimiter) {
    prefix_ = database + delimiter + table;
}

std::string Record::getFormatedRow() {
    std::string row = prefix_;
    for (const auto &c : cols_) {
        row += delimiter_ + c;
    }
    return row;
}

void Record::setPrefix(const std::string &prefix) {
    prefix_ = prefix;
}

bool Record::setKey(const std::string &key) {
    size_t pos = key.find_last_of(delimiter_);
    if (pos == std::string::npos) {
        return false;
    }
    key_ = key;
    prefix_ = key.substr(0, pos);
    return true;
}

std::string Record::getKey() {
    return key_;
}

void Record::appendCol(const std::string &col) {
    cols_.push_back(col);
}

std::vector<std::string> Record::getCols() {
    return cols_;
}

void Record::clearCols() {
    cols_.clear();
}

bool Record::parseFromFormatedRow(std::string row) {
    std::string::size_type pos1, pos2;
    std::vector<std::string> v;
    pos2 = row.find(delimiter_);
    pos1 = 0;
    while (std::string::npos != pos2) {
        v.push_back(row.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = row.find(delimiter_, pos1);
    }
    if (pos1 != row.length()) {
        v.push_back(row.substr(pos1));
    }
    if (v.size() < 3) {
        return false;
    }
    prefix_ = v[0] + v[1];
    cols_.clear();
    for (size_t i = 2; i < v.size(); ++i) {
        cols_.push_back(v[i]);
    }
    return true;
}
