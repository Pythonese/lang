#include "data_tree.h"

data_tree::Dynamic data_tree::toDynamic(const std::string &value)
{
    return data_tree::Dynamic(data_tree::standartTypes[S], (size_t)new std::string(value));
}

data_tree::Dynamic data_tree::toDynamic(Tuple value)
{
    return Dynamic(data_tree::standartTypes[data_tree::StandartType::T], (size_t)Tuple(std::move(value)).arr);
}

data_tree::Dynamic data_tree::toDynamic(HeapArray value)
{
    return Dynamic(data_tree::standartTypes[data_tree::StandartType::A], (size_t)HeapArray(std::move(value)).arr);
}

data_tree::Dynamic data_tree::toDynamic(void *astObject)
{
    return Dynamic(data_tree::standartTypes[data_tree::StandartType::O], (size_t)new Object(astObject));
}

data_tree::Dynamic data_tree::toDynamic(PrimitiveType type, const std::string &value)
{
    switch (type)
    {
    case PrimitiveType::U:
        return data_tree::Dynamic(data_tree::standartTypes[U], std::stoull(value));
        break;
    case PrimitiveType::I:
        return data_tree::Dynamic(data_tree::standartTypes[I], std::stoll(value));
        break;
    case PrimitiveType::F:
        return data_tree::Dynamic(data_tree::standartTypes[F], std::stod(value));
        break;
    case PrimitiveType::C:
        return data_tree::Dynamic(data_tree::standartTypes[C], (size_t)new char(value[0]));
        break;
    case PrimitiveType::S:
        return data_tree::Dynamic(data_tree::standartTypes[S], (size_t)new std::string(value));
        break;
    case PrimitiveType::B:
        return data_tree::Dynamic(data_tree::standartTypes[B], value == "FALSE" ? false : (value == "TRUE" ? true : throw std::runtime_error("Invalid boolean value")));
        break;
    default:
        throw std::runtime_error("Unknown type");
        break;
    }
}

std::string data_tree::standartTypeToString(size_t type)
{
    switch (type)
    {
    case U:
        return "U";
        break;
    case I:
        return "I";
        break;
    case F:
        return "F";
        break;
    case C:
        return "C";
        break;
    case S:
        return "S";
        break;
    case B:
        return "B";
        break;
    case StandartType::T:
        return "T";
        break;
    case StandartType::A:
        return "A";
        break;
    case StandartType::O:
        return "O";
        break;
    case StandartType::G:
        return "G";
        break;
    case StandartType::FUN:
        return "FUN";
        break;
    case StandartType::PRINT:
        return "PRINT";
        break;
    case StandartType::INPUT:
        return "INPUT";
        break;
    default:
        break;
    }
}

size_t data_tree::getStandartType(std::string name)
{
    if (name == "U") return (U);
    if (name == "I") return (I);
    if (name == "F") return (F);
    if (name == "C") return (C);
    if (name == "S") return (S);
    if (name == "B") return (B);
    if (name == "T") return data_tree::StandartType::T;
    if (name == "A") return data_tree::StandartType::A;
    if (name == "O") return data_tree::StandartType::O;
    if (name == "G") return data_tree::StandartType::G;
    if (name == "FUN") return data_tree::StandartType::FUN;
    if (name == "PRINT") return data_tree::StandartType::PRINT;
    if (name == "INPUT") return data_tree::StandartType::INPUT;
    throw std::runtime_error("Invalid standart type");
}

bool data_tree::isStandartType(std::string name)
{
    return getStandartType(name) != (data_tree::StandartType)-1;
}

data_tree::Dynamic data_tree::callStandartFun(Dynamic *fun, Dynamic value)
{
    switch (fun->value)
    {
    case 0:
        return callU((StandartType)toStandartType(value.type), value.value);
        break;
    case 1:
        return callI((StandartType)toStandartType(value.type), value.value);
        break;
    case 2:
        return callF((StandartType)toStandartType(value.type), value.value);
        break;
    case 3:
        return callC((StandartType)toStandartType(value.type), value.value);
        break;
    case 4:
        return callS((StandartType)toStandartType(value.type), value.value);
        break;
    case 5:
        return callB((StandartType)toStandartType(value.type), value.value);
        break;
    case 6:
        return callT((StandartType)toStandartType(value.type), value.value);
        break;
    case 7:
        return callA((StandartType)toStandartType(value.type), value.value);
        break;
    case 8:
        return callO((StandartType)toStandartType(value.type), value.value);
        break;
    case 9:
        return callG((StandartType)toStandartType(value.type), value.value);
        break;
    case 10:
        return callFUN((StandartType)toStandartType(value.type), value.value);
        break;
    case 11:
        return callPRINT((StandartType)toStandartType(value.type), value.value);
        break;
    case 12:
        return callINPUT((StandartType)toStandartType(value.type), value.value);
        break;
    default:
        throw std::runtime_error("Unknown type");
        break;
    }
}

data_tree::Dynamic *data_tree::getGrandestParent(Dynamic* value)
{
    while (1)
    {
        if (value->type == standartTypes[StandartType::FUN] && value->value <= 12) return value;
        value = value->type;
    }
}

size_t data_tree::toStandartType(Dynamic *type)
{
    return type->value;
}
