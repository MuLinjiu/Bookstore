//
// Created by 18382 on 2021/1/21.
//

#ifndef NODELIST_CPP_CAMMAND_USER_H
#define NODELIST_CPP_CAMMAND_USER_H
#include <iostream>
#include <cstring>
#include <vector>
#include<stack>
#include"nodelist.h"
#include "Bookstore.h"

#define TOTLEMONEY_FILE "totalmoney.dat"
#define TRANSACTION_FILE "transaction.dat"//记录每一笔交易
#define BOOK_FILE "book_file.dat"//记录书的文件
#define USER "user.dat"
#define USER_ID_LIST_FILE "user_id.dat"

#define ISBN_FILE "isbn.dat"

#define AUTHOR_FILE "author.dat"

#define NAME_FILE "name.dat"

#define KEYWORD_FILE "keyword.dat"


//enum type{ISBN,NAME,AUTHOR,KEYWORD,PRICE};

using namespace std;
class user{
public:
    int priority = -1;
    char user_id[31];
    char password[31];
    char name[31];
    int select = -1;
    user(){};
    user(const string&user_id,const string &pass,const string &name, int pri);
};

void login(const string &user_id_,string password = "");



void logout();


void register_(user &U);
void addacount(user &a);

void deleteaccount(const char* userid);

void changepassword(const char* user_id,const char * newpas,const char* oldpas = "");


void selectbook(const string & ISBN_);


void import(int quantity, double totleprice);


void buy(const string &ISBN_,int quantity_);


void showfinance();


void showfinancetime(int times);


void show();



void show(const string &key,nodelist &a);


void modifyISBN(const string &key);



void modifyNAME(const string &key);



void modifyAUTHOR(const string &key);




void modifyKEYWORD(const string &key);




void modifyPRICEC(double price);


void Run_Program(string &a);

void splitkey(string str,vector<string>&a);

void clear();

int getselect();

template <class T>
T my_read(const string&  filename,int offset){
    fstream fin;
    fin.open(filename,ios::in|ios::binary);
    if(!fin)throw("error");
    T tmp;
    fin.seekg(offset);
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(T));
    fin.close();
    return tmp;
}

template <class T>
int my_write(const string& filename,T &o,int offset = -1){
    fstream fin;
    fin.open(filename,ios::in | ios::binary | ios::out);
    if(!fin)throw("cannot open the file(my_write)");
    if(offset < 0){
        fin.seekg(0,ios::end);
        offset = fin.tellg();
    }else{
        fin.seekg(offset);
    }
    fin.write(reinterpret_cast<char *>(&o),sizeof(T));
    fin.close();
    return offset;
}
#endif //NODELIST_CPP_CAMMAND_USER_H
