#include "auri.h"

AUri::AUri(const string &uri) {
    mUri        = uri;
    regex mExpresion("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?");
    regex_match(mUri, mResult, mExpresion);
}

string AUri::scheme() const {
    return mResult[2].str();
}

string AUri::host() const {
    return mResult[4];
}

string AUri::path() const {
    return mResult[5];
}

string AUri::query() const {
    return mResult[7];
}

string AUri::fragment() const {
    return mResult[9];
}
