#include "StringUtils.hpp"

StringUtils::StringUtils() {
   //ctor
}

StringUtils::~StringUtils() {
   //dtor
}

vector<string> &StringUtils::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> StringUtils::split(const string &s, char delim) {
    vector<string> elems;
    StringUtils::split(s, delim, elems);
    return elems;
}
