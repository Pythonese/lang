#include "../TokenPreparer/preparer.h"
#include "../Ast/ast.h"
#ifndef PARSER_H
#define PARSER_H 1
namespace parser
{
    ast::Tuple* parseTuple(const std::vector<token_preparer::Token1>& v, ast::Object* parent, size_t start, size_t end);
    ast::Array* parseArray(const std::vector<token_preparer::Token1>& v, ast::Object* parent, size_t start, size_t end);
    ast::Object* parseObject(const std::vector<token_preparer::Token1>& v, ast::Object* parent, size_t start, size_t end);
    ast::Generic* parseGeneric(const std::vector<token_preparer::Token1>& v, ast::Object* parent, size_t start, size_t end);
    ast::Block* parseBlock(const std::vector<token_preparer::Token1>& v, ast::Object* parent, size_t start, size_t end, char blockType);
    ast::Object* parse(const std::vector<token_preparer::Token1>& v);
}
#endif