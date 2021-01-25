//
// Created by 18382 on 2021/1/21.
//

#include "Bookstore.h"
#include <iomanip>
#include <fstream>
#include <cstring>
extern nodelist USER_ID_LIST;

using namespace std;
Book::Book() {}
Book::Book(double _price, int _quantity, const string &ISBN_, const string &_name, const string &_author,
           const string &_keyword) {
    price = _price;
    strcpy(ISBN,ISBN_.c_str());
    strcpy(name,_name.c_str());
    strcpy(author,_author.c_str());
    strcpy(key_word,_keyword.c_str());
    quantity = _quantity;
}

bool Book::operator<(Book &a) {
    int c = strcmp(this->ISBN,a.ISBN);
    if(c < 0)return true;
    else return false;
}



void Book::show() const {
    cout<<ISBN<<"\t"<<name<<"\t"<<author<<"\t"<<key_word<<"\t";
    printf("%.2lf\t%d\n",price,quantity);

}

transaction::transaction() = default;

transaction::transaction(const string & ISBN_,const string & user_id_,int quant, double price){
    strcpy(ISBN,ISBN_.c_str());
    strcpy(user_id,user_id_.c_str());
    quantity = quant;
    totalprice = price;
}

totlemoney::totlemoney() {
    expense = 0;
    benefit = 0;
}


void initialize(){
//注意初始化totlemoney进入文件
    fstream fin,fout;
    fin.open(USER_ID_LIST_FILE,ios::in);
    if(!fin){
        fin.open(USER_ID_LIST_FILE,ios::out);
        fin.close();
        fin.open(USER,ios::out);
        fin.close();
        fin.open(TOTLEMONEY_FILE,ios::out);
        fin.close();
        fin.open(TRANSACTION_FILE,ios::out);
        fin.close();
        fin.open(BOOK_FILE,ios::out);
        fin.close();
        fin.open(ISBN_FILE,ios::out);
        fin.close();
        fin.open(AUTHOR_FILE,ios::out);
        fin.close();
        fin.open(NAME_FILE,ios::out);
        fin.close();
        fin.open(KEYWORD_FILE,ios::out);
        fin.close();
        user root("root","sjtu","root",7);
        int offset = my_write<user>(USER,root);
        node tmp(offset,"root");
        USER_ID_LIST.addnode(tmp);



        totlemoney tmpq;
        fout.open(TOTLEMONEY_FILE,ios::in | ios ::out | ios::binary);
        fout.seekp(0);
        fout.write(reinterpret_cast<char*>(&tmpq),sizeof(totlemoney));
        fout.close();
    }else{
        fin.close();
    }
}


bool mycmp(Book a,Book b){
    return (a < b);
}