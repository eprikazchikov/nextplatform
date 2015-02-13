#ifndef AURI_H
#define AURI_H

#include <regex>

using namespace std;

class AUri {
public:
    AUri                        (const string &uri);

    string                      scheme                      () const;
    string                      host                        () const;
    string                      path                        () const;
    string                      query                       () const;
    string                      fragment                    () const;

protected:
    string                      mUri;

    smatch                      mResult;

};

#endif // AURI_H
