//
// Created by 18382 on 2021/1/21.
//

#ifndef NODELIST_CPP_BOOKSTORE_H
#define NODELIST_CPP_BOOKSTORE_H
#include <iostream>
#include <cstring>
#include <vector>
#include<stack>
#include "Cammand(user).h"



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
    bool operator < (Book &a);
    void show() const;
};


class transaction{
public:
    char dealtime[30] = {0};
    char user_id[30] = {0};
    char ISBN[30] = {0};
    int quantity = 0;
    double totalprice = 0;//正数为收入，负数为支出
    transaction();
    transaction(const string & ISBN_,const string & user_id_,int quant, double price);

};


class totlemoney{
public:
    double benefit = 0;
    double expense = 0;
    totlemoney();
};

void initialize();

bool mycmp(Book a,Book b);


#endif //NODELIST_CPP_BOOKSTORE_H
