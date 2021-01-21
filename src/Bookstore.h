//
// Created by 18382 on 2021/1/21.
//

#ifndef NODELIST_CPP_BOOKSTORE_H
#define NODELIST_CPP_BOOKSTORE_H
#include <iostream>
#include <cstring>
#include <vector>
#include<stack>
#include<nodelist.h>


using namespace std;

class Book{
public:
    double price = 0;
    int quantity = 0;
    char ISBN[20] = {0};
    char name[60] = {0};
    char key_word[60] = {0};
    char author[60] = {0};
    Book();
    Book(double _price, int _quantity, const string &ISBN_, const string &_name, const string &_author, const string &_keyword);

    void show() const;
};


void initialize();




#endif //NODELIST_CPP_BOOKSTORE_H
