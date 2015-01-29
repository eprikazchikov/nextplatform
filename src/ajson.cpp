#include "ajson.h"

#include "aobject.h"
#include "aobjectsystem.h"

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
    AJsonValue n;
    n.setType(AJsonValue::ROOT);
    stack<AJsonValue *> nodes;
    nodes.push(&n);
    string name;
    States state    = propertyValue;
    while(it < data.length()) {
        skipSpaces(data.c_str(), it);
        switch(data[it]) {
            case '{': {
                AJsonValue v;
                v.setType(AJsonValue::OBJECT);
                nodes.push(&nodes.top()->setValue(name, v));

                state   = propertyName;
                name    = "";
            } break;
            case '}': {
                nodes.pop();
            } break;
            case '[': {
                if(state == propertyValue) {
                    AJsonValue v;
                    v.setType(AJsonValue::ARRAY);
                    nodes.push(&nodes.top()->setValue(name, v));
                    name    = "";
                }
            } break;
            case ']': {
                nodes.pop();
                state   = propertyName;
            } break;
            case ':': {
                state   = propertyValue;
            } break;
            case ',': {
                if(state == propertyValue && nodes.top()->type() != AJsonValue::ARRAY) {
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
                    nodes.top()->setValue(name, str);
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
                bool number     = false;
                while(it < data.length()) {
                    c = data[++it];
                    if(!isDigit(c) && c != '.') {
                        break;
                    }
                    if(c == '.') {
                        number  = true;
                    }
                }
                if(state == propertyValue) {
                    AVariant v(data.substr(s, it - s));
                    nodes.top()->setValue(name, ((number) ? v.toFloat() : v.toInt()));
                }
                it--;
            } break;
            case 't': {
                if(data.substr(it, 4) == J_TRUE) {
                    if(state == propertyValue) {
                        nodes.top()->setValue(name, true);
                    }
                    it  += 3;
                }
            } break;
            case 'f': {
                if(data.substr(it, 5) == J_FALSE) {
                    if(state == propertyValue) {
                        nodes.top()->setValue(name, false);
                    }
                    it  += 4;
                }
            } break;
            case 'n': {
                if(data.substr(it, 4) == J_NULL) {
                    if(state == propertyValue) {
                        nodes.top()->setValue(name, NULL);
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

AJsonValue::types AJsonValue::type() {
    return mType;
}

void AJsonValue::setType(const types type) {
    mType       = type;
}

void AJsonValue::setValue(const string &name, const AVariant &value) {
    AJsonValue v;
    v.mType     = AJsonValue::VARIANT;
    v.mValue    = value;
    setValue(name, v);
}

AJsonValue &AJsonValue::setValue(const string &name, AJsonValue &value) {
    if(mType == AJsonValue::OBJECT) {
        mMap[name]  = value;
        return mMap[name];
    } else {
        mArray.push_back(value);
        return mArray.back();
    }
}
