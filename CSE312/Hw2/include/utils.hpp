#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <array>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#define DEFAULT_TIME_FORMAT "%d/%m/%Y %H:%M:%S"

using namespace std;

template <size_t N>
array<uint8_t, N> stou8Array(const string &str) {
    array<uint8_t, N> arr = {0};
    size_t copyLength = std::min(str.size(), arr.size());

    for (size_t i = 0; i < copyLength; ++i) {
        arr[i] = static_cast<uint8_t>(str[i]);
    }

    return arr;
}

template <size_t N>
string u8ArrayToString(const array<uint8_t, N> &arr) {
    string str(arr.begin(), arr.end());
    str.erase(remove(str.begin(), str.end(), '\0'), str.end());
    return str;
}

const string formatTime(time_t time, const string &format = DEFAULT_TIME_FORMAT);
string join(const vector<string> &vec, const string &delimiter);
string repeat(const string &str, int count);

#endif
