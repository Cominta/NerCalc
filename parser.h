#pragma once

#include <iostream>
#include "AST.h"
#include <map>
#include <cmath>
#include <exception>
#include <QDebug>

/*
*	E -> T - T | T + T | T
*	T -> F * T | F / T | F
*	F -> VAR | DOUBLE | (E) | -F | sqrt(E)
*/

class WrongExpr : public std::exception
{
public:
    const char* what() const noexcept override { return "Wrong expression"; }
};

class WrongBrackets : public WrongExpr
{
public:
    const char* what() const noexcept override { return "Wrong brackets"; }
};

class WrongNums : public WrongExpr
{
public:
    const char* what() const noexcept override { return "Wrong numbers"; }
};

class EmptyLine : public WrongExpr
{
public:
    const char* what() const noexcept override { return "Empty Line"; }
};

class Parser
{
private:
    ASTItems::Token* tree = nullptr;
    std::map<char, double> variables;
    bool exp = false;

    std::string currentStr;
    int currentIndex;
    char next;

    int getBaseNum(double& num);
    double getNum();
    
    ASTItems::Token* expr(); // E
    ASTItems::Token* term(); // T
    ASTItems::Token* factor(); // F
    ASTItems::Token* brackets();

    void nextToken();

public:
    Parser() = default;
    ~Parser();

    double parse(std::string str);

    void setVar(char symbol, double value) { this->variables[symbol] = value; }
    double getValVar(char symbol) { return this->variables[symbol]; }
    bool getExp() const { return this->exp; }
};
