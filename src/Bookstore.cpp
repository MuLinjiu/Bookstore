//
// Created by 18382 on 2021/1/21.
//

#include "Bookstore.h"
#include <iomanip>
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
void Book::show() const {
    cout<<ISBN<<" "<<name<<" "<<author<<" "<<key_word<<" "<<setprecision(2)<<price<<" "<<quantity<<endl;
}