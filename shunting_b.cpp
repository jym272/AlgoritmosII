#include <iostream>
#include <queue>
#include <set>
#include <cstring>
#include <stack>

#include "bignum.h"

using namespace std;

int hasgreaterPrecedence(char it, char * top){
	//la precedencia es: * / -->2
	//					 + - ..>1
	if (*top == '*' || *top == '/'){
		return 1; // porque el tpo siempre tendrá mayor precedencia
	}else{ //el top será un mas o un menos
		if(it =='+' || it =='-') //es decir, si el top y el it tienen igual precedencia
			return 1;
		else
			return 0; //el it es un * o /
	}
}

char RPNtoDouble(queue<char> *output){
		stack<char> resultado;

        while(!output->empty()) 
        {
            // If the token is a value push it onto the stack
            if (std::isdigit(output->front())) 
            {
				resultado.push(output->front());    
				output->pop();            
            }
            else
            {
                // Token is an operator: pop top two entries
				cout<<"La operaciones es :"<< output->front()<<endl;
				char d1 = resultado.top();
				resultado.pop();
				int digit1 = d1 - '0';
				cout<< digit1<<endl;
				char d2 = resultado.top();
				resultado.pop();
				int digit2 = d2 - '0';
                cout<< digit2<<endl;
                //Get the result
	            int result = output->front() == '+' ? digit2 + digit1 : 
                             output->front() == '-' ? digit2 - digit1 :
                             output->front() == '*' ? digit2 * digit1 :
                                                      digit2 / digit1 ;               
                                 
                // Push result onto stack
				cout<< result<<endl;
                resultado.push(result + '0');    
				output->pop();                                                        
            }                        
        }        
         
        return resultado.top();
}


// --->  128222227961264 * (11374617 - 1897262 + 1874)

int main(){

    //string token = "2-(3-9*16/4)*3-6+6*(3-1+2)-6";
	string token = "(4-3)*6+3-7*(4/2-1)";
    string::const_iterator it = token.begin();
    
    queue<char> output;
    stack<char> operators;
    qu
    //incializando el set de operadores
    //en el set el orden importa 
    std::set<char> operators_chars;
    char const * CharList = "*/+-";
    operators_chars.insert(CharList, CharList + strlen(CharList));


    while(it != token.end()){ //puedo validar con un && entrada de caracteros no numericos ni los simbolos especiales

        if(std::isdigit(*it)){ 
            output.push(*it);
			//pushearlo todo hasta encontrar un operador o ()
        }else if(operators_chars.find(*it) != operators_chars.end()){ //si es un operador
            
			while(	!operators.empty()
					&&  hasgreaterPrecedence(*it, &operators.top())
					&&  ( operators.top() != '(') ){

						output.push(operators.top());
						operators.pop();
			
			}
            operators.push(*it);
        }else if(*it == '('){ //left parentesis
            operators.push(*it);
        }
        else if(*it == ')'){ //right parentesis
            while(operators.top() != '('){
				if (operators.empty()){ //una validacion
					cout<<"Error, stack runs out without finding a left parenthesis, then there are mismatched parentheses."<<endl;
					break;
				}else{
					output.push(operators.top());
					operators.pop();
				}
            }
			if(operators.top() == '('){
				operators.pop();
			}
        }

        ++it;
    }
	/* After while loop, if operator stack not null, 
	pop everything to output queue */
    while(!operators.empty()){
		// si encontre un parentesis en el top de la pila entonces hay mismatched parentesis
		if(operators.top() == '(' || operators.top() == ')'){
			cout<<"mismatched parentesis"<<endl;
			break;
		}
		output.push(operators.top());
		operators.pop();
	}

	cout<<RPNtoDouble(&output);
	/*
    //imprimiendo el queue output:
    while (!output.empty()) {
        cout << ' ' << output.front();
        output.pop();
        }


	*/
    cout<<endl;
    


    return 0;
}