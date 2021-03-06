//
// Created by 18382 on 2021/1/20.
//
#include "Cammand(user).h"
#include "nodelist.h"
#include <iostream>
#include <cstring>

node::node(const node &a) {
    offset = a.offset;
    strcpy(key,a.key);
}


node::node(int offset_, const string& key_) {
    offset = offset_;
    strcpy(key,key_.c_str());
}


bool node::operator<(const node &a) const {
    if(strcmp(key,a.key) < 0)return true;
    else return false;
}


node &node::operator=(const node &a){
    if(this == &a)return *this;
    else {
        offset = a.offset;
        strcpy(key,a.key);
        return *this;
    }
}


square::square() {
    next = -1;
    previous = -1;
    length = 0;
}

void nodelist::merge(int offset1, int offset2) {
    fstream fin,fout;
    fin.open(filename,ios::in | ios::binary);
    fout.open(filename,ios::out | ios :: in | ios ::binary);
    if(!fin || !fout)throw("error");
    square square1,square2 ;
    fout.open(filename,ios::out | ios :: in | ios ::binary);
    fout.seekg(offset1);
    fout.read(reinterpret_cast<char *>(&square1),sizeof(square));
    fout.seekg(offset2);
    fout.read(reinterpret_cast<char *>(&square2),sizeof(square));
    square1.next = square2.next;
    for(int i = 0 ; i < square2.length ; i++){
        square1.nodearray[i + square1.length] = square2.nodearray[i];
    }
    square1.length += square2.length;
    if(square2.next > 0){//还有块
        fin.seekp(square2.next + sizeof(int));//第二块后一块的previous
        fin.write(reinterpret_cast<char *>(&offset1),sizeof(int));
    }
    fin.seekp(offset1);
    fin.write(reinterpret_cast<char *>(&square1),sizeof(square));
    fin.close();
    fout.close();
}


void nodelist::split(int offset) {
    fstream fin,fout;
    fin.open(filename,ios::in | ios::binary);
    fout.open(filename,ios::out | ios :: in | ios ::binary);
    if(!fin || !fout)throw("error");
    square square1,square2,square_tmp;
    fout.seekg(offset);
    fout.read(reinterpret_cast<char *>(&square_tmp),sizeof(square));
    for(int i = 0 ; i <square_tmp.length ; i++)
    {
        if(i < split_remain ){
            square1.nodearray[i] = square_tmp.nodearray[i];
        }
        else{
            square2.nodearray[i - split_remain] = square_tmp.nodearray[i];
        }
    }
    square1.length = split_remain;
    square2.length = square_tmp.length - split_remain;
    fin.seekg(0,ios::end);
    int offset2 = fin.tellg();
    if(square_tmp.next >= 0) {
        fout.seekp(square_tmp.next + sizeof(int));
        fout.write(reinterpret_cast<char *>(&offset2), sizeof(int));
    }
    square2.next = square_tmp.next;
    square2.previous = offset;
    square1.next = offset2;
    square1.previous = square_tmp.previous;
    fout.seekp(offset);
    fout.write(reinterpret_cast<char*>(&square1),sizeof(square));
    fout.seekp(offset2);
    fout.write(reinterpret_cast<char *>(&square2),sizeof(square));
    fin.close();
    fout.close();
}


int nodelist::nextsquare(int offset) {
    fstream fin,fout;
    fin.open(filename,ios::in | ios::out | ios::binary);
    if(!fin)throw("error");
    fin.seekg(offset);
    int next;
    fin.read(reinterpret_cast<char *>(&next),sizeof(int));
    fin.close();
    return next;
}


void nodelist::addnode(node &o) {
    fstream fin,fout;
    fin.open(filename,ios::in | ios :: binary);
    fout.open(filename,ios::in | ios::out | ios::binary);
    if(!fin || !fout)throw("error");
    fin.seekg(0,ios::end);
    int a = fin.tellg();
    if(a == 0){
        square square_first;
        fout.seekp(0);
        fout.write(reinterpret_cast<char *>(&square_first),sizeof(square));
    }
    int cur = 0;
    int next = nextsquare(cur);
    while(next != -1){
        square tmp ;
        fin.seekg(next);
        fin.read(reinterpret_cast<char *>(&tmp),sizeof(square));
        if(strcmp(o.key,tmp.nodearray[0].key) < 0){
            break;
        }
        cur = next;
        next = nextsquare(cur);
    }
    fin.seekg(cur);
    square tmp;
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(square));
    bool flag = false;
    int x;
    fin.seekg(cur);
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(square));
    for (int i = 0; i < tmp.length; i++) {
        if (o < tmp.nodearray[i]) {
            x = i;
            flag = true;
            break;
        }
    }
    if(!flag) x = tmp.length;
    for(int i = tmp.length; i >= x + 1 ; i--)//递减（防止覆盖）
    {
        tmp.nodearray[i] = tmp.nodearray[i - 1];
    }
    tmp.nodearray[x] = o;
    tmp.length++;
    fout.seekp(cur);
    fout.write(reinterpret_cast<char*>(&tmp),sizeof(square));
    fin.close();
    fout.close();
    if(tmp.length >= square_size){
        split(cur);
    }
}


void nodelist::deletenode(node &o) {
    fstream fin,fout;
    fin.open(filename,ios::in | ios :: binary);
    fout.open(filename,ios::in | ios::out | ios::binary);
    if(!fin || !fout)throw("error");
    fin.seekg(0,ios::end);
    int a = fin.tellg();
    if(a == 0){
        return;
    }
    int cur = 0;
    int next = nextsquare(0);
    while(next != -1){
        square tmp;
        fin.seekg(next);
        fin.read(reinterpret_cast<char*>(&tmp),sizeof(square));
        if(strcmp(o.key,tmp.nodearray[0].key) < 0)break;
        square a1;
        fin.seekg(cur);
        fin.read(reinterpret_cast<char*>(&a1),sizeof(square));
    }
    square tmp;
    fin.seekg(cur);
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(square));
    bool flag = false;
    int x;
        for (int i = 0; i < tmp.length; i++) {
            if (strcmp(o.key, tmp.nodearray[i].key) == 0 && o.offset == tmp.nodearray[i].offset) {
                x = i;
                flag = true;
                break;
            }
        }
    if(!flag){
        return;
    }
    for(int i = x ; i <= tmp.length - 2; i++)
    {
        tmp.nodearray[i] = tmp.nodearray[i + 1];
    }
    tmp.length--;
    fout.seekp(cur);
    fout.write(reinterpret_cast<char*>(&tmp),sizeof(square));
    next = nextsquare(cur);
    int nextlen = 0;
    if(next != -1) {
        fin.seekg(next + 2 * sizeof(int));
        fin.read(reinterpret_cast<char *>(&nextlen), sizeof(int));
    }
    fin.close();
    fout.close();
    if(tmp.length + nextlen <= merge_max_lenth && next != -1)merge(cur,next);
}

void nodelist::findnode(const string & key_, vector<int>&possibileoffset){
    fstream fin,fout;
    fin.open(filename,ios::in | ios :: binary);
    fout.open(filename,ios::in | ios::out | ios::binary);
    if(!fin || !fout)throw("error");
    fin.seekg(0,ios::end);
    int a = fin.tellg();
    fin.close();
    if(a == 0){
        square tmp;
        fout.seekp(0);
        fout.write(reinterpret_cast<const char *>(&tmp), sizeof(square));
        fout.close();
    }
    fin.open(filename,ios::in | ios :: binary);
    int cur = 0;
    int next = nextsquare(0);
    int pre = 0;
    while(next >= 0){
        square tmp;
        fin.seekg(next);
        fin.read(reinterpret_cast<char*>(&tmp),sizeof(square));
        if(strcmp(key_.c_str(),tmp.nodearray[0].key) < 0)break;
        square a1;
        fin.seekg(cur);
        fin.read(reinterpret_cast<char*>(&a1),sizeof(square));
        if(strcmp(key_.c_str(),tmp.nodearray[0].key) > 0)pre = cur;
        cur = next;
        next = nextsquare(cur);
    }
    while(true){
        square tmp;
        fin.seekg(pre);
        fin.read(reinterpret_cast<char*>(&tmp),sizeof(square));
        for (int i = 0; i < tmp.length; i++) {
            if (strcmp(key_.c_str(),tmp.nodearray[i].key) == 0) {
                possibileoffset.push_back(tmp.nodearray[i].offset);
            }
        }
        if(pre == next)break;
        pre = nextsquare(pre);
    }
    fin.close();
}


nodelist::nodelist(const string &file_) {
    strcpy(filename,file_.c_str());
}
