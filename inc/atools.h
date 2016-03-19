#ifndef ATOOLS
#define ATOOLS

#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdint>

using namespace std;

wstring utf8_to_wchar(const string &in);
string wchar_to_utf8(const wstring &in);

#if defined(__ANDROID__)

template <typename T>
inline string to_string(T value) {
    ostringstream os;
    os << value;

    return os.str();
}

inline int stoi(const string& _Str, size_t *_Idx = 0, int _Base = 10) {	// convert string to int
    const char *_Ptr = _Str.c_str();
    char *_Eptr;
    long _Ans = strtol(_Ptr, &_Eptr, _Base);

    if (_Idx != 0) {
        *_Idx = (size_t)(_Eptr - _Ptr);
    }
    return ((int)_Ans);
}

inline float stof(const string& _Str, size_t *_Idx = 0) {	// convert string to float
    const char *_Ptr = _Str.c_str();
    char *_Eptr;
    float _Ans = (float)strtod(_Ptr, &_Eptr);

    if (_Idx != 0) {
        *_Idx = (size_t)(_Eptr - _Ptr);
    }
    return (_Ans);
}

#endif

#endif // ATOOLS

