#include "utils.hpp"

const string formatTime(time_t time, const string &format) {
    stringstream buffer;
    tm tm = *localtime(&time);
    buffer << put_time(&tm, format.c_str());
    return buffer.str();
}

string join(const vector<string> &vec, const string &delimiter) {
    ostringstream oss;

    if (!vec.empty()) {
        auto it = vec.begin();
        oss << *it++;
        while (it != vec.end() && !(*it).empty()) {
            oss << delimiter << *it++;
        }
    }

    return oss.str();
}

string repeat(const string &str, int count) {
    string result;

    for (int i = 0; i < count; ++i) {
        result += str;
    }

    return result;
}