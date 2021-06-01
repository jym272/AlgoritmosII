#include <iostream>
#include <queue>
#include <set>
#include <cstring>
#include <stack>

#include "bignum.h"

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

bignum RPNtoDouble(queue<string> *output){
		stack<char> resultado;
		stack<bignum> resultado_b;

        while(!output->empty()) 
        {
            // If the token is a value push it onto the stack
            if (std::isdigit(output->front().at(0))) 
            {
				bignum bb(output->front(),output->front().length()); //---> full precision
				cout<<bb<<endl;
				resultado_b.push(bb);
				//resultado.push(output->front());    
				output->pop();            
            }
            else
            {
                // Token is an operator: pop top two entries
				std::cout<<"La operaciones es :"<< output->front().at(0)<<endl;
				bignum b1= resultado_b.top();
				resultado_b.pop();
				//char d1 = resultado.top();
				//resultado.pop();
				
				//int digit1 = d1 - '0';
				cout<< b1<<endl;
				bignum b2 = resultado_b.top();
				//char d2 = resultado.top();
				resultado_b.pop();
				//resultado.pop();
				//int digit2 = d2 - '0';
                cout<< b2<<endl;
                //Get the result
	            bignum result = output->front().at(0) == '+' ? b2 + b1 : 
                            	output->front().at(0) == '-' ? b2 - b1 :
														    b2 * b1 ; 
								/* 
                             	output->front().at(0) == '*' ? b2 * b1 :
                                                      b2 / b1 ;  */             
                                 
                // Push result onto stack
				cout<< result<<endl;
                resultado_b.push(result);
				//resultado.push(result + '0');    
				output->pop();                                                        
            }                        
        }        
         
        return resultado_b.top();
}

int main(){

    //string token = "2-(3-90*160*4)*301-6+6*(3-1+2)-6";
	//string token = "(4-3)*6+3-7*(4/2-1)";
	//string token = "(1589-5154)*1521+123156448-85484515451-8974984*(12151-554954*54654654-4565*(564564-85154*2))";
    string token = "51-51564-dwd54";
	string::const_iterator it = token.begin();
    
    queue<char> output;
	queue<string> output_s;
    stack<char> operators;


    //incializando el set de operadores
    //en el set el orden importa 
    std::set<char> operators_chars;
    char const * CharList = "*/+-";
    operators_chars.insert(CharList, CharList + strlen(CharList));

	
    while(it != token.end()){ //puedo validar con un && entrada de caracteros no numericos ni los simbolos especiales
		string bignum_s; //reseteo el string

        if (std::isdigit(*it)){ //si es digito 
			while(std::isdigit(*it)){ //acumulo todos lo digitos 
				bignum_s+=*it;
				++it;
			}
			//std::cout<<*it<<endl;
			output_s.push(bignum_s);
			if(it == token.end()){ // el siguiente token es el EOF
				cout<<"brak activarte"<<endl;
				break;
			}
		
		    //cout<<"Es un digito"<<endl;
			
			//output.push(*it);
			//pushearlo todo hasta encontrar un operador o ()
			// y pasarlo a un bignum.
        }
		if(operators_chars.find(*it) != operators_chars.end()){ //si es un operador
            
			while(	!operators.empty()
					&&  hasgreaterPrecedence(*it, &operators.top())
					&&  ( operators.top() != '(') ){

						string s_op(1, operators.top());
						//std::cout<<s_op<<"aca"<<endl;
						output_s.push(s_op);	
						//output.push(operators.top());
						operators.pop();
			
			}
			//cout<<*it<<endl;
            operators.push(*it);
        }else if(*it == '('){ //left parentesis
            operators.push(*it);
        }
        else if(*it == ')'){ //right parentesis
            while(operators.top() != '('){
				if (operators.empty()){ //una validacion
					std::cout<<"Error, stack runs out without finding a left parenthesis, then there are mismatched parentheses."<<endl;
					break;
				}else{
					string s_op(1, operators.top());
					//cout<<s_op<<endl;
					output_s.push(s_op);
					//output.push(operators.top());
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
		string s_op(1, operators.top());
		//cout<<s_op<<endl;
		output_s.push(s_op);
		//output.push(operators.top());
		operators.pop();
	}

	//cout<<RPNtoDouble(&output);
	std::cout<<RPNtoDouble(&output_s);
    //imprimiendo el queue output:
	/*
	std::cout<<"Impresion: "<<endl;

    while (!output_s.empty()) {
        std::cout << output_s.front()<<endl;
        output_s.pop();
        }
    */


    return 0;
}