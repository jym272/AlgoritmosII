#include <iostream>
#include <queue>
#include <set>
#include <cstring>
/*
  string::const_iterator it = arg.begin();
    while(it != arg.end() && std::isdigit(*it))
        ++it;
    if (!arg.empty() && it == arg.end()){
        precision.isSet=true;
        precision.value = std::stoi(arg); // transformamos a entero

*/
using namespace std;

int main(){

    string token = "6*6-8";
    string::const_iterator it = token.begin();
    queue<char> output;
    
    //incializando el set de operadores
    std::set<char> chars;
    char const * CharList = "+-*/()";
    chars.insert(CharList, CharList + strlen(CharList));
    
    while(it != token.end()){ //puedo validar con un && entrada de caracteros no numericos ni los simbolos especiales

        if(std::isdigit(*it)){ 
            cout<<"es un digito"<<endl;
            output.push(*it);
        }else if(chars.find(*it) != chars.end()){ //si es un operador
            cout<<"es un operardor"<<endl;
            while("")
        }
        ++it;
    }

    while (!output.empty()) {
        cout << ' ' << output.front();
        output.pop();
        }

    cout<<endl;
    


    return 0;
}