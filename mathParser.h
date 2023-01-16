#ifndef MATH_PARSER_H
#define MATH_PARSER_H

#include "lexAnalyser.h"
#include <stack>

class MathParser
{
    private:
        std::string inputString;
        int currentIndex;

        LexAnalyser* lexanalyser;

        void printExpr(std::vector<LexAnalyser::Token*> tokenArray); // чисто для теста
        
        void calculateStack(std::stack<double>& numbers, std::stack<LexAnalyser::TokenType>& op, LexAnalyser::TokenType currentOp = LexAnalyser::TokenType::END, bool RB = false);
        double calculate(std::vector<LexAnalyser::Token*> tokenArray);

        std::map<LexAnalyser::TokenType, int> priority = {
            {LexAnalyser::TokenType::PLUS, 1},
            {LexAnalyser::TokenType::SUB, 1},
            {LexAnalyser::TokenType::DIVIDE, 2},
            {LexAnalyser::TokenType::MULTIPLY, 2},
        };

    public:
        MathParser();
        ~MathParser();

        double parseString(std::string inputString, int index = 0);
        void addVariable(char name, std::string value);
        double getValueVariable(char name) {return this->lexanalyser->variables[name];};
};

#endif
