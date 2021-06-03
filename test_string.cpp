#include <iostream>
#include <queue>
#include <set>
#include <cstring>
#include <stack>
#include <cctype>
using namespace std;

int main(){

    //incializando el set de operadores para comparación
    std::set<char> operators_chars;
    char const * CharList = "*/+-";
    operators_chars.insert(CharList, CharList + strlen(CharList));

    string token= "g+la"; //se almacenará la línea
    string::const_iterator it = token.begin();

    if(*(it-1)){
        cout<<"hay algo"<<endl;
    }else
        cout<<" no hay nada"<<endl;
    it++;
    if (it == token.begin() || (operators_chars.find(*(it)) != operators_chars.end() || *it == '(') ){
        cout<<"Toy al principio del string"<<endl;
    }
    
    
    while(it != token.end()){
        cout<<*it;
        it++;
    }

    if(*(it++)){
        cout<<"hay algo"<<endl;
    }else
        cout<<" no hay nada"<<endl;


    return 0;
}