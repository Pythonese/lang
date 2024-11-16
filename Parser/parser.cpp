#include "parser.h"

ast::Tuple *parser::parseTuple(const std::vector<token_preparer::Token1> &v, ast::Object *parent, size_t start, size_t end)
{
    ast::Tuple* tuple = new ast::Tuple();
    ast::Node* left = nullptr;
    ast::Node* right = nullptr;
    ast::Operator* headOper = nullptr;
    for (size_t i = start; i <= end;)
    {
        if (v[i].type == tokenizer::Token1::LPAR)
        {
            size_t j = findCloseParen(i, v);
            ast::Block* block = parseBlock(v, parent, i + 1, j - 1, v[i].value[0]);
            if (left == nullptr)
            {
                left = block;
            }
            else
            {
                right = block;
            }
            i = j + 1;
            continue;
        }
        if (v[i].type == tokenizer::Token1::VAL_NAME || v[i].type == tokenizer::Token1::VAR_NAME)
        {
            ast::Variable* variable = new ast::Variable(v[i].value);
            if (left == nullptr)
            {
                left = variable;
            }
            else
            {
                right = variable;
            }
            i++;
            continue;
        }
        if (v[i].type >= tokenizer::Token1::U && v[i].type <= tokenizer::Token1::B)
        {
            ast::Constant* constant = new ast::Constant(data_tree::PrimitiveType(v[i].type - 2), v[i].value);
            if (left == nullptr)
            {
                left = constant;
            }
            else
            {
                right = constant;
            }
            i++;
            continue;
        }
        if (v[i].type == tokenizer::Token1::OPER)
        {
            if (v[i].value == ":") headOper = new ast::Colon(left, right);
            else if (v[i].value == ".") headOper = new ast::Dot(left, right);
            else if (v[i].value == "=") headOper = new ast::Assign(left, right);
            else if (v[i].value == ":=") headOper = new ast::ColonAssign(left, right);
            else if (v[i].value == "$") headOper = new ast::Call(left, right);
            else if (v[i].value == "\\") headOper = new ast::Backslash(left, right);
            else throw std::runtime_error("Unexpected operator");
            i++;
            continue;
        }
        if (v[i].type == tokenizer::Token1::EOL)
        {
            if (headOper == nullptr)
            {
                if (left == nullptr)
                {
                    i++;
                    continue;
                }
                tuple->getNodes().push_back(left);
                left = nullptr;
                i++;
                continue;
            }
            headOper->getLeft() = left;
            headOper->getRight() = right;
            tuple->getNodes().push_back(headOper);
            headOper = nullptr;
            left = nullptr;
            right = nullptr;
            i++;
            continue;
        }
    }
    if (headOper == nullptr)
    {
        if (left != nullptr)
            tuple->getNodes().push_back(left);
        left = nullptr;
    }
    else
    {
        headOper->getLeft() = left;
        headOper->getRight() = right;
        if (headOper != nullptr)
            tuple->getNodes().push_back(headOper);
        headOper = nullptr;
        left = nullptr;
        right = nullptr;
    }
    return tuple;
}

ast::Array *parser::parseArray(const std::vector<token_preparer::Token1> &v, ast::Object *parent, size_t start, size_t end)
{
    ast::Array* array = new ast::Array();
    ast::Node* left;
    ast::Node* right;
    ast::Operator* headOper = nullptr;
    for (size_t i = start; i <= end;)
    {
        if (v[i].type == tokenizer::Token1::LPAR)
        {
            size_t j = findCloseParen(i, v);
            ast::Block* block = parseBlock(v, parent, i + 1, j - 1, v[i].value[0]);
            if (left == nullptr)
            {
                left = block;
            }
            else
            {
                right = block;
            }
            i = j + 1;
            continue;
        }
        if (v[i].type == tokenizer::Token1::VAL_NAME || v[i].type == tokenizer::Token1::VAR_NAME)
        {
            ast::Variable* variable = new ast::Variable(v[i].value);
            if (left == nullptr)
            {
                left = variable;
            }
            else
            {
                right = variable;
            }
            i++;
            continue;
        }
        if (v[i].type >= tokenizer::Token1::U and v[i].type <= tokenizer::Token1::B)
        {
            ast::Constant* constant = new ast::Constant(data_tree::PrimitiveType(v[i].type - 2), v[i].value);
            if (left == nullptr)
            {
                left = constant;
            }
            else
            {
                right = constant;
            }
            i++;
            continue;
        }
        if (v[i].type == tokenizer::Token1::OPER)
        {
            if (v[i].value == ":") headOper = new ast::Colon(left, right);
            else if (v[i].value == ".") headOper = new ast::Dot(left, right);
            else if (v[i].value == "=") headOper = new ast::Assign(left, right);
            else if (v[i].value == ":=") headOper = new ast::ColonAssign(left, right);
            else if (v[i].value == "$") headOper = new ast::Call(left, right);
            else if (v[i].value == "\\") headOper = new ast::Backslash(left, right);
            else throw std::runtime_error("Unexpected operator");
            i++;
            continue;
        }
        if (v[i].type == tokenizer::Token1::EOL)
        {
            if (headOper == nullptr)
            {
                array->getNodes().push_back(left);
                left = nullptr;
                i++;
                continue;
            }
            headOper->getLeft() = left;
            headOper->getRight() = right;
            array->getNodes().push_back(headOper);
            headOper = nullptr;
            left = nullptr;
            right = nullptr;
            i++;
            continue;
        }
    }
    return array;
}

ast::Object *parser::parseObject(const std::vector<token_preparer::Token1> &v, ast::Object *parent, size_t start, size_t end)
{
    ast::Object* object = new ast::Object(ast::Variable(v[start].value), ast::Variable("RETURN"), nullptr);
    object->getParent() = parent;
    ast::Node* left = nullptr;
    ast::Node* right = nullptr;
    ast::Operator* headOper = nullptr;
    for (size_t i = start; i <= end;)
    {
        if (v[i].type == tokenizer::Token1::LPAR)
        {
            size_t j = findCloseParen(i, v);
            if (left == nullptr)
            {
                if (v[j + 1].type != tokenizer::Token1::OPER)
                {
                    i++;
                    continue;
                }
                left = parseBlock(v, object, i + 1, j - 1, v[i].value[0]);
            }
            else
            {
                right = parseBlock(v, object, i + 1, j - 1, v[i].value[0]);
            }
            i = j + 1;
            continue;
        }
        if (v[i].type == tokenizer::Token1::RPAR)
        {
            i++;
            continue;
        }
        if (v[i].type == tokenizer::Token1::VAL_NAME || v[i].type == tokenizer::Token1::VAR_NAME)
        {
            ast::Variable* variable = new ast::Variable(v[i].value);
            if (left == nullptr)
            {
                left = variable;
            }
            else
            {
                right = variable;
            }
            i++;
            continue;
        }
        if (v[i].type >= tokenizer::Token1::U && v[i].type <= tokenizer::Token1::B)
        {
            ast::Constant* constant = new ast::Constant(data_tree::PrimitiveType(v[i].type - 2), v[i].value);
            if (left == nullptr)
            {
                left = constant;
            }
            else
            {
                right = constant;
            }
            i++;
            continue;
        }
        if (v[i].type == tokenizer::Token1::OPER)
        {
            if (v[i].value == ":") headOper = new ast::Colon(left, right);
            else if (v[i].value == ".") headOper = new ast::Dot(left, right);
            else if (v[i].value == "=") headOper = new ast::Assign(left, right);
            else if (v[i].value == ":=") headOper = new ast::ColonAssign(left, right);
            else if (v[i].value == "$") headOper = new ast::Call(left, right);
            else if (v[i].value == "\\") headOper = new ast::Backslash(left, right);
            else throw std::runtime_error("Unexpected operator");
            i++;
            continue;
        }
        if (v[i].type == tokenizer::Token1::EOL)
        {
            if (left == nullptr)
            {
                if (right != nullptr) { throw std::runtime_error("Unexpected end of line"); }
            }
            else
            {
                headOper->getLeft() = left;
                headOper->getRight() = right;
                object->getOperators().push_back(headOper);
                headOper = nullptr;
                left = nullptr;
                right = nullptr;
            }
            i++;
            continue;
        }
        throw std::runtime_error("Unexpected token in object " + v[i].toString());
    }
    return object;
}

ast::Generic *parser::parseGeneric(const std::vector<token_preparer::Token1> &v, ast::Object *parent, size_t start, size_t end)
{
    ast::Generic* generic = new ast::Generic();
    for (size_t i = start; i <= end; i++)
    {
        if (v[i].type == tokenizer::Token1::VAL_NAME || v[i].type == tokenizer::Token1::VAR_NAME || (v[i].type >= tokenizer::Token1::U && v[i].type <= tokenizer::Token1::B))
        {
            ast::Variable* variable = new ast::Variable(v[i].value);
            generic->getVariables().push_back(variable);
            continue;
        }
        throw std::runtime_error("Unexpected generic type");
    }
    return generic;
}

ast::Block *parser::parseBlock(const std::vector<token_preparer::Token1> &v, ast::Object *parent, size_t start, size_t end, char blockType)
{
    switch (blockType)
    {
    case '(':
        return parseTuple(v, parent, start, end);
    case '[':
        return parseArray(v, parent, start, end);
    case '{':
        return parseObject(v, parent, start, end);
    case '<':
        return parseGeneric(v, parent, start, end);
    default:
        throw std::runtime_error("Unexpected block type");
    }
}

ast::Object *parser::parse(const std::vector<token_preparer::Token1> &v)
{
    return (ast::Object*)parser::parseBlock(v, nullptr, 0, v.size() - 1, '{');
}