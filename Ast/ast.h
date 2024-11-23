

#ifndef AST_H
#define AST_H 1

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <map>
#include <unordered_map>
#include "../DataTree/data_tree.h"
#include "../ErrorChecker/error_checker.h"

namespace ast
{
    class Node;
    class Constant;
    class Variable;
    class Block;
    class Tuple;
    class Array;
    class Object;
    class Generic;
    class Operator;

    static data_tree::Dynamic* currentDataObject;
    static Object* currentCodeObject;

    class Node
    {
    public:
        virtual data_tree::Dynamic& getValue() = 0;
        virtual data_tree::Dynamic& getReturnValue() = 0;
        virtual void exec() = 0;
        virtual void call(data_tree::Dynamic arg) = 0;
        virtual void sendMessage(std::string message) = 0;
        virtual data_tree::Dynamic* getType() {}
    };

    class Variable : public Node
    {
        std::string name;
        Object* parentWhereDeclared;
        data_tree::Dynamic returnValue;
    public:
        Variable() {}
        Variable(std::string name) : name(name), parentWhereDeclared(nullptr) {}
        // Variable(std::string name, Object* parentWhereDeclared) : name(name), parentWhereDeclared(parentWhereDeclared) {}
        bool operator==(const Variable& other) const { return name == other.name && parentWhereDeclared == other.parentWhereDeclared; }
        std::string getName() const { return name; }
        void setValue(data_tree::Dynamic value) { *currentDataObject->getAttr(name) = value; }
        data_tree::Dynamic& getValue() { return *currentDataObject->getAttr(name); }
        data_tree::Dynamic& getReturnValue() { return returnValue; }
        void exec();
        void call(data_tree::Dynamic arg);
        void sendMessage(std::string message)
        {
        }
        data_tree::Dynamic* getType()
        {
            return getValue().type;
        }
    };
    struct VariableHasher
    {
        size_t operator()(const Variable& v) const
        {
            return std::hash<std::string>()(v.getName());
        }
    };

    class Constant : public Node
    {
        data_tree::Dynamic value;
    public:
        data_tree::Dynamic& getValue() { return value; }
        data_tree::Dynamic& getReturnValue() { throw std::runtime_error("Constant cannot have returned value"); return value; }
        void exec();
        void call(data_tree::Dynamic arg) { throw std::runtime_error("Constant cannot be called"); }
        void sendMessage(std::string message) { throw std::runtime_error("Constant cannot be called"); }
        Constant(data_tree::PrimitiveType type, const std::string& value)
        {
            Variable v(data_tree::standartTypeToString((data_tree::StandartType)type));
            v.call(data_tree::toDynamic(value));
            this->value = v.getReturnValue();
        }
        data_tree::Dynamic* getType()
        {
            return value.type;
        }
    };

    class Block : public Node
    {
    public:
        void call(data_tree::Dynamic arg) { throw std::runtime_error("Block cannot be called"); }
        void sendMessage(std::string message) { throw std::runtime_error("Block cannot be called"); }
        data_tree::Dynamic& getReturnValue() { throw std::runtime_error("Block cannot have returned value"); return *((data_tree::Dynamic*)nullptr); }
    };

    class Tuple : public Block
    {
        std::vector<Node*> nodes;
        data_tree::Dynamic value;
    public:
        std::vector<Node*>& getNodes() { return nodes; }
        data_tree::Dynamic& getValue() { return value; }
        void exec();
        data_tree::Dynamic* getType()
        {
            return data_tree::standartTypes[data_tree::StandartType::T];
        }
    };

    class Array : public Block
    {
        std::vector<Node*> nodes;
        data_tree::Dynamic value;
    public:
        std::vector<Node*>& getNodes() { return nodes; }
        data_tree::Dynamic& getValue() { return value; }
        void exec();
        data_tree::Dynamic* getType()
        {
            return data_tree::standartTypes[data_tree::StandartType::A];
        }
    };

    class Object : public Block
    {
        std::vector<Operator*> operators;
        data_tree::Dynamic value;
        data_tree::Dynamic returnValue;
        Variable argVariable; // init in parser
        Variable retVariable;
        Object* parent;
        std::unordered_map<Variable, std::vector<data_tree::Dynamic>, VariableHasher> variables;
    public:
        Object(Variable argVariable, Variable retVariable, Object* parent) : argVariable(argVariable), retVariable(retVariable), parent(parent) {}
        struct O
        {
            Object* code;
            data_tree::Dynamic value;
            O(Object* code) : code(code) {}
            O(Object* code, data_tree::Dynamic value) : code(code), value(value) {}
        };
        std::vector<Operator*>& getOperators() { return operators; }
        data_tree::Dynamic& getValue() { return *new data_tree::Dynamic(data_tree::standartTypes[data_tree::StandartType::O], (size_t)new O(this, value)); }
        data_tree::Dynamic& getReturnValue() { return returnValue; }
        void exec();
        Object*& getParent() { return parent; }
        void call(data_tree::Dynamic arg)
        {
            variables[argVariable].back() = arg;
            exec();
            returnValue = variables[retVariable].back();
        }
        void sendMessage(std::string message)
        {
        }
        data_tree::Dynamic* getVariable(std::string name)
        {
            data_tree::Dynamic* attr = value.getAttr(name);
            if (attr == nullptr)
            {
                if (error_checker::isValName(name)) return getConstantVariable(name);
                throw std::runtime_error("Variable not found");
            }
            return attr;
        }
        data_tree::Dynamic* getConstantVariable(std::string name)
        {
            if (data_tree::isStandartType(name)) return data_tree::standartTypes[data_tree::getStandartType(name)];
            Object* obj = this;
            data_tree::Dynamic* attr = value.getAttr(name);
            while (attr == nullptr)
            {
                obj = obj->parent;
                if (obj == nullptr) return nullptr;
                attr = obj->value.getAttr(name);
            }
            return attr;
        }
        data_tree::Dynamic* getType()
        {
            return data_tree::standartTypes[data_tree::StandartType::O];
        }
    };

    class Generic : public Block
    {
        std::vector<Variable*> variables;
        data_tree::Dynamic value;
    public:
        std::vector<Variable*>& getVariables() { return variables; }
        data_tree::Dynamic& getValue() { exec(); return value; }
        void exec();
        data_tree::Dynamic* getType()
        {
            return data_tree::standartTypes[data_tree::StandartType::G];
        }
    };

    class Operator : public Node
    {
        Node* left;
        Node* right;
    public:
        Node*& getLeft() { return left; }
        Node*& getRight() { return right; }
        void setLeft(Node* node) { left = node; }
        void setRight(Node* node) { right = node; }

        data_tree::Dynamic& getValue() { return *((data_tree::Dynamic*)nullptr); }
        data_tree::Dynamic& getReturnValue() {}
        void call(data_tree::Dynamic arg) {}
        void sendMessage(std::string message) {}
        data_tree::Dynamic* getType()
        {
            return getValue().type;
        }

        Operator(Node* left, Node* right) : left(left), right(right) {}
    };

    class Colon : public Operator
    {
    public:
        void exec();
        Colon(Node* left, Node* right) : Operator(left, right) {}
    };

    class Dot : public Operator
    {
        data_tree::Dynamic* value;
    public:
        void exec();
        data_tree::Dynamic& getValue() { return *value; }
        Dot(Node* left, Node* right) : Operator(left, right) {}
    };

    class Assign : public Operator
    {
    public:
        void exec();
        Assign(Node* left, Node* right) : Operator(left, right) {}
    };

    class ColonAssign : public Operator
    {
    public:
        void exec();
        ColonAssign(Node* left, Node* right) : Operator(left, right) {}
    };

    class Call : public Operator
    {
        data_tree::Dynamic returnValue;
    public:
        void exec();
        data_tree::Dynamic& getValue() { return returnValue; }
        data_tree::Dynamic& getReturnValue() { return returnValue; }
        Call(Node* left, Node* right) : Operator(left, right) {}
    };
    
    class Backslash : public Operator
    {
        data_tree::Dynamic returnValue;
    public:
        void exec();
        data_tree::Dynamic& getValue() { return returnValue; }
        data_tree::Dynamic& getReturnValue() { return returnValue; }
        Backslash(Node* left, Node* right) : Operator(left, right) {}
    };
}

#endif