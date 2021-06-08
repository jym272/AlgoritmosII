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
void fixed_precision::capture(){
    
    string s; //se almacenará la lína 
    smatch m; // ver regex c++, se almacenará las "captura" realizadas por el regex
    static bool entry_error=false;

    //validacion regex---> https://regex101.com/  consultar: ^(\d+|\-\d+)\s*(\+|\*|\-)\s*(\d+|\-\d+)$
    //\s --> matches any whitespace character (equivalent to [\r\n\t\f\v ])
    regex e ("^\\s*(\\d+|\\-\\d+)\\s*(\\+|\\*|\\-)\\s*(\\d+|\\-\\d+)\\s*$");

    // Recibo el flujo isstream y lo guarda en un string
    while (getline(*iss_, s)){
        if (s.empty() == true){ // si la linea está vacía,
                *oss_<<"Finished program"<<endl;       
                break;
        }
        if (std::regex_search (s,m,e)) { //true: A match was found within the string.
            //for (auto x:m) std::cout << x << ".."; realiza lo mismo que el for de abajo
            //for (unsigned i=0; i<m.size(); ++i) {
            //    cout << "[" << m[i] << "] ";}
            /////////////////////////////////////////////////////////////////////////////////    
            // m.str(i)
            // En i=0 se encuentra toda el string, i=1 1er bignum, i=2 la operacion, i=3 2do bignum
            string a=m.str(1); 
            string b=m.str(3);
            // Los strings válidos se convierten a flujo de entrada
            // Conversion a flujo isstream 
            stringstream stream_a(a);
            stringstream stream_b(b);
            bignum aa, bb;  //se declara los bignum

            stream_a >> aa;
            stream_b >> bb;
    
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