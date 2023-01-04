#ifndef LEX_ANALYSER_H
#define LEX_ANALYSER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

class LexAnalyser
{
    public:
        enum class TokenType
        {
            NUM,
            PLUS,
            SUB,
            DIVIDE,
            MULTIPLY,
            LB,
            RB,
            END,
            VAR,
            UNKNOWN
        };

    private:
        std::map<char, TokenType> mapTokens = {
            {'+', TokenType::PLUS},
            {'-', TokenType::SUB},
            {'/', TokenType::DIVIDE},
            {'*', TokenType::MULTIPLY},
            {'(', TokenType::LB},
            {')', TokenType::RB}
        };


    public:
        struct Token
        {
            TokenType tType;
            double value = 0;
            char var = NULL;
        };

        std::map<char, int> variables;

        LexAnalyser();
        ~LexAnalyser();

        std::vector<Token*> analyseString(std::string inputString, int index);
};

#endif 
