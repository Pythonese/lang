#include <iostream>
#include <fstream>
#include <cstring>
#include "Parser/parser.h"

int main(int argc, char* argv[])
{
    std::ifstream file;
    if (argc >= 2)
    {
        file.open(argv[1]);
    }
    else
    {
        file.open("../../../input.txt");
    }
    char str1[1000]{};
    file.read(str1, 1000);
    file.close();
    std::string str = error_checker::removeComments(str1) + "\n";
    std::string s = error_checker::codeGood(str);
    if (s != "")
    {
        std::cout << s << std::endl;
        return 0;
    }
    std::vector<tokenizer::Token1> tokens = tokenizer::correctTokens(tokenizer::split(str));
    if (tokenizer::checkTokens(tokens) != 0)
    {
        std::cout << "ERROR " << tokenizer::checkTokens(tokens) << std::endl;
        return 0;
    }
    
    ast::Object* obj = parser::parse(token_preparer::assignsIntoParens(token_preparer::callsIntoParens(token_preparer::putCallOpers(token_preparer::colonsIntoParens(token_preparer::dotsAndBackSlashesIntoParens(tokens))))));
    obj->exec();

    return 0;
}