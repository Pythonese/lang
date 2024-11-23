#include "data_tree.h"

data_tree::Dynamic data_tree::callU(StandartType type, size_t value)
{
    switch (type)
    {
    case U:
        return Dynamic(standartTypes[U], size_t(bit_cast<size_t>(value)));
    case I:
        return Dynamic(standartTypes[U], size_t(bit_cast<ptrdiff_t>(value)));
    case F:
        return Dynamic(standartTypes[U], size_t(bit_cast<double>(value)));
    case C:
        return Dynamic(standartTypes[U], size_t(bit_cast<char>(value)));
    case S:
    {
        auto s = *(std::string*)(value);
        auto n = std::stoull(s);
        return Dynamic(standartTypes[U], n);
    }
    case B:
        return Dynamic(standartTypes[U], bool(value));
    default:
        throw std::runtime_error("Unknown type");
    }
}

data_tree::Dynamic data_tree::callI(StandartType type, size_t value)
{
    switch (type)
    {
    case U:
        return Dynamic(standartTypes[I], ptrdiff_t(bit_cast<size_t>(value)));
    case I:
        return Dynamic(standartTypes[I], ptrdiff_t(bit_cast<ptrdiff_t>(value)));
    case F:
        return Dynamic(standartTypes[I], ptrdiff_t(bit_cast<double>(value)));
    case C:
        return Dynamic(standartTypes[I], ptrdiff_t(bit_cast<char>(value)));
    case S:
        return Dynamic(standartTypes[I], bit_cast<size_t>(std::stoll(*bit_cast<std::string*>(value))));
    case B:
        return Dynamic(standartTypes[I], bool(value));
    default:
        throw std::runtime_error("Unknown type");
    }
}

data_tree::Dynamic data_tree::callF(StandartType type, size_t value)
{
    switch (type)
    {
    case U:
        return Dynamic(standartTypes[F], double(bit_cast<size_t>(value)));
    case I:
        return Dynamic(standartTypes[F], double(bit_cast<ptrdiff_t>(value)));
    case F:
        return Dynamic(standartTypes[F], double(bit_cast<double>(value)));
    case C:
        return Dynamic(standartTypes[F], double(bit_cast<char>(value)));
    case S:
        return Dynamic(standartTypes[F], bit_cast<size_t>(std::stod(*bit_cast<std::string*>(value))));
    case B:
        return Dynamic(standartTypes[F], bool(value));
    default:
        throw std::runtime_error("Unknown type");
    }
}

data_tree::Dynamic data_tree::callC(StandartType type, size_t value)
{
    switch (type)
    {
    case U:
        return Dynamic(standartTypes[C], char(bit_cast<size_t>(value)));
    case I:
        return Dynamic(standartTypes[C], char(bit_cast<ptrdiff_t>(value)));
    case F:
        return Dynamic(standartTypes[C], char(bit_cast<double>(value)));
    case C:
        return Dynamic(standartTypes[C], char(bit_cast<char>(value)));
    case S:
        return Dynamic(standartTypes[C], (*bit_cast<std::string*>(value))[0]);
    case B:
        return Dynamic(standartTypes[C], bool(value));
    default:
        throw std::runtime_error("Unknown type");
    }
}

data_tree::Dynamic data_tree::callS(StandartType type, size_t value)
{
    switch (type)
    {
    case U:
        return Dynamic(standartTypes[S], (size_t)new std::string(std::to_string(bit_cast<size_t>(value))));
    case I:
        return Dynamic(standartTypes[S], (size_t)new std::string(std::to_string(bit_cast<ptrdiff_t>(value))));
    case F:
        return Dynamic(standartTypes[S], (size_t)new std::string(std::to_string(bit_cast<double>(value))));
    case C:
        return Dynamic(standartTypes[S], (size_t)new std::string(std::to_string(bit_cast<char>(value))));
    case S:
    {
        auto s = *bit_cast<std::string*>(value);
        return Dynamic(standartTypes[S], (size_t)new std::string(s));
    }
        return Dynamic(standartTypes[S], (size_t)new std::string(*bit_cast<std::string*>(value)));
    case B:
        return Dynamic(standartTypes[S], value ? (size_t)new std::string("TRUE") : (size_t)new std::string("FALSE"));
    default:
        throw std::runtime_error("Unknown type");
    }
}

data_tree::Dynamic data_tree::callB(StandartType type, size_t value)
{
    switch (type)
    {
    case U:
        return Dynamic(standartTypes[B], bool(bit_cast<size_t>(value)));
    case I:
        return Dynamic(standartTypes[B], bool(bit_cast<ptrdiff_t>(value)));
    case F:
        return Dynamic(standartTypes[B], bool(bit_cast<double>(value)));
    case C:
        return Dynamic(standartTypes[B], bool(bit_cast<char>(value)));
    case S:
        return Dynamic(standartTypes[B], *bit_cast<std::string*>(value) == "TRUE" ? true : (*bit_cast<std::string*>(value) == "FALSE" ? false : throw std::runtime_error("Unknown type")));
    case B:
        return Dynamic(standartTypes[B], value);
    default:
        throw std::runtime_error("Unknown type");
    }
}

data_tree::Dynamic data_tree::callT(StandartType type, size_t value)
{
    switch (type)
    {
    case T:
        return Dynamic(standartTypes[StandartType::T], bit_cast<size_t>(Tuple(bit_cast<Tuple>(value))));
    case A:
        return Dynamic(standartTypes[StandartType::T], bit_cast<size_t>(Tuple(bit_cast<Tuple>(value))));
    default:
        throw std::runtime_error("Unknown type");
    }
}

data_tree::Dynamic data_tree::callA(StandartType type, size_t value)
{
    switch (type)
    {
    case T:
        return Dynamic(standartTypes[StandartType::A], bit_cast<size_t>(Tuple(bit_cast<Tuple>(value))));
    case A:
        return Dynamic(standartTypes[StandartType::A], bit_cast<size_t>(Tuple(bit_cast<Tuple>(value))));
    default:
        throw std::runtime_error("Unknown type");
    }
}

data_tree::Dynamic data_tree::callO(StandartType type, size_t value)
{
    switch (type)
    {
    case O:
        return Dynamic(standartTypes[StandartType::O], bit_cast<size_t>(Object(bit_cast<Object>(value))));
    default:
        throw std::runtime_error("Unknown type");
    }
}

data_tree::Dynamic data_tree::callG(StandartType type, size_t value) { throw std::runtime_error("Not implemented"); return Dynamic(); }

data_tree::Dynamic data_tree::callFUN(StandartType type, size_t value)
{
    switch (type)
    {
    case O:
        return callO(type, value);
    default:
        throw std::runtime_error("Unknown type");
    }
}

data_tree::Dynamic data_tree::callPRINT(StandartType type, size_t value)
{
    switch (type)
    {
        case U:
            std::cout << bit_cast<size_t>(value) << std::endl;
            break;
        case I:
            std::cout << bit_cast<ptrdiff_t>(value) << std::endl;
            break;
        case F:
            std::cout << bit_cast<double>(value) << std::endl;
            break;
        case C:
            std::cout << bit_cast<char>(value) << std::endl;
            break;
        case S:
            std::cout << *bit_cast<std::string*>(value) << std::endl;
            break;
        case B:
            std::cout << (bit_cast<bool>(value) ? "TRUE" : "FALSE") << std::endl;
            break;
        case T:
            {
                Tuple& t = *(Tuple*)(&value);
                std::cout << "(\n";
                for (size_t i = 0; i < t.size(); i++)
                {
                    callPRINT((StandartType)toStandartType(((t)[i].type)), (t)[i].value);
                }
                std::cout << ")" << std::endl;
            }
            break;
        default:
            throw std::runtime_error("Unknown type");
    }
    return Dynamic(data_tree::standartTypes[U], 0);
}

data_tree::Dynamic data_tree::callINPUT(StandartType type, size_t value)
{
    switch (type)
    {
        case U:
            std::cout << bit_cast<size_t>(value);
            break;
        case I:
            std::cout << bit_cast<ptrdiff_t>(value);
            break;
        case F:
            std::cout << bit_cast<double>(value);
            break;
        case C:
            std::cout << bit_cast<char>(value);
            break;
        case S:
            std::cout << *bit_cast<std::string*>(value);
            break;
        case B:
            std::cout << (bit_cast<bool>(value) ? "TRUE" : "FALSE");
            break;
        case T:
            {
                Tuple& t = *(Tuple*)(&value);
                std::cout << "(\n";
                for (size_t i = 0; i < t.size(); i++)
                {
                    callPRINT((StandartType)toStandartType(((t)[i].type)), (t)[i].value);
                }
                std::cout << ")";
            }
            break;
        default:
            throw std::runtime_error("Unknown type");
    }

    std::string s;
    std::getline(std::cin, s);
    return data_tree::toDynamic(s);
}