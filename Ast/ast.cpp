#include "ast.h"

void ast::Constant::exec()
{
}

void ast::Variable::exec()
{
}

void ast::Variable::call(data_tree::Dynamic arg)
{
    if (data_tree::isStandartType(name))
    {
        returnValue = data_tree::callStandartFun(data_tree::standartTypes[data_tree::getStandartType(name)], arg);
        return;
    }
    ((Object*)(parentWhereDeclared->getVariable(name)->value))->call(arg);
    returnValue = ((Object*)(parentWhereDeclared->getVariable(name)->value))->getReturnValue();
}

void ast::Tuple::exec()
{
    if (nodes.size() == 1)
    {
        nodes[0]->exec();
        value = nodes[0]->getValue();
        return;
    }
    value.value = (size_t)data_tree::Tuple(nodes.size()).arr;
    value.type = data_tree::standartTypes[data_tree::StandartType::T];
    for (size_t i = 0; i < nodes.size(); i++)
    {
        nodes[i]->exec();
        (*(data_tree::Tuple*)(&value.value))[i] = nodes[i]->getValue();
    }
}

void ast::Array::exec()
{
    value.value = (size_t)data_tree::HeapArray(nodes.size()).arr;
    value.type = data_tree::standartTypes[data_tree::StandartType::A];
    for (size_t i = 0; i < nodes.size(); i++)
    {
        nodes[i]->exec();
        (*(data_tree::HeapArray*)&value)[i] = nodes[i]->getValue();
    }
}

void ast::Object::exec()
{
    data_tree::Dynamic* dataObj = currentDataObject;
    currentDataObject = &value;
    Object* codeObj = currentCodeObject;
    currentCodeObject = this;
    value.value = (size_t)new data_tree::Object(this);
    value.type = data_tree::standartTypes[data_tree::StandartType::O];
    for (size_t i = 0; i < operators.size(); i++)
    {
        operators[i]->exec();
    }
    currentDataObject = dataObj;
    currentCodeObject = codeObj;
}

void ast::Generic::exec()
{
    value.value = (size_t)data_tree::Generic(variables.size()).arr;
    value.type = data_tree::standartTypes[data_tree::StandartType::A];
    for (size_t i = 0; i < variables.size(); i++)
    {
        (*(data_tree::HeapArray*)&value)[i] = variables[i]->getValue();
    }
}

void ast::Colon::exec()
{
    if (Variable* left = dynamic_cast<Variable*>(getLeft()))
    {
        if (Variable* right = dynamic_cast<Variable*>(getRight()))
        {
            currentDataObject->declare(left->getName(), currentCodeObject->getVariable(right->getName()));
        }
    }
    else throw std::runtime_error("Invalid colon operator");
}

void ast::Dot::exec()
{
    if (Variable* right = dynamic_cast<Variable*>(getRight()))
    {
        value = currentDataObject->getAttr(right->getName());
    }
    else throw std::runtime_error("Invalid dot operator");
}

void ast::Assign::exec()
{
    if (Variable* left = dynamic_cast<Variable*>(getLeft()))
    {
        getRight()->exec();
        *currentDataObject->getAttr(left->getName()) = data_tree::callStandartFun(currentCodeObject->getVariable(left->getName())->type, getRight()->getValue());
    }
    else throw std::runtime_error("Invalid assign operator");
}

void ast::ColonAssign::exec()
{
    if (Variable* left = dynamic_cast<Variable*>(getLeft()))
    {
        currentDataObject->declare(left->getName(), getRight()->getType());
        *currentDataObject->getAttr(left->getName()) = getRight()->getValue();
    }
    else throw std::runtime_error("Invalid colon-assign operator");
}

void ast::Call::exec()
{
    getRight()->exec();
    getLeft()->call(getRight()->getValue());
    returnValue = getLeft()->getReturnValue();
}

void ast::Backslash::exec()
{
}
