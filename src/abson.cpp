#include "abson.h"

#include <streambuf>

#include "atools.h"

AVariant ABson::load(const AByteVector &data, AVariant::Type type) {
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

                result.appendProperty((value) ? true : false, name);
            } break;
            case DOUBLE: {
                double value;
                memcpy(&value, &data[offset], sizeof(double));
                offset += sizeof(double);

                result.appendProperty((float)value, name); /// \todo: double type support
            } break;
            case STRING: {
                int32_t length;
                memcpy(&length, &data[offset], sizeof(uint32_t));
                offset += sizeof(uint32_t);

                char *value = new char[length];
                memcpy(value, &data[offset], length);
                offset += length;

                result.appendProperty(value, name);
                delete []value;
            } break;
            case INT32: {
                int32_t value;
                memcpy(&value, &data[offset], sizeof(uint32_t));
                offset += sizeof(uint32_t);

                result.appendProperty(value, name);
            } break;
            case OBJECT:
            case ARRAY: {
                int32_t length;
                memcpy(&length, &data[offset], sizeof(uint32_t));

                AByteVector sub(data.begin() + offset, data.begin() + offset + length);
                AVariant array  = load(sub, (t == ARRAY) ? AVariant::LIST : AVariant::MAP);
                result.appendProperty(array, name);

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
        case AVariant::BOOL: {
            result.push_back( (data.toBool()) ? 0x01 : 0x00 );
        } break;
        case AVariant::FLOAT: {
            double value    = data.toFloat();
            result.assign( reinterpret_cast<char *>( &value ), reinterpret_cast<char *>( &value ) + sizeof( value ) );
        } break;
        case AVariant::INT: {
            int32_t value   = data.toInt();
            result.assign( reinterpret_cast<char *>( &value ), reinterpret_cast<char *>( &value ) + sizeof( value ) );
        } break;
        case AVariant::STRING: {
            string value    = data.toString();
            uint32_t size   = value.size() + 1;
            result.resize(size + sizeof(uint32_t));

            memcpy(&result[0], &size, sizeof(uint32_t));
            memcpy(&result[sizeof(uint32_t)], value.c_str(), size);
        } break;
        case AVariant::LIST: {
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
        case AVariant::NONE: {
            result  = NONE;
        } break;
        case AVariant::BOOL: {
            result  = BOOL;
        } break;
        case AVariant::FLOAT: {
            result  = DOUBLE;
        } break;
        case AVariant::INT: {
            result  = INT32;
        } break;
        case AVariant::STRING: {
            result  = STRING;
        } break;
        case AVariant::LIST: {
            result  = ARRAY;
        } break;
        default: {
            result  = OBJECT;
        } break;
    }
    return result;
}
