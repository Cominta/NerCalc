#include "parser.h"

int Parser::getBaseNum(double& num)
{
    int count = 0;
    bool zero = false;

    while (std::isdigit(this->next) && this->next != '\0')
    {
        count++;
        num *= 10;
        num += (int)this->next - 48;

        if ((num == 0 && zero == true))
        {
            throw WrongNums();
        }

        if (num == 0)
        {
            zero = true;
        }

        this->nextToken();

    }

    return count;
}

double Parser::getNum()
{
    double num = (int)this->next - 48;

    this->nextToken();

    if (num == 0 && std::isdigit(this->next))
    {
        throw WrongNums();
    }

    this->getBaseNum(num);

    if (this->next == 'e')
    {
        double coof = 0;
        bool negative = false;

        this->nextToken();

        if (this->next == '-')
        {
            negative = true;
            this->nextToken();
        }

        else if (this->next == '+')
        {
            this->nextToken();
        }

        else if (!std::isdigit(this->next))
        {
            return num;
        }

        double oldCoof = coof;

        this->getBaseNum(coof);

        //if (coof == oldCoof)
        //{
        //    if (this->next == 'e')
        //    {
        //        this->nextToken();
        //    }

        //    return num;
        //}

        if (negative)
        {
            coof = -coof;
        }

        return num * pow(10, coof);
    }

    else if (this->next == '.')
    {
        double floatPart = 0;
        this->nextToken();
        int count = this->getBaseNum(floatPart);
        floatPart /= pow(10, count);

        if (this->next == '.')
        {
            throw WrongExpr();
        }

        return num + floatPart;
    }

    return num;
}

ASTItems::Token* Parser::expr()
{
    ASTItems::Token* a = this->term();

    while (true)
    {
        if (this->next == '+')
        {
            this->nextToken();
            ASTItems::Token* b = this->term();

            a = new ASTItems::Plus(a, b);
        }

        else if (this->next == '-')
        {
            this->nextToken();
            ASTItems::Token* b = this->term();

            a = new ASTItems::Minus(a, b);
        }

        else
        {
            return a;
        }
    }
}

ASTItems::Token* Parser::term()
{
    ASTItems::Token* a = this->factor();

    if (this->next == '*')
    {
        this->nextToken();
        ASTItems::Token* b = this->term();

        return new ASTItems::Multi(a, b);
    }

    else if (this->next == '/')
    {
        this->nextToken();
        ASTItems::Token* b = this->term();

        return new ASTItems::Divide(a, b);
    }

    else
    {
        return a;
    }
}

ASTItems::Token* Parser::factor()
{
    if (std::isalpha(this->next))
    {
        if (this->next == 's' && this->currentStr[this->currentIndex] == 'q' && 
            this->currentStr[this->currentIndex + 1] == 'r' && this->currentStr[this->currentIndex + 2] == 't')
        {
            this->nextToken();
            this->nextToken();
            this->nextToken();
            this->nextToken();

            if (this->next == '(')
            {
                return new ASTItems::Num(sqrt(this->brackets()->eval()));
            }

            throw WrongExpr();
        }

        char var = this->next;
        this->nextToken();

        if (this->variables.find(var) != this->variables.end())
        {
            return new ASTItems::Num(this->variables[var]);
        }

        throw WrongExpr();
    }

    else if (std::isdigit(this->next))
    {
        double num = this->getNum();

        if (this->next == '(' || this->next == 'e')
        {
            throw WrongExpr();
        }

        //char num = this->next;
        return new ASTItems::Num(num);
    }

    else if (next == '(')
    {
        return this->brackets();
    }

    else if (this->next == '-') // unary -
    {
        this->nextToken();
        return new ASTItems::Negative(this->factor());
    }

    else
    {
        throw WrongExpr();
    }
}

ASTItems::Token* Parser::brackets()
{
    this->nextToken();
    ASTItems::Token* a = this->expr();

    if (a == nullptr)
    {
        throw WrongBrackets();
    }

    if (this->next == ')')
    {
        this->nextToken();
        return a;
    }

    throw WrongBrackets();
}

void Parser::nextToken()
{
    if (this->currentStr.size() < this->currentIndex)
    {
        return;
    }

    while (this->currentStr[this->currentIndex] == ' ')
    {
        this->currentIndex++;
    }

    this->next = this->currentStr[this->currentIndex];
    this->currentIndex++;
}

double Parser::parse(std::string str)
{
    if (str.size() > 0)
    {
        this->currentStr = str;
        this->currentIndex = 0;
        this->nextToken();

        this->tree = this->expr();

        if (this->tree == nullptr)
        {
            throw WrongExpr();
        }

        double result = this->tree->eval();
        delete this->tree;

        return result;
    }
}

Parser::~Parser()
{
    if (this->tree != nullptr)
    {
        delete this->tree;
    }
}
