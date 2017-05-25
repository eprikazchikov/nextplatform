#include "auri.h"

AUri::AUri(const string &uri) {
    PROFILE_FUNCTION()
    mUri        = uri;
    regex mExpresion("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?");
    regex_match(mUri, mResult, mExpresion);
}

string AUri::scheme() const {
    PROFILE_FUNCTION()
    return mResult[2].str();
}

string AUri::host() const {
    PROFILE_FUNCTION()
    return mResult[4];
}

string AUri::path() const {
    PROFILE_FUNCTION()
    return mResult[5];
}

string AUri::query() const {
    PROFILE_FUNCTION()
    return mResult[7];
}

string AUri::fragment() const {
    PROFILE_FUNCTION()
    return mResult[9];
}

string AUri::name() const {
    PROFILE_FUNCTION()
    string str = path();
    size_t found = str.rfind('/');
    if(found != string::npos) {
        str.replace(0, found + 1, "");
    }
    return str;
}
