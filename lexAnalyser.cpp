#include "lexAnalyser.h"

LexAnalyser::LexAnalyser()
{

}

LexAnalyser::~LexAnalyser()
{
    
}

std::vector<LexAnalyser::Token*> LexAnalyser::analyseString(std::string inputString, int index)
{
    int i = index;
    std::vector<LexAnalyser::Token*> output;

    while (inputString[i] != '$')
    {
        LexAnalyser::Token* currentToken = new LexAnalyser::Token();

        if (isdigit(inputString[i]))
        {
            double num = inputString[i] - '0';
            i++;

            while (isdigit(inputString[i]))
            {
                num *= 10;
                num += inputString[i] - '0';
                i++;
            }
            
            currentToken->value = num;
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
