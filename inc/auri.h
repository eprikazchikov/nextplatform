#ifndef AURI_H
#define AURI_H

#include <regex>

#include <acommon.h>

using namespace std;

class NEXT_LIBRARY_EXPORT AUri {
public:
    AUri                        (const string &uri);

    string                      scheme                      () const;
    string                      host                        () const;
    string                      path                        () const;
    string                      query                       () const;
    string                      fragment                    () const;
    string                      name                        () const;

protected:
    string                      mUri;

    smatch                      mResult;

};

#endif // AURI_H
