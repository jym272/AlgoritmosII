#include "precisionfija.h"

using namespace std;
fixed_precision::fixed_precision()
{
}
fixed_precision::fixed_precision(istream &iss, ostream &oss){
    iss_ = &iss;
    oss_ = &oss;
}
fixed_precision::~fixed_precision(){
    
}
int hasgreater_equalPrecedence(char it, char * top){
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
/**
    * Proceso de la Cola de RPN a bignum 
    *
    * @param [in] RPN Se recibe por referencia la fila Reverse Polish Notation
    * @return El bignum con el resultado final luego de procesar todoa la cola RPN
*/
bool RPNtobignum(queue<string> *RPN, bignum *resultado, std::set<char> *operators_chars){
	stack<bignum> resultado_b; //se van apilando las operaciones con bignum, el ultimo en desapilarse es el resultado
    string error_msg_syntax;
    while(!RPN->empty()) 
    {
        // Desencolo un token: strings de dígitos u operaciones.
        //Si llega un bignum o un bignum negativo 
        if ( (RPN->front().at(0) == '-' && RPN->front().size() > 1) || std::isdigit(RPN->front().at(0)) )
        {
            stringstream stream_a(RPN->front());
			bignum aa;
            stream_a >> aa;
			resultado_b.push(aa);    //lo apilo a la pila del resultado bignum
			RPN->pop();              //lo saco de la fila
        }
        else if(operators_chars->find(RPN->front().at(0)) != operators_chars->end()){
            // En la cola RPN se tiene un operador valido, desapilo dos bignum.
			bignum  b1, b2;
			if (!resultado_b.empty()){ //siempre debe haber dos bignum, en otro caso es un error de ingreso ej: 5++5
				b1= resultado_b.top();
				resultado_b.pop();
			}else{
                return true;
			}
			if(!resultado_b.empty()){
				b2 = resultado_b.top();
				resultado_b.pop();
			}else{
                return true;
			}
			//Get the result
	        bignum result = RPN->front().at(0) == '+' ? b2 + b1 : 
                        	RPN->front().at(0) == '-' ? b2 - b1 : 
                         	RPN->front().at(0) == '*' ? b2 * b1 :
                                                        b2 / b1 ;          
                             
            // Push result onto stack
			resultado_b.push(result);
			RPN->pop();                                                        
        }else{ //no es un operador valido 
            return true;
        }                      
    }  
    if(resultado_b.empty()) //la pila tiene que almacenar el resultado
        return true;
    else
        *resultado=resultado_b.top();
    return false;
}



bool fixed_precision::shunting(){
    
	string token; //se almacenará la línea

    bool entry_error=false;
    static bool exit_status=0;

    //incializando el set(conjunto) de operadores para comparación
    std::set<char> operators_chars;
    char const * CharList = "*/+-";
    operators_chars.insert(CharList, CharList + strlen(CharList));

    while(getline(*iss_, token)){
        if (token.empty() == true){ // si la linea está vacía,
                *oss_<<"Finished program"<<endl;       
                break;
        }
        
        queue<string> RPN_s; // cola RPN  
        stack<char> operators; // pila de operadores
        entry_error = false;
        string::const_iterator it = token.begin();

        while(it != token.end()){ 
	    	string bignum_s; // string que será el bignum
            
            if (*it == '-' && *(it+1) /*&& *(it-1) != ')'*/ ){ //si puede ser un digito negativo y el siguiente existe
                bool flag_ = false;
                
                if (it != token.begin()){ //para acceder al anterior no debo estar en el principio, evita errores de memoria     
                    if (*(it-1) == ')') //Caso particular, si existe lo leo.
                        flag_ = true; // ej (-2-5)-6 --> en este caso el - es una operacion, no el signo del bignum   
                }
                
                if(!flag_ && std::isdigit(*(it+1))){ //se que existe el siguiente, lo leo 
                    ++it; // ya se que lo que sigue es un digito
                    bignum_s+='-';
                    while(std::isdigit(*it)){ //acumulo todos lo digitos 
	    			    bignum_s+=*it;
	    			    ++it;
	    		    }
                    RPN_s.push(bignum_s);
                    if(it == token.end()){ // si el siguiente token es el EOF
	    		    	break; //sale del proceso de la línea (solo afecta al while mas inmediato)
	    	    	}
                } 
            }
            if (std::isdigit(*it)){ //si es digito, un bignun positivo
	    		while(std::isdigit(*it)){ //acumulo todos lo digitos 
	    			bignum_s+=*it;
	    			++it;
	    		}
	    		RPN_s.push(bignum_s);
                if(it == token.end()){ // si el siguiente token es el EOF
	    			break; //sale del proceso de la línea (solo afecta al while mas inmediato)
	    		}
            }
	    	if(operators_chars.find(*it) != operators_chars.end()){ //si es un operador válido

	    		while(	!operators.empty()
	    				&&  hasgreater_equalPrecedence(*it, &operators.top())
	    				&&  ( operators.top() != '(') )
                {
	    			string s_op(1, operators.top());
	    			RPN_s.push(s_op);	
	    			operators.pop();
	    		}
	    		operators.push(*it);
            }else if(*it == '('){ //left parentesis
                operators.push(*it);
            }
            else if(*it == ')'){ //right parentesis
                while( !operators.empty() && operators.top() != '('){
                    string s_op(1, operators.top());
	    			RPN_s.push(s_op);
	    			operators.pop();
	    		}
                if (operators.empty()){
                    *oss_<<"Entry not processed: stack runs out without finding a left parenthesis, then there are mismatched parentheses."<<endl;
	    			entry_error=true;
                    break;
                }else{ //operators.top() == '('
                    operators.pop();
                }
            }
	    	// if is awhite-space characters are any of:
	    	// ' '	(0x20)	space (SPC)
	    	// '\t'	(0x09)	horizontal tab (TAB)
	    	// '\n'	(0x0a)	newline (LF)
	    	// '\v'	(0x0b)	vertical tab (VT)
	    	// '\f'	(0x0c)	feed (FF)
	    	// '\r'	(0x0d)	carriage return (CR)
	    	else if (!isspace(*it)){ //a esta altura solo esta permitido whitespace
	    		*oss_<<"Entry not processed: wrong characters"<<endl;
                entry_error=true;
                break;
	    	}
	    	++it;
        }
        //Desapilo lo restante:
	    /* After while loop, if operator stack not null, 
	    pop everything to RPN queue */
        while(!operators.empty()){
	        // si encontre un parentesis en lo que resta de la pila entonces hay mismatched parentesis
	        if(operators.top() == '(' || operators.top() == ')'){
	        	*oss_<<"Entry not processed: mismatched parenthesis"<<endl;
	        	entry_error = 1;
                break;
	        }else{
	            string s_op(1, operators.top());
	            RPN_s.push(s_op);
	            operators.pop();
            }
	    }
        //Imprimiendo el queue RPN:
	    /*
	    std::cout<<"Impresion: "<<endl;
        while (!RPN_s.empty()) {
            std::cout << " "<< RPN_s.front();
            RPN_s.pop();
            }
        exit(1);
        */
        if(!entry_error){ // no se realiza el calculo de bignum si tengo error en la entrada
            bignum resultado; 
            entry_error = RPNtobignum(&RPN_s, &resultado, &operators_chars); // Se desencola a bignum y puede haber errores de entrada que fueron validas hasta este momento
                                                           // ej: 5++5                 
            if (!entry_error){ // Si se realizo las operaciones de forma correcta se imprime el resultado
                *oss_ << resultado;
            }
            else{
                *oss_ <<"Entry not processed: syntax error in expression"<<endl;                 
                exit_status=1;
            }
        }else{
            exit_status = 1; //hubo al menos un error, la salida sera error
        }
    }
    if(exit_status) {
        return 1; //Hubo errores en el ingreso de operaciones, El programa termina con 1
    }else
        return 0;
 }