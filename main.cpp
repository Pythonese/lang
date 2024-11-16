#include <iostream>
#include <fstream>
#include "Parser/parser.h"

int main(int argc, char* argv[])
{
    // std::cout << error_checker::isValName("a") << std::endl;
    // exit(0);

    std::ifstream file;
    file.open("input.txt");
    char str1[1000]{};
    file.read(str1, 1000);
    file.close();
    std::string str = error_checker::removeComments(str1) + "\n";
    std::cout << str << std::endl << std::endl;
    std::string s = error_checker::codeGood(str);
    if (s != "")
    {
        std::cout << s << std::endl;
        return 0;
    }
    std::vector<tokenizer::Token1> tokens = tokenizer::split(str);
    tokens = tokenizer::correctTokens(tokens);
    if (tokenizer::checkTokens(tokens) != 0)
    {
        std::cout << "ERROR " << tokenizer::checkTokens(tokens) << std::endl;
        return 0;
    }
    tokens = token_preparer::dotsAndBackSlashesIntoParens(tokens);
    tokens = token_preparer::colonsIntoParens(tokens);
    tokens = token_preparer::putCallOpers(tokens);
    tokens = token_preparer::callsIntoParens(tokens);
    tokens = token_preparer::assignsIntoParens(tokens);
    
    for (tokenizer::Token1 t : tokens)
    {
        std::cout << t.toString() << std::endl;
    }
    
    ast::Object* obj = parser::parse(tokens);
    std::cout << "Program started" << std::endl;
    obj->exec();

    return 0;
}