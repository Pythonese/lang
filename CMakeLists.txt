cmake_minimum_required(VERSION 3.24.0)
project(Lang VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Lang main.cpp ErrorChecker/error_checker.cpp Tokenizer/tokenize.cpp TokenPreparer/preparer.cpp Ast/ast.cpp Parser/parser.cpp DataTree/data_tree.cpp DataTree/call.cpp)

target_include_directories(Lang PRIVATE ErrorChecker Tokenizer TokenPreparer Ast Parser DataTree)