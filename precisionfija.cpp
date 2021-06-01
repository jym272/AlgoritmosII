#include "precisionfija.h"

using namespace std;
precision_fija::precision_fija()
{
}
precision_fija::precision_fija(istream &iss, ostream &oss){
    iss_ = &iss;
    oss_ = &oss;
}
precision_fija::~precision_fija(){
    
}
void precision_fija::captura(precision_t *precision){
    
    string s; //se almacenará la lína 
    smatch m; // ver regex c++, se almacenará las "captura" realizadas por el regex
    static bool entry_error=false;

    //validacion regex---> https://regex101.com/  consultar: ^(\d+|\-\d+)\s*(\+|\*|\-)\s*(\d+|\-\d+)$
    //\s --> matches any whitespace character (equivalent to [\r\n\t\f\v ])
    regex e ("^(\\d+|\\-\\d+)\\s*(\\+|\\*|\\-)\\s*(\\d+|\\-\\d+)$");

    // Recibo el flujo isstream y lo guarda en un string
    while (getline(*iss_, s)){
        if (s.empty() == true){ // si la linea está vacía,
                *oss_<<"Finished program"<<endl;       
                break;
        }
        if (std::regex_search (s,m,e)) { //true: A match was found within the string.
            //for (auto x:m) std::cout << x << ".."; realiza lo mismo que el for de abajo
            //for (unsigned i=0; i<m.size(); ++i) {
            //    cout << "[" << m[i] << "] ";
            //}
            /////////////////////////////////////////////////////////////////////////////////    
            // m.str(i)
            // En i=0 se encuentra toda el string, i=1 1er bignum, i=2 la operacion, i=3 2do bignum
            string a=m.str(1); 
            string b=m.str(3);

            int precision_a = precision->isSet ? precision->value : a.length();
            int precision_b = precision->isSet ? precision->value : b.length();
            //cout<<precision_a<<" "<<precision_b<<endl;
            bignum aa(a, precision_a);
            bignum bb(b, precision_b); 
            switch (m.str(2)[0])
            {
            case '+':
                *oss_ << aa + bb; 
                break;
            case '-':
                *oss_ << aa - bb;
                break;
            case '*':
                *oss_ << aa * bb;
                break;
            default:
                // validado desde el regex, no es posible que llegue acá;
                break;
            }
            s = m.suffix().str(); // captura el "\n" (o cualquier otro caracter que no haya sido procesado por regex) de la 
                                  // línea dejando al objeto match "ready" para la siguiente captura
         }
         else{ 
            *oss_<<"Entry not processed"<<endl;
            entry_error=true;
         }
    } 
    if (iss_->bad()) {
        cerr << "cannot read from input stream."
        << endl;
        exit(1);
    }
    if(entry_error) {
        exit(1); //el programa terminará con valor no nulo ya que hubo al menos un error en el procesamiento de operaciones.
    }
}



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

bignum RPNtobignum(queue<string> *output){
		
        stack<char> resultado;
		stack<bignum> resultado_b;

        while(!output->empty()) 
        {
            // Desencolo un token: strings de dígitos u operaciones.
            if (std::isdigit(output->front().at(0))) 
            {
				bignum bb(output->front(),output->front().length()); //---> full precision
				resultado_b.push(bb);    
				output->pop();            
            }
            else
            {
                // Token is an operator: pop top two entries
				bignum b1,b2;
				if (!resultado_b.empty()){
					b1= resultado_b.top();
					resultado_b.pop();
				}else{
					cout<<"Este vacia, hay un error desde la operacion"<<endl;
					exit(1);
				}

				if(!resultado_b.empty()){
					b2 = resultado_b.top();
					resultado_b.pop();
				}else{
					cout<<"este vacia, hay un error desde la operacion"<<endl;
					exit(1);
				}

				//Get the result
	            bignum result = output->front().at(0) == '+' ? b2 + b1 : 
                            	output->front().at(0) == '-' ? b2 - b1 :
														    b2 * b1 ; 
								/* 
                             	output->front().at(0) == '*' ? b2 * b1 :
                                                      b2 / b1 ;  */             
                                 
                // Push result onto stack
				resultado_b.push(result);
				output->pop();                                                        
            }                        
        }        
        return resultado_b.top();
}



 void precision_fija::shunting(){
    
	string token; //se alamcenará la línea
	queue<string> RPN_s; // cola RPN  
    stack<char> operators; // pila de operadores
    
    bool entry_error=false;
    static bool exit_status=0;

    //incializando el set de operadores para comparación
    std::set<char> operators_chars;
    char const * CharList = "*/+-";
    operators_chars.insert(CharList, CharList + strlen(CharList));

    while(getline(*iss_, token)){
        if (token.empty() == true){ // si la linea está vacía,
                *oss_<<"Finished program"<<endl;       
                break;
        }
        string::const_iterator it = token.begin();
        while(it != token.end()){ 
	    	string bignum_s; // string que será el bignum
            entry_error = false;
            
            if (std::isdigit(*it)){ //si es digito 
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
	    				&&  hasgreaterPrecedence(*it, &operators.top())
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
                while(operators.top() != '('){
	    			if (operators.empty()){ //una validacion
	    				std::cout<<"Error, stack runs out without finding a left parenthesis, then there are mismatched parentheses."<<endl;
	    				break;
	    			}else{
	    				string s_op(1, operators.top());
	    				RPN_s.push(s_op);
	    				operators.pop();
	    			}
                }
	    		if(operators.top() == '('){
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
	    		*oss_<<"Entry not processed"<<endl;
                entry_error=true;
                break;
	    	}
	    	++it;
        }
        //Desapilo lo restante:
	    /* After while loop, if operator stack not null, 
	    pop everything to output queue */

        if(!entry_error){ // no se realiza lo que viene si tengo error en la entrada
            while(!operators.empty()){
	        	// si encontre un parentesis en el top de la pila entonces hay mismatched parentesis
	        	if(operators.top() == '(' || operators.top() == ')'){
	        		cout<<"mismatched parentesis"<<endl;
	        		break;
	        	}
	        	string s_op(1, operators.top());
	        	RPN_s.push(s_op);
	        	operators.pop();
	        }

            *oss_<<RPNtobignum(&RPN_s);
        }else{
            exit_status = 1; //hubo al menos un error, la salida sera error
        }
        //Imprimiendo el queue output:
	    /*
	    std::cout<<"Impresion: "<<endl;
        while (!RPN_s.empty()) {
            std::cout << RPN_s.front()<<endl;
            RPN_s.pop();
            }
        */
    }
    if(exit_status) {
        exit(1); //el programa terminará con valor no nulo ya que hubo al menos un error en el procesamiento de operaciones.
    }
 }