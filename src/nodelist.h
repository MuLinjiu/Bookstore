//
// Created by 18382 on 2021/1/20.
//
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>

#define square_size 300
#define  split_remain 150
#define max_key_length 80
#define merge_max_lenth 290
using namespace std;
#ifndef MAIN_CPP_NODELIST_H
#define MAIN_CPP_NODELIST_H

class node{
public:
    int offset;
    char key[81];
    explicit node(int offset_ = -1, const string& key_ = "");
    node(const node &a);
    bool operator<(const node &a) const;
    node &operator = (const node &a);
};

class square{
public:
    int next = -1;
    int previous = -1;
    int length = 0;
    node nodearray[square_size];
    square();
};
class nodelist{
public:
    char filename[100]{};
    fstream fin,fout;
    int nextsquare(int offset);
    void merge(int offset1,int offset2);
    void split(int offset);
    void addnode(node & o);
    void deletenode(node & o);
    explicit nodelist(const string &file_);
    void findnode(const string & key_, vector<int>possibileoffset);//借鉴夏天
};
#endif //MAIN_CPP_NODELIST_H
