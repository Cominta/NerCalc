#pragma once

#include <iostream>

namespace ASTItems
{
    // Tree

    // super-struct (super-class)
    struct Token
    {
        virtual void print() = 0;
        virtual double eval() = 0;

        virtual ~Token() {}
    };

    // Binary operation
    struct Plus : Token
    {
        Token* left;
        Token* right;

        Plus(Token* left, Token* right) : left(left), right(right) {}
        ~Plus()
        {
            if (this->left != nullptr)
            {
                delete this->left;
            }

            if (this->right != nullptr)
            {
                delete this->right;
            }
        }

        void print()
        {
            left->print();
            std::cout << " + ";
            right->print();
        }

        double eval()
        {
            return this->left->eval() + this->right->eval();
        }
    };

    struct Minus : Token
    {
        Token* left;
        Token* right;

        Minus(Token* left, Token* right) : left(left), right(right) {}
        ~Minus()
        {
            if (this->left != nullptr)
            {
                delete this->left;
            }

            if (this->right != nullptr)
            {
                delete this->right;
            }
        }

        void print()
        {
            left->print();
            std::cout << " - ";
            right->print();
        }

        double eval()
        {
            return this->left->eval() - this->right->eval();
        }
    };

    struct Multi : Token
    {
        Token* left;
        Token* right;

        Multi(Token* left, Token* right) : left(left), right(right) {}
        ~Multi()
        {
            if (this->left != nullptr)
            {
                delete this->left;
            }

            if (this->right != nullptr)
            {
                delete this->right;
            }
        }

        void print()
        {
            left->print();
            std::cout << " * ";
            right->print();
        }

        double eval()
        {
            return this->left->eval() * this->right->eval();
        }
    };

    struct Divide : Token
    {
        Token* left;
        Token* right;

        Divide(Token* left, Token* right) : left(left), right(right) {}
        ~Divide()
        {
            if (this->left != nullptr)
            {
                delete this->left;
            }

            if (this->right != nullptr)
            {
                delete this->right;
            }
        }

        void print()
        {
            left->print();
            std::cout << " / ";
            right->print();
        }

        double eval()
        {
            return this->left->eval() / this->right->eval();
        }
    };

    // Unary operation
    struct Negative : Token
    {
        Token* down; // node bellow 

        Negative(Token* down) : down(down) {}
        ~Negative()
        {
            if (this->down != nullptr)
            {
                delete this->down;
            }
        }

        void print()
        {
            std::cout << "-(";
            down->print();
            std::cout << ")";
        }

        double eval()
        {
            return -(this->down->eval());
        }
    };

    // Just number
    struct Num : Token
    {
        double value;

        Num(double value) : value(value) {}

        void print()
        {
            std::cout << this->value;
        }

        double eval()
        {
            return this->value;
        }
    };

    // Tree
}
