//
// Created by 18382 on 2021/1/24.
//

#include <iostream>
#include "Bookstore.h"
#include "Cammand(user).h"


using namespace std;
int main() {
    initialize();
    string command;
    while(getline(cin,command)){
        try{
            Run_Program(command);
        }catch (...){
            cout<<"Invalid\n";
        }
    }
    return 0;
}