#include "ajson.h"

#include "aobject.h"
#include "aobjectsystem.h"

#include <QDebug>

#define J_TRUE  "true"
#define J_FALSE "false"
#define J_NULL  "null"

enum States {
    objectBegin = 1,
    objectEnd,
    arrayBegin,
    arrayEnd,
    propertyNext,
    propertyName,
    propertyValue
};

string AJson::objectSave(const AObject &object) {
    return string();
}

AObject *AJson::objectLoad(const string &data) {
    return 0;
}

bool AJson::parse(const string &data) {
    unsigned int it = 0;
    string name;
    AJsonNode node;
    States state    = propertyValue;
    bool array      = false;
    while(it < data.length()) {
        skipSpaces(data.c_str(), it);
        switch(data[it]) {
            case '{': {
                qDebug() << "Begin object";
                state   = propertyName;
                name    = "";
            } break;
            case '}': {
                qDebug() << "End object";
            } break;
            case '[': {
                if(state == propertyValue) {
                    array   = true;
                }
            } break;
            case ']': {
                array   = false;
                state   = propertyName;
            } break;
            case ':': {
                state   = propertyValue;
            } break;
            case ',': {
                if(state == propertyValue && !array) {
                    state   = propertyName;
                    name    = "";
                } else {
                    state   = propertyValue;
                }
            } break;
            case '"': {
                unsigned int s  = ++it;
                char c          = 0;
                while(it < data.length()) {
                    c = data[++it];
                    if(c == '\\') {
                        c = data[++it];
                    } else if(c == '"') {
                        break;
                    }
                }
                string str  = data.substr(s, it - s);
                if(state == propertyName) {
                    name    = str;
                } else {
                    node[name]  = str;
                }
            } break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '-': {
                unsigned int s  = it;
                char c          = 0;
                while(it < data.length()) {
                    c = data[++it];
                    if(!isDigit(c) && c != '.') {
                        break;
                    }
                }
                if(state == propertyValue) {
                    node[name]  = data.substr(s, it - s);
                }
                it--;
            } break;
            case 't': {
                if(data.substr(it, 4) == J_TRUE) {
                    if(state == propertyValue) {
                        node[name]  = J_TRUE;
                    }
                    it  += 3;
                }
            } break;
            case 'f': {
                if(data.substr(it, 5) == J_FALSE) {
                    if(state == propertyValue) {
                        node[name]  = J_FALSE;
                    }
                    it  += 4;
                }
            } break;
            case 'n': {
                if(data.substr(it, 4) == J_NULL) {
                    if(state == propertyValue) {
                        node[name]  = J_NULL;
                    }
                    it  += 3;
                }
            } break;
            default: break;
        }
        it++;
    }
    return true;
}

void AJson::skipSpaces(const char *data, unsigned int &p) {
    while(isSpace(data[p])) {
        p++;
    }
}

inline bool AJson::isSpace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

inline bool AJson::isDigit(char c) {
    return c >= '0' && c <= '9';
}

AJsonNode::types AJsonNode::type() {
    return mType;
}

void AJsonNode::setValue(const string &name, const AVariant &value) {

}
