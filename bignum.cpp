#include "bignum.h"

bignum::bignum()
{
    digits = NULL;
    dim = 0;
    sign = false;
}
bignum::bignum(int n)
{
    digits = new unsigned short[n]();
    dim = n;
    sign = false;
}
bignum::bignum(const bignum &a)
{
    digits = new unsigned short[a.dim];
    for(int i = 0; i < a.dim; i++)
        digits[i] = a.digits[i];
    dim = a.dim;
    sign = a.sign;
}
bignum::bignum(std::string& str, int precision)
{   
    dim = str.length();
    sign = false;
    int j = 0; //Contador del string
    if(str.at(0) == '-') 
    {   
        sign = true;
        precision++; //Contemplo el '-' en la precision
        dim--;
        j++;
    }
    digits = new unsigned short[dim];    
    for(int i = dim - 1; i >= 0; i--) 
    {
        if(j < precision)
        { 
            char c = str.at(j++);      
            digits[i] = c - '0'; 
        }
        else
            digits[i] = 0;
    }
}
bignum::~bignum()
{
    if(digits)
        delete []digits; 
}

void bignum::emitir_bignum()
{
for(int i = 0; i < dim; i++)
    cout << digits[i]; 

cout << ", "  << (sign ? "NEGATIVO" : "POSITIVO") << endl;
}
static void copy_array(unsigned short *dest, unsigned short *orig, int n)
{
    for(int i = 0; i < n; i++)
        dest[i] = orig[i];
}

static int resize(unsigned short *&a, int n) // Quita los ceros sobrantes y devuelve la nueva dim
{    
    int ceros = 0;
    while((a[n - ceros - 1] == 0) && (n - ceros - 1) > 0) ceros++;     
    unsigned short *aux = new unsigned short[n - ceros]; //  (n- ceros) DA NEGATIVO EN ALGUNAS MULTIPLICACIONES
    copy_array(aux, a, n - ceros);
    delete[] a;
    a = aux; 
    return n - ceros;
}

bignum bignum::agregar_ceros(int pos, int n)// Le agrande el vector a (dim+n) xq sino eliminaba los ultimos
{
    unsigned short *aux = new unsigned short[dim + n]();  
    copy_array(aux + n, digits, dim);   // le dejo n ceros al principio a aux
    delete []digits;
    digits = aux;
    dim += n;
    return *this;
}

bignum& bignum::operator=(const bignum& b)
{
    if(&b != this) 
    {
        if(dim != b.dim) 
        {
            unsigned short *aux = new unsigned short[b.dim];
            delete[] digits;
            dim = b.dim;
            sign = b.sign;
            digits = aux;
            copy_array(digits, b.digits, dim);
            return *this;
        }
        else 
        {
            sign = b.sign;
            copy_array(digits, b.digits, dim);
            return *this; 
        }
    }    
    return *this;
}

bignum operator*(const bignum& a, const bignum& b) 
{
    int largo = a.dim + b.dim;

    bignum retorno(largo);
    for (int k = 0; k < b.dim; k++)
    {
        bignum multi(a.dim + 2 + k);
        multi = a * b.digits[k];
        multi.sign = false;    // Pongo los dos positivos, sino hace resta en vez de suma cuando hay uno negativo
        retorno.sign = false;
        retorno = retorno + multi.agregar_ceros(a.dim + 1, k);
    }
    b.sign == a.sign ? retorno.sign = false : retorno.sign = true;
    return retorno;
}

bignum operator*(const bignum& a, const unsigned short b) 
{
    bignum resultado(a.dim + 1);
    int i = 0;
    unsigned short carry = 0;
    for (; i < a.dim; )
    {
        unsigned short multi = 0;
        multi = a.digits[i] * b;
        if(multi + carry > 9)     
            resultado.digits[i] = (multi + carry) % 10;
        else
            resultado.digits[i] = multi + carry;
        resultado.digits[i + 1] = (multi + carry) / 10;
        carry = 0;
        carry = resultado.digits[i + 1];  
        i++;
    }
    return resultado;
}

bignum operator+(const bignum& a, const bignum& b)
{
    int new_dim;
    a.dim > b.dim ? new_dim = a.dim + 1 : new_dim = b.dim + 1;
    bignum c(new_dim);
    if(a.sign && !b.sign) // a < 0 y b > 0 --> c = b - a
    {   
        bignum aa(a);
        aa.sign = false;
        c = b - aa;
        return c;
    }
    if(!a.sign && b.sign) // a > 0 y b < 0 --> c = a - b
    {   
        bignum bb(b);
        bb.sign = false;
        c = a - bb;
        return c;
    }
    unsigned short *aa = new unsigned short[new_dim]();
    unsigned short *bb = new unsigned short[new_dim]();
    copy_array(aa, a.digits, a.dim);
    copy_array(bb, b.digits, b.dim);
    c.sign = a.sign;
    for(int i = 0; i < new_dim; i++)
    {
        unsigned short carry = 0;
        unsigned short suma = 0;
        suma = aa[i] + bb[i] + c.digits[i];
        carry = suma / 10; 
        c.digits[i] = suma % 10;
        if(i < new_dim - 1)
            c.digits[i + 1] = carry;
    }              
    c.dim = resize(c.digits, new_dim);
    delete []aa;
    delete []bb;
    return c;
}

static bool mayor(unsigned short *v1, size_t n1, unsigned short *v2, size_t n2)
{
    if(n1 > n2)
        return true;
    if(n1 < n2)
        return false;    
    else {
        size_t i = n1 - 1;    
        while(v1[i] == v2[i]) i--;
        if(v1[i] > v2[i]) return true;
        return false;    
    }    
}
static bool modulo_igual(unsigned short *v1, int n1, unsigned short *v2, int n2)
{
    if(n1 != n2) return false;

    for(int i = 0; i < n1; i++)
        if(v1[i] != v2[i]) return false;
    return true;
}
static unsigned short *resta(unsigned short *a, int na, unsigned short *b, int nb, int &nc)
{
    unsigned short *c = new unsigned short[na]();
    nc = na;

    for(int i = 0; i < nb; i++)
    {
        if(a[i] < b[i])             //Pide carry   
        {
            a[i] += 10; 
            c[i] = a[i] - b[i];
            if(a[i + 1] != 0)       //Si el que sigue no es cero le resta 1
                a[i + 1]--;
            else                    //Si es cero, sigue hasta encontrar un num > 0
            {
                int j = i + 1;
                while(a[j] == 0)
                {    
                    a[j] = 9;
                    j++;
                }
                a[j]--;
            }    
        }
        else if(a[i] > b[i])
            c[i] = a[i] - b[i];
        else c[i] = 0;        
    }
    for(int i = nb; i < na; i++)    //Completa los restantes
        c[i] = a[i];
    
    if(c[na - 1] == 0)              //Si cambia la dimension de la resta, la ajusta   
        nc = resize(c, na);
    return c; 
}
bignum operator-(const bignum& a, const bignum& b)
{  
    bignum c; // c.digits = {0};
    int dim_c;

    unsigned short *aux;
    if(modulo_igual(b.digits, b.dim, a.digits, a.dim))
    {    
        if(!(b.sign ^ a.sign)) return c; // XNOR: a = b = true o a = b = false --> c = 0
        c = a + a;
        if(b.sign)
            return c;     // b < 0 y a = b --> c = a + a
        c.sign = true;           // a < 0 y a = b --> c = -(a + a)
        return c; 
    }
    if(a.sign && !b.sign)        // a < 0 y b > 0 --> c = -(a + b) 
    {
        bignum aa(a);
        aa.sign = false;
        c = aa + b;
        c.sign = true;
        return c;
    }
    if(!a.sign && b.sign)        // a > 0 y b < 0 --> c = a + b 
    {
        bignum bb(b);
        bb.sign = false;
        c = a + bb;
        return c;      
    }
    if(mayor(b.digits, b.dim, a.digits, a.dim)) //  b > a --> c = -(b - a) 
    {
        aux = resta(b.digits, b.dim, a.digits, a.dim, dim_c);
        c.digits = new unsigned short[dim_c];
        c.dim = dim_c;
        if(!a.sign)                           // b > a, a > 0 y b > 0 --> c < 0          
            c.sign = true;
        copy_array(c.digits, aux, dim_c);
        delete []aux;
        return c;
    }                                                       // a > b --> c = a - b                        
    aux = resta(a.digits, a.dim, b.digits, b.dim, dim_c);
    c.digits = new unsigned short[dim_c];
    c.dim = dim_c;
    if(a.sign)                  // a > b, a < 0 y b < 0 --> c < 0
        c.sign = true;
    copy_array(c.digits, aux, dim_c);
    delete []aux;
    return c;
}
std::ostream& operator<<(std::ostream& oss_, const bignum& out){
	if(out.dim == 0){
        oss_<<'0';
    }
    else{
        oss_<< (out.sign ? "-" : "");
        for(int i=out.dim; i!=0; i--){
		    oss_<< out.digits[i-1];
	    }
    }
    oss_<<"\n";
    return oss_;
}
/**
    * Sobrecarga del operador de ingreso.
    *
    * @param [out] iss_ Se trabaja sobre el istream
    * @param [out] in Se carga al bignum la entrada
    * @return El istream iss_
*/
std::istream& operator>>(std::istream& iss_, bignum& in){

    // realizar un regex para validacion-- si pasa se crea --sino se expone error 
    // me invocan de forma cin >> a, a es un bignum
    // la precision con la cual recibo datos del istream es un párametro que fijo el usario
    // Esta ultima opcion pueda cambiar, si en el programa se necesita que el operator>> siempre trabajae con full precision
    string s;
    iss_>> s;
    bignum parse(s,100);
    in = parse;
    return iss_;

}
