#include "../Tokenizer/tokenize.h"
#ifndef PREPARER_H
#define PREPARER_H 1
namespace token_preparer
{
    const std::string opers = error_checker::opers;
    const std::string ignore = error_checker::ignore;
    const std::string parens = error_checker::parens;
    const std::string openParens = error_checker::openParens;
    const std::string closeParens = error_checker::closeParens;
    const std::string separators = error_checker::separators;
    using Token1 = tokenizer::Token1;
    std::vector<token_preparer::Token1> twoOpersIntoParens(const std::vector<token_preparer::Token1>&, size_t start, size_t end, const std::string& o1, const std::string& o2);
    std::vector<token_preparer::Token1> dotsAndBackSlashesIntoParens(const std::vector<token_preparer::Token1>&);
    std::vector<token_preparer::Token1> colonsIntoParens(const std::vector<token_preparer::Token1>&);
    std::vector<token_preparer::Token1> putCallOpers(const std::vector<token_preparer::Token1>&);
    std::vector<token_preparer::Token1> callsIntoParens(const std::vector<token_preparer::Token1>&);
    std::vector<token_preparer::Token1> assignsIntoParens(const std::vector<token_preparer::Token1>&);
    std::vector<token_preparer::Token1> removeBadParens(const std::vector<token_preparer::Token1>&);
    std::vector<token_preparer::Token1> prepare(const std::vector<token_preparer::Token1>&);
}

#endif