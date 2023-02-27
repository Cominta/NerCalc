#include "lexAnalyser.h"
#include <stack>

LexAnalyser::LexAnalyser()
{

}

LexAnalyser::~LexAnalyser()
{
    
}

bool LexAnalyser::checkValidBrack(std::string str)
{
    std::stack<char> stack;
    std::map<char, char> symbols = {
        {')', '('},
        {'}', '{'},
        {']', '['}
    };

    for (int i = 0; i < str.size(); i++)
    {
        switch (str[i])
        {
            case '(':
                stack.push('(');
                break;

            case '[':
                stack.push('[');
                break;

            case '{':
                stack.push('{');
                break;
        }

        if (str[i] == ')' || str[i] == ']' || str[i] == '}')
        {
            if (stack.empty() || symbols[str[i]] != stack.top())
            {
                return false;
            }

            stack.pop();
        }
    }

    return stack.empty();
}

std::vector<LexAnalyser::Token*> LexAnalyser::analyseString(std::string inputString, int index)
{
    if (!this->checkValidBrack(inputString))
    {
        throw 0;
    }

    int i = index;
    std::vector<LexAnalyser::Token*> output;

    while (inputString[i] != '$')
    {
        LexAnalyser::Token* currentToken = new LexAnalyser::Token();

        if (isdigit(inputString[i]))
        {
            double num = inputString[i] - '0';

            long long int divideToDoubleCof = 1;
            bool divideToDouble = false;

            int numToSci; // кооф для научной нотации
            bool toSci = false;

            i++;

            while (isdigit(inputString[i]) || inputString[i] == '.' || inputString[i] == 'e')
            {
                if (divideToDouble)
                {
                    divideToDoubleCof *= 10;
                }

                if ((inputString[i] == '.' && (divideToDouble || toSci)) || (inputString[i] == 'e' && (divideToDouble || toSci || !isdigit(inputString[i + 1]))))
                {
                    throw 0;
                }

                else if (inputString[i] == '.')
                {
                    divideToDouble = true;
                    i++;
                    continue;
                }

                else if (inputString[i] == 'e')
                {
                    numToSci = inputString[i + 1] - '0';
                    toSci = true;
                    i += 2;
                    continue;
                }

                if (toSci)
                {
                    numToSci *= 10;
                    numToSci += inputString[i] - '0';
                }

                else
                {
                    num *= 10;
                    num += inputString[i] - '0';
                }

                i++;
            }
            
            if (toSci)
            {
                currentToken->value = num * pow(10, numToSci);
            }

            else
            {
                currentToken->value = num / divideToDoubleCof;
            }

            currentToken->tType = LexAnalyser::TokenType::NUM;
        }

        else if (isalpha(inputString[i]))
        {
            currentToken->tType = TokenType::VAR;
            currentToken->var = inputString[i];
            i++;
        }

        else if (inputString[i] == ' ')
        {
            i++;
            delete currentToken;
            continue;
        }

        else 
        {
            if (output.empty() || ((output.back()->tType == LexAnalyser::TokenType::MULTIPLY || output.back()->tType == LexAnalyser::TokenType::DIVIDE ||
                output.back()->tType == LexAnalyser::TokenType::PLUS || output.back()->tType == LexAnalyser::TokenType::SUB)
                && this->mapTokens[inputString[i]] != LexAnalyser::TokenType::LB))
            {
                delete currentToken;
                throw 0;
            }

            else
            {
                currentToken->tType = this->mapTokens[inputString[i]];
                i++;
            }
        }

        output.push_back(currentToken);
    }

    LexAnalyser::Token* endToken = new Token();
    endToken->tType = LexAnalyser::TokenType::END;
    output.push_back(endToken);

    return output;
}
