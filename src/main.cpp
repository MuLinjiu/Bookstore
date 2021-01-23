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
            cout<<"INVALID\n";
        }
    }
    return 0;
}