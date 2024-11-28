

#ifndef DATA_TREE_H
#define DATA_TREE_H 1

#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <stdexcept>
#include <iostream>

template <typename T1, typename T2>
T1 bit_cast(T2 value)
{
    return *(T1*)&value;
}

namespace data_tree
{
    struct Dynamic;
    struct Object
    {
        Object(void* code) : code(code), attributes() {}
        Object(const Object& o) : code(o.code), attributes(o.attributes) {}
        Dynamic& operator[](std::string name) { return attributes[name]; }
        void* code;
        std::unordered_map<std::string, Dynamic> attributes;
    };
    struct Dynamic
    {
        Dynamic* type;
        size_t value;
        Dynamic() : type(nullptr), value(0) {}
        Dynamic(int a) : type(nullptr), value(a) {}
        Dynamic(Dynamic* type, size_t value) : type(type), value(value) {}

        bool operator==(const Dynamic& other) const { return type == other.type && value == other.value; }
        
        Dynamic* getAttr(std::string name)
        {
            Object* obj = bit_cast<Object*>(value);
            auto& a = obj->attributes;
            auto f = a.find(name);
            if (f != a.end())
            {
                return &f->second;
            }
            return nullptr;
        }
        Dynamic* declare(std::string name, Dynamic* type)
        {
            Object* obj = bit_cast<Object*>(value);
            auto& a = obj->attributes;
            auto f = a.find(name);
            if (f != a.end())
            {
                f->second.type = type;
                return &f->second;
            }
            a[name] = Dynamic(type, 0);
            return &a[name];
        }
    };

    struct Tuple
    {
        Tuple(size_t size) : arr(new Dynamic[size + 1] { Dynamic(nullptr, size) }) {}
        Tuple(const Tuple& t)
        {
            this->arr = new Dynamic[t.size() + 1];
            for (size_t i = 0; i <= t.size(); i++) this->arr[i] = t.arr[i];
        }
        Dynamic& operator[](size_t i) { return arr[i + 1]; }
        size_t size() const { return arr[0].value; }
        Dynamic* arr;
    };

    struct HeapArray
    {
        HeapArray(size_t size) : arr(new Dynamic[size + 1] { Dynamic(nullptr, size) }) {}
        HeapArray(const HeapArray& t)
        {
            this->arr = new Dynamic[t.size() + 1];
            for (size_t i = 0; i <= t.size(); i++) this->arr[i] = t.arr[i];
        }
        Dynamic& operator[](size_t i) { return arr[i + 1]; }
        size_t size() const { return arr[0].value; }
        Dynamic* arr;
    };
    
    struct Generic
    {
        Generic(size_t size) : arr(new Dynamic[size + 1] { Dynamic(nullptr, size) }) {}
        Generic(const Generic& t)
        {
            this->arr = new Dynamic[t.size() + 1];
            for (size_t i = 0; i <= t.size(); i++) this->arr[i] = t.arr[i];
        }
        Dynamic& operator[](size_t i) { return arr[i + 1]; }
        size_t size() const { return arr[0].value; }
        Dynamic* arr;
    };

    enum PrimitiveType
    {
        U, I, F, C, S, B
    };
    enum StandartType
    {
        T=6, A, O, G, FUN, PRINT, INPUT
    };

    Dynamic toDynamic(const std::string& value);
    Dynamic toDynamic(Tuple value);
    Dynamic toDynamic(HeapArray value);
    Dynamic toDynamic(void* astObject);
    Dynamic toDynamic(PrimitiveType type, const std::string& value);

    std::string standartTypeToString(size_t type);

    size_t getStandartType(std::string name);
    Dynamic callU(StandartType type, size_t value);
    Dynamic callI(StandartType type, size_t value);
    Dynamic callF(StandartType type, size_t value);
    Dynamic callC(StandartType type, size_t value);
    Dynamic callS(StandartType type, size_t value);
    Dynamic callB(StandartType type, size_t value);
    Dynamic callT(StandartType type, size_t value);
    Dynamic callA(StandartType type, size_t value);
    Dynamic callO(StandartType type, size_t value);
    Dynamic callG(StandartType type, size_t value);
    Dynamic callFUN(StandartType type, size_t value);
    Dynamic callPRINT(StandartType type, size_t value);
    Dynamic callINPUT(StandartType type, size_t value);
    static data_tree::Dynamic* standartTypes[13] = { new data_tree::Dynamic(0), new data_tree::Dynamic(1), new data_tree::Dynamic(2), new data_tree::Dynamic(3), new data_tree::Dynamic(4), new data_tree::Dynamic(5), new data_tree::Dynamic(6), new data_tree::Dynamic(7), new data_tree::Dynamic(8), new data_tree::Dynamic(9), new data_tree::Dynamic(10), new data_tree::Dynamic(11), new data_tree::Dynamic(12) };

    bool isStandartType(std::string name);
    Dynamic callStandartFun(Dynamic* fun, Dynamic value);
    Dynamic* getGrandestParent(Dynamic* value);
    size_t toStandartType(Dynamic* type);
}

#endif