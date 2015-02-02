#include "ajson.h"

#include "aobject.h"
#include "aobjectsystem.h"

#define J_TRUE  "true"
#define J_FALSE "false"
#define J_NULL  "null"

#define FORMAT (depth > -1) ? "\n" : "";

enum States {
    objectBegin = 1,
    objectEnd,
    arrayBegin,
    arrayEnd,
    propertyNext,
    propertyName,
    propertyValue
};

AJson::AJson() {
    mType   = AJson::VARIANT;
}

bool AJson::load(const string &data) {
    stack<AJson *> nodes;
    nodes.push(this);
    string name;
    States state    = propertyValue;
    unsigned int it = 0;
    bool root       = true;
    while(it < data.length()) {
        skipSpaces(data.c_str(), it);
        switch(data[it]) {
            case '{': {
                if(!root) {
                    AJson v;
                    nodes.push(&nodes.top()->appendProperty(v, name));
                } else {
                    root    = false;
                }
                nodes.top()->mType  = AJson::OBJECT;
                name    = "";
                state   = propertyName;
            } break;
            case '}': {
                nodes.pop();
            } break;
            case '[': {
                if(state == propertyValue) {
                    if(!root) {
                        AJson v;
                        nodes.push(&nodes.top()->appendProperty(v, name));
                    } else {
                        root    = false;
                    }
                    nodes.top()->mType  = AJson::ARRAY;
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
                if(state == propertyValue && nodes.top()->mType != AJson::ARRAY) {
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
                    nodes.top()->appendProperty(str, name);
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
                    nodes.top()->appendProperty(((number) ? v.toFloat() : v.toInt()), name);
                }
                it--;
            } break;
            case 't': {
                if(data.substr(it, 4) == J_TRUE) {
                    if(state == propertyValue) {
                        nodes.top()->appendProperty(true, name);
                    }
                    it  += 3;
                }
            } break;
            case 'f': {
                if(data.substr(it, 5) == J_FALSE) {
                    if(state == propertyValue) {
                        nodes.top()->appendProperty(false, name);
                    }
                    it  += 4;
                }
            } break;
            case 'n': {
                if(data.substr(it, 4) == J_NULL) {
                    if(state == propertyValue) {
                        nodes.top()->appendProperty(NULL, name);
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

string AJson::save(int depth) {
    string result;
    switch(mType) {
        case AJson::VARIANT: {
            if(mValue.type() == AVariant::STRING) {
                result += '"' + mValue.toString() + '"';
            } else {
                result += mValue.toString();
            }
        } break;
        case AJson::OBJECT: {
            result += "{";
            result += FORMAT;
            unsigned int i = 1;
            for(auto &it: mMap) {
                result.append(depth + 1, '\t');
                result += "\"" + it.first + "\": " + (it.second).save((depth > -1) ? depth + 1 : depth);
                result += ((i < mMap.size()) ? "," : "");
                result += FORMAT;
                i++;
            }
            if(depth > -1) {
                result.append(depth, '\t');
            }
            result += "}";
        } break;
        case AJson::ARRAY: {
            result += "[";
            result += FORMAT;
            unsigned int i = 1;
            for(auto &it: mArray) {
                result.append(depth + 1, '\t');
                result += (it).save((depth > -1) ? depth + 1 : depth) + ((i < mArray.size()) ? "," : "");
                result += ((i < mMap.size()) ? "," : "");
                result += FORMAT;
                i++;
            }
            if(depth > -1) {
                result.append(depth, '\t');
            }
            result += "]";
        } break;
        default: break;
    }
    return result;
}

AJson::types AJson::type() const {
    return mType;
}

void AJson::setType(AJson::types type) {
    mType   = type;
}

AJson &AJson::property(const string &key) {
    if(mType == AJson::OBJECT) {
        return mMap[key];
    }
}

inline void AJson::skipSpaces(const char *data, unsigned int &p) {
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

void AJson::appendProperty(const AVariant &value, const string &name) {
    AJson v;
    v.mType     = AJson::VARIANT;
    v.mValue    = value;
    appendProperty(v, name);
}

AJson &AJson::appendProperty(const AJson &value, const string &name) {
    if(mType == AJson::OBJECT) {
        mMap[name]  = value;
        return mMap[name];
    } else {
        mArray.push_back(value);
        return mArray.back();
    }
}
