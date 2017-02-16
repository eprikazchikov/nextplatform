#include "abson.h"

#include <streambuf>

#include "atools.h"

AVariant appendProperty(const AVariant &container, const AVariant &data, const string &name) {
    switch(container.type()) {
        case AMetaType::VariantList: {
            AVariant::AVariantList list = container.toList();
            list.push_back(data);
            return list;
        } break;
        case AMetaType::VariantMap: {
            AVariant::AVariantMap map   = container.toMap();
            if(name == STRUCTURE) {
                auto d = map.find(DATA);
                if(d != map.end()) {
                    AVariant::AVariantList list = d->second.toList();

                    AMetaType::Type type    = (AMetaType::Type)data.toInt();
                    void *object    = AMetaType::create(type);
                    AMetaType::convert(&list, AMetaType::VariantList, object, type);
                    return AVariant(type, object);
                }
            }
            map[name]    = data;
            return map;
        } break;
        default: break;
    }

    return container;
}

AVariant ABson::load(const AByteVector &data, AMetaType::Type type) {
    AVariant result(type);

    uint32_t offset = 0;

    uint32_t size;
    memcpy(&size, &data[offset], sizeof(uint32_t));
    offset  += sizeof(uint32_t);

    while(offset < size) {
        uint8_t t   = data[offset];
        offset++;

        string name;
        for(; offset < size; offset++) {
            if(data[offset] == 0) {
                offset++;
                break;
            }
            name.push_back(data[offset]);
        }

        switch(t) {
            case BOOL: {
                uint8_t value   = data[offset];
                offset++;

                result  = appendProperty(result, (value) ? true : false, name);
            } break;
            case DOUBLE: {
                double value;
                memcpy(&value, &data[offset], sizeof(double));
                offset += sizeof(double);

                result  = appendProperty(result, value, name);
            } break;
            case STRING: {
                int32_t length;
                memcpy(&length, &data[offset], sizeof(uint32_t));
                offset += sizeof(uint32_t);

                char *value = new char[length];
                memcpy(value, &data[offset], length);
                offset += length;

                result  = appendProperty(result, value, name);
                delete []value;
            } break;
            case INT32: {
                int32_t value;
                memcpy(&value, &data[offset], sizeof(uint32_t));
                offset += sizeof(uint32_t);

                result  = appendProperty(result, value, name);
            } break;
            case OBJECT:
            case ARRAY: {
                int32_t length;
                memcpy(&length, &data[offset], sizeof(uint32_t));

                AByteVector sub(data.begin() + offset, data.begin() + offset + length);
                AVariant array  = load(sub, (t == ARRAY) ? AMetaType::VariantList : AMetaType::VariantMap);
                result  = appendProperty(result, array, name);

                offset += length;
            } break;
            default: break;
        }

    }

    return result;
}

AByteVector ABson::save(const AVariant &data) {
    AByteVector result;

    switch(data.type()) {
        case AMetaType::Bool: {
            result.push_back( (data.toBool()) ? 0x01 : 0x00 );
        } break;
        case AMetaType::Double: {
            double value    = data.toDouble();
            result.assign( reinterpret_cast<char *>( &value ), reinterpret_cast<char *>( &value ) + sizeof( value ) );
        } break;
        case AMetaType::Int: {
            int32_t value   = data.toInt();
            result.assign( reinterpret_cast<char *>( &value ), reinterpret_cast<char *>( &value ) + sizeof( value ) );
        } break;
        case AMetaType::String: {
            string value    = data.toString();
            uint32_t size   = value.size() + 1;
            result.resize(size + sizeof(uint32_t));

            memcpy(&result[0], &size, sizeof(uint32_t));
            memcpy(&result[sizeof(uint32_t)], value.c_str(), size);
        } break;
        case AMetaType::VariantList: {
            uint32_t size   = sizeof(uint32_t);
            result.resize(size);
            uint32_t offset = size;
            uint32_t index  = 0;
            AVariant::AVariantList list = data.toList();
            for(auto &it: list) {
                AByteVector element = save(it);
                string i    = to_string(index);
                uint8_t t   = type(it);
                size       += element.size() + 1 + (i.size() + 1);
                result.resize(size);

                memcpy(&result[offset++], &t, 1);
                memcpy(&result[offset], i.c_str(), i.size() + 1);
                offset     += i.size() + 1;
                memcpy(&result[offset], &element[0], element.size());
                offset     += element.size();

                index++;
            }
            result.push_back(0x00);
            memcpy(&result[0], &(++size), sizeof(uint32_t));
        } break;
        default: {
            uint32_t size   = sizeof(uint32_t);
            result.resize(size);
            uint32_t offset = size;
            uint32_t index  = 0;
            AVariant::AVariantMap map   = data.toMap();
            for(auto &it: map) {
                AByteVector element = save(it.second);
                uint8_t t   = type(it.second);
                size       += element.size() + 1 + (it.first.size() + 1);
                result.resize(size);

                memcpy(&result[offset++], &t, 1);
                memcpy(&result[offset], it.first.c_str(), it.first.size() + 1);
                offset     += it.first.size() + 1;
                memcpy(&result[offset], &element[0], element.size());
                offset     += element.size();

                index++;
            }
            result.push_back(0x00);
            memcpy(&result[0], &(++size), sizeof(uint32_t));
        } break;
    }
    return result;
}

uint8_t ABson::type(const AVariant &data) {
    uint8_t result;
    switch (data.type()) {
        case AMetaType::Invalid:        result  = NONE; break;
        case AMetaType::Bool:           result  = BOOL; break;
        case AMetaType::Double:         result  = DOUBLE; break;
        case AMetaType::Int:            result  = INT32; break;
        case AMetaType::String:         result  = STRING; break;
        case AMetaType::VariantList:    result  = ARRAY; break;
        default:                        result  = OBJECT; break;
    }
    return result;
}
