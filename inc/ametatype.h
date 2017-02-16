#ifndef AMETATYPE_H
#define AMETATYPE_H

#include <map>
#include <typeinfo>
#include <typeindex>

using namespace std;

class AMetaType {
public:
    /*! \enum Type */
    enum Type {
        Invalid                 = 0,
        Bool,
        Int,
        Double,
        String,
        VariantMap,
        VariantList,

        Vector2D                = 10,
        Vector3D,
        Vector4D,
        Quaternion,
        Matrix3D,
        Matrix4D,
        Curve,
        UserType                = 20
    };

    struct Table {
        int                 (*get_size)                 ();
        void                (*static_new)               (void**);
        void                (*construct)                (void**);
        void                (*static_delete)            (void**);
        void                (*destruct)                 (void**);
        void                (*clone)                    (const void**, void**);
        void                (*move)                     (const void**, void**);
        bool                (*compare)                  (const void**, const void**);
        type_index const    (*index)                    ();
        const char         *name;
    };

public:
    AMetaType               (const Table *table);

    const char             *name                        () const;
    int                     size                        () const;
    void                   *construct                   (void *where, const void *copy = nullptr) const;
    void                   *create                      (const void *copy = nullptr) const;
    void                    destroy                     (void *data) const;
    void                    destruct                    (void *data) const;
    bool                    compare                     (const void *left, const void *right) const;
    bool                    isValid                     () const;

    static uint32_t         registerType                (Table &table);

    static uint32_t         type                        (const char *name);

    template<typename T>
    static uint32_t         type                        () {
        for(auto it : s_Types) {
            if(it.second.index() == type_index(typeid(T)) ) {
                return it.first;
            }
        }
        return Invalid;
    }

    static const char      *name                        (uint32_t type);
    static int              size                        (uint32_t type);
    static void            *construct                   (uint32_t type, void *where, const void *copy = nullptr);
    static void            *create                      (uint32_t type, const void *copy = nullptr);
    static void             destroy                     (uint32_t type, void *data);
    static void             destruct                    (uint32_t type, void *data);

    static bool             compare                     (const void *left, const void *right, uint32_t type);
    static bool             convert                     (const void *from, uint32_t fromType, void *to, uint32_t toType);
    static bool             hasConverter                (uint32_t from, uint32_t to);

    static bool             toBoolean                   (void *to, const void *from, const uint32_t fromType);
    static bool             toInteger                   (void *to, const void *from, const uint32_t fromType);
    static bool             toDouble                    (void *to, const void *from, const uint32_t fromType);
    static bool             toString                    (void *to, const void *from, const uint32_t fromType);
    static bool             toList                      (void *to, const void *from, const uint32_t fromType);
    static bool             toVector2D                  (void *to, const void *from, const uint32_t fromType);
    static bool             toVector3D                  (void *to, const void *from, const uint32_t fromType);
    static bool             toVector4D                  (void *to, const void *from, const uint32_t fromType);
    static bool             toMatrix3D                  (void *to, const void *from, const uint32_t fromType);
    static bool             toMatrix4D                  (void *to, const void *from, const uint32_t fromType);
    static bool             toQuaternion                (void *to, const void *from, const uint32_t fromType);
    static bool             toCurve                     (void *to, const void *from, const uint32_t fromType);

protected:
    typedef map<uint32_t, Table>    TypeMap;
    typedef map<string, uint32_t>   NameMap;

    typedef bool            (*converterCallback)        (void *to, const void *from, const uint32_t fromType);

    typedef map<uint32_t, map<uint32_t, converterCallback>> ConverterMap;

    const Table            *m_pTable;

    static TypeMap          s_Types;
    static NameMap          s_Names;

    static uint32_t         s_NextId;

    static ConverterMap     s_Converters;
};

template<typename T>
struct TypeFuncs {
    static int size() {
        return sizeof(T);
    }
    static void static_new(void** dest) {
        *dest = new T();
    }
    static void static_delete(void** x) {
        delete (*reinterpret_cast<T**>(x));
    }
    static void construct(void** dest) {
        new (*dest) T();
    }
    static void destruct(void** x) {
        (*reinterpret_cast<T**>(x))->~T();
    }
    static void clone(const void** src, void** dest) {
        *dest = new T(**reinterpret_cast<const T**>(src));
    }
    static void move(const void** src, void** dest) {
        **reinterpret_cast<T**>(dest) =
            **reinterpret_cast<const T**>(src);
    }
    static bool compare(const void** left, const void** right) {
        return (**reinterpret_cast<const T**>(left) == **reinterpret_cast<const T**>(right));
    }
    static type_index const index() {
        return type_index(typeid(T));
    }
};


//Bool template type
template<bool B> struct Bool;
typedef Bool<true> True;
typedef Bool<false> False;

//Bool implementation
template<bool B>
struct Bool {
    static const bool value = B;
    typedef Bool type;
};

template<typename T, typename Ptr>
struct CheckType;

template<typename T>
struct CheckType<T, True> {
    typedef typename std::add_pointer<
            typename std::remove_cv<
            typename std::remove_pointer<T>
            ::type>::type>::type type;
};

template<typename T>
struct CheckType<T, False> {
    typedef typename std::remove_cv<T>::type type;
};

template<typename T>
struct Table {
    typedef Bool<std::is_pointer<T>::value>         is_pointer;
    typedef typename CheckType<T, is_pointer>::type T_no_cv;

    static AMetaType::Table *get(const char *typeName) {
        static AMetaType::Table staticTable {
            TypeFuncs<T_no_cv>::size,
            TypeFuncs<T_no_cv>::static_new,
            TypeFuncs<T_no_cv>::construct,
            TypeFuncs<T_no_cv>::static_delete,
            TypeFuncs<T_no_cv>::destruct,
            TypeFuncs<T_no_cv>::clone,
            TypeFuncs<T_no_cv>::move,
            TypeFuncs<T_no_cv>::compare,
            TypeFuncs<T_no_cv>::index,
            typeName
        };
        return &staticTable;
    }
};

//Function to unpack args properly
template<typename T>
inline static AMetaType::Table *getTable(const char *typeName) {
    return Table<T>::get(typeName);
}

template<typename T>
static uint32_t         registerMetaType            (const char *typeName) {
    return AMetaType::registerType(*getTable<T>(typeName));
}

#endif // AMETATYPE_H
