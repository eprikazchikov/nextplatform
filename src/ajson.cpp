#include "ajson.h"

#include "avariant.h"

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
}

AVariant AJson::load(const string &data) {
    AVariant result;

    stack<AVariant> s;
    stack<string>   n;
    string name;
    States state    = propertyValue;
    unsigned int it = 0;
    while(it < data.length()) {
        skipSpaces(data.c_str(), it);
        switch(data[it]) {
            case '{': {
                s.push(AVariant(AVariant::MAP));
                n.push(name);
                name    = "";
                state   = propertyName;
            } break;
            case '}': {
                result  = s.top();
                s.pop();
                if(s.size()) {
                    s.top().appendProperty(result, n.top());
                }
                n.pop();
                state   = propertyName;
            } break;
            case '[': {
                if(state == propertyValue) {
                    s.push(AVariant(AVariant::LIST));
                    n.push(name);
                    name    = "";
                }
            } break;
            case ']': {
                result  = s.top();
                s.pop();
                if(s.size()) {
                    s.top().appendProperty(result, n.top());
                }
                n.pop();
                state   = propertyName;
            } break;
            case ':': {
                state   = propertyValue;
            } break;
            case ',': {
                if(s.top().type() == AVariant::LIST) {
                    state   = propertyValue;
                } else {
                    state   = propertyName;
                    name    = "";
                }
            } break;
            case '"': {
                string str  = readString(data, it);
                if(state == propertyName) {
                    name    = str;
                } else {
                    s.top().appendProperty(str, name);
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
                unsigned int st = it;
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
                    AVariant v(data.substr(st, it - st));
                    s.top().appendProperty((number) ? AVariant(v.toFloat()) : AVariant(v.toInt()), name);
                }
                it--;
            } break;
            case 't': {
                if(data.substr(it, 4) == J_TRUE) {
                    if(state == propertyValue) {
                        s.top().appendProperty(true, name);
                    }
                    it  += 3;
                }
            } break;
            case 'f': {
                if(data.substr(it, 5) == J_FALSE) {
                    if(state == propertyValue) {
                        s.top().appendProperty(false, name);
                    }
                    it  += 4;
                }
            } break;
            case 'n': {
                if(data.substr(it, 4) == J_NULL) {
                    if(state == propertyValue) {
                        s.top().appendProperty(NULL, name);
                    }
                    it  += 3;
                }
            } break;
            default: break;
        }
        it++;
    }
    return result;
}

string AJson::save(const AVariant &data, int depth) {
    string result;
    switch(data.type()) {
        case AVariant::STRING: {
            result += '"' + data.toString() + '"';
        } break;
        case AVariant::MAP: {
            result += "{";
            result += FORMAT;
            unsigned int i = 1;
            AVariant::AVariantMap map   = data.toMap();
            for(auto &it: map) {
                result.append(depth + 1, '\t');
                result += "\"" + it.first + "\":" + ((depth > -1) ? " " : "") + save(it.second, (depth > -1) ? depth + 1 : depth);
                result += ((i < map.size()) ? "," : "");
                result += FORMAT;
                i++;
            }
            if(depth > -1) {
                result.append(depth, '\t');
            }
            result += "}";
        } break;
        case AVariant::VECTOR2D:
        case AVariant::VECTOR3D:
        case AVariant::VECTOR4D:
        case AVariant::LIST: {
            result += "[";
            result += FORMAT;
            unsigned int i = 1;
            AVariant::AVariantList list = data.toList();
            for(auto &it: list) {
                result.append(depth + 1, '\t');
                result += save(it, (depth > -1) ? depth + 1 : depth);
                result += ((i < list.size()) ? "," : "");
                result += FORMAT;
                i++;
            }
            if(depth > -1) {
                result.append(depth, '\t');
            }
            result += "]";
        } break;
        default: {
            result += data.toString();
        } break;
    }
    return result;
}

inline string AJson::readString(const string &data, unsigned int &it) {
    unsigned int s  = ++it;
    char c          = data[s];
    while(it < data.length() && c != '"') {
        c = data[++it];
        if(c == '\\') {
            c = data[++it];
        }
    }
    return data.substr(s, it - s);
}

inline void AJson::skipSpaces(const char *data, unsigned int &it) {
    while(isSpace(data[it])) {
        it++;
    }
}

inline bool AJson::isSpace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

inline bool AJson::isDigit(char c) {
    return c >= '0' && c <= '9';
}
