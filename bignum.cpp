#include "bignum.h"

bignum::bignum()
{
    digits = NULL;
    dim = 0;
    sign = POS;
    
    KARATSUBA_ALGORITHM ? this->p = new karatof_mul
                 : this->p = new classic_mul;  
}
bignum::bignum(int n)
{
    digits = new unsigned short[n]();
    dim = n;
    sign = POS;

    KARATSUBA_ALGORITHM ? this->p = new karatof_mul
                 : this->p = new classic_mul; 
}
bignum::bignum(const bignum &a)
{
    digits = new unsigned short[a.dim];
    for(int i = 0; i < a.dim; i++)
        digits[i] = a.digits[i];
    dim = a.dim;
    sign = a.sign;

    KARATSUBA_ALGORITHM ? this->p = new karatof_mul
                 : this->p = new classic_mul; 
}
bignum::bignum(std::string& str, int precision)
{   
    dim = str.length();
    sign = POS;
    int j = 0; //Contador del string
    if(str.at(0) == '-') 
    {   
        sign = NEG;
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

    KARATSUBA_ALGORITHM ? this->p = new karatof_mul
                 : this->p = new classic_mul; 
}
bignum::~bignum()
{
    if(digits)
        delete []digits; 
   if (p)
       delete p;
}
void copy_array(unsigned short *dest, unsigned short *orig, int n)
{
    for(int i = 0; i < n; i++)
        dest[i] = orig[i];
}
bignum::bignum(const unsigned short *v, int n, sign_t sign_) 
{
    digits = new unsigned short[n];
    for(int i = 0; i < n; i++)
        digits[i] = v[i];
    dim = n;
    sign = sign_;
}
int resize(unsigned short *&a, int n) // Quita los zeros sobrantes y devuelve la nueva dim
{    
    int zeros = 0;
    while((a[n - zeros - 1] == 0) && (n - zeros - 1) > 0) zeros++;     
    unsigned short *aux = new unsigned short[n - zeros]; 
    copy_array(aux, a, n - zeros);
    delete[] a;
    a = aux; 
    return n - zeros;
}
bool bignum::is_zero() const
{
    size_t add = 0;
    for(int i = 0; i < dim; i++)
        add += digits[i];
    return add == 0;       //Suma de todos los digitos == zero == true
}
bignum bignum::add_zeros(int zeros, bool start) //inicio == false, agrega zeros al final
{
    if(zeros == 0) return *this;
    unsigned short *aux = new unsigned short[dim + zeros]();  
    start ? copy_array(aux + zeros, digits, dim) : copy_array(aux, digits, dim);
    delete []digits;
    digits = aux;
    dim += zeros;
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
bignum operator*(const bignum& a, const bignum& b) {
    bignum result;
    result = a.p->multi(a,b);
    return result; 
}

/*
bignum operator*(const bignum& a, const bignum& b) 
{
    bignum result;
    product *p;
    //std::cout<<"opertor**"<<std::endl;

    if(KARATSUBA_ALGORITHM == true)
    {
	p = new classic_mul;
        result = p->multi(a, b);
        delete p;
	return result; 
    }
    else
    {
	p = new karatof_mul;
	result = p->multi(a, b);
        delete p;
	return result;
    }	
}
*/
bignum operator*(const bignum& a, const unsigned short b) 
{
    bignum result(a.dim + 1);
    int i = 0;
    unsigned short carry = 0;
    for (; i < a.dim; )
    {
        unsigned short multi = 0;
        multi = a.digits[i] * b;
        if(multi + carry > 9)     
            result.digits[i] = (multi + carry) % 10;
        else
            result.digits[i] = multi + carry;
        result.digits[i + 1] = (multi + carry) / 10;
        carry = 0;
        carry = result.digits[i + 1];  
        i++;
    }
    result.dim = resize(result.digits, result.dim); //ojo
    return result;
}
bignum operator+(const bignum& a, const bignum& b)
{
    int new_dim;
    a.dim > b.dim ? new_dim = a.dim + 1 : new_dim = b.dim + 1;
    bignum c(new_dim);
    if(a.sign && !b.sign) // a < 0 y b > 0 --> c = b - a
    {   
        bignum aa(a);
        aa.sign = POS;
        c = b - aa;
        return c;
    }
    if(!a.sign && b.sign) // a > 0 y b < 0 --> c = a - b
    {   
        bignum bb(b);
        bb.sign = POS;
        c = a - bb;
        return c;
    }
    c.sign = a.sign;
    int na = 0; int nb = 0;
    for(int i = 0; i < new_dim; i++)
    {
        unsigned short carry = 0;
        unsigned short add = 0;
        add = ((na < a.dim) ? a.digits[na++] : 0)  
                            + ((nb < b.dim) ? b.digits[nb++] : 0) 
                            + c.digits[i];
        carry = add / 10; 
        c.digits[i] = add % 10;
        if(i < new_dim - 1)
            c.digits[i + 1] = carry; 
    }              
    c.dim = resize(c.digits, new_dim);
    return c;
}
bool bigger(unsigned short *v1, size_t n1, unsigned short *v2, size_t n2)
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
bool equal_module(unsigned short *v1, int n1, unsigned short *v2, int n2)
{
    if(n1 != n2) return false;

    for(int i = 0; i < n1; i++)
        if(v1[i] != v2[i]) return false;
    return true;
}
unsigned short *substract(unsigned short *a, int na, unsigned short *b, int nb, int &nc)
{
    unsigned short *c = new unsigned short[na]();
    nc = na;

    for(int i = 0; i < nb; i++)
    {
        if(a[i] < b[i])             //Pide carry   
        {
            a[i] += 10; 
            c[i] = a[i] - b[i];
            if(a[i + 1] != 0)       //Si el que sigue no es zero le resta 1
                a[i + 1]--;
            else                    //Si es zero, sigue hasta encontrar un num > 0
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
    if(equal_module(b.digits, b.dim, a.digits, a.dim))
    {    
        if(!(b.sign ^ a.sign)){ 
            c.digits = new unsigned short[1]();
            c.digits[0]=0;
            c.dim=1;
            return c; // XNOR: a = b = true o a = b = false --> c = 0
        }
        c = a + a;
        if(b.sign)
            return c;     // b < 0 y a = b --> c = a + a
        c.sign = NEG;           // a < 0 y a = b --> c = -(a + a)
        return c; 
    }
    if(a.sign && !b.sign)        // a < 0 y b > 0 --> c = -(a + b) 
    {
        bignum aa(a);
        aa.sign = POS;
        c = aa + b;
        c.sign = NEG;
        return c;
    }
    if(!a.sign && b.sign)        // a > 0 y b < 0 --> c = a + b 
    {
        bignum bb(b);
        bb.sign = POS;
        c = a + bb;
        return c;      
    }
    if(bigger(b.digits, b.dim, a.digits, a.dim)) //  b > a --> c = -(b - a) 
    {
        aux = substract(b.digits, b.dim, a.digits, a.dim, dim_c);
        c.digits = new unsigned short[dim_c];
        c.dim = dim_c;
        if(!a.sign)                           // b > a, a > 0 y b > 0 --> c < 0          
            c.sign = NEG;
        copy_array(c.digits, aux, dim_c);
        delete []aux;
        return c;
    }                                                       // a > b --> c = a - b                        
    aux = substract(a.digits, a.dim, b.digits, b.dim, dim_c);
    c.digits = new unsigned short[dim_c];
    c.dim = dim_c;
    if(a.sign)                  // a > b, a < 0 y b < 0 --> c < 0
        c.sign = NEG;
    copy_array(c.digits, aux, dim_c);
    delete []aux;
    return c;
}
std::ostream& operator<<(std::ostream& oss_, const bignum& out){
	if(out.dim == 0 || ( out.dim == 1 && out.digits[0]==0)){
        //std::cout<<"La dimension es zero!"<<std::endl;
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
bool operator==(const bignum& rhs, const bignum& lhs)
{
	if (rhs.dim != lhs.dim)
		return false;

	for (int i = 0; i < rhs.dim; i++)
	{
		if (rhs.digits[i] != lhs.digits[i])
			return false;
	}


	return true;
}
bool operator<(const bignum& lhs, const bignum& rhs)
{
	if (lhs.dim > rhs.dim)
		return false;
	if(lhs.dim < rhs.dim)
		return true;
	if (lhs == rhs)
		return false;

	for (int i = lhs.dim - 1; i >= 0; i--)
	{
		if  (lhs.digits[i] > rhs.digits[i])
			return false;
		else if(lhs.digits[i] < rhs.digits[i])
			return true;
	}

	return true;
}
bool operator>=(const bignum& lhs, const bignum& rhs)
{
	if (lhs.dim < rhs.dim)
		return false;
	if (lhs.dim > rhs.dim)
		return true;

	int i = lhs.dim - 1;

	while(lhs.digits[i] == rhs.digits[i] && i > 0)
		i--;

	return lhs.digits[i] >= rhs.digits[i];
}
bool operator<=(const bignum& lhs, const bignum& rhs)
{
	if (lhs.dim > rhs.dim)
		return false;
	if(lhs.dim < rhs.dim)
		return true;

	int i = lhs.dim - 1;

	while(lhs.digits[i] == rhs.digits[i] && i > 0)
		i--;

	return lhs.digits[i] <= rhs.digits[i];
}
unsigned short 
bignum::calc_coc(const bignum & lhs)
{
	//n es la dimension de lhs
	bignum num(this->dim + 2);
	num = lhs;
	unsigned short count = 0;
	while(num <= *this)
	{
		count++;
		num = num + lhs;
	}
	return count;
}
bignum bignum::shift()
{
	unsigned short *aux = new unsigned short [dim + 1]();
	copy_array(aux + 1, digits, dim);
	delete []digits;
	digits = aux;
	dim++;
	return *this;
}
bignum operator/(const bignum& div, const bignum& dsor)
{
	bignum divisor = dsor;
	divisor.dim = resize(divisor.digits, divisor.dim);
	if (div < divisor)
	{
		bignum zero(1);
		zero.digits[0] = 0;
		return zero;
	}

	for (int i = divisor.dim - 1; divisor.digits[i] == 0; i--)
		if (i == 0)
		{
            std::cerr<<"ZeroDivisionError: division by zero\n";
            exit(1);
			//bignum error(1);
			//error.digits[0] = 1;
			//error.sign = NEG; //se encontraba true, se coloca signo negativo  
			//return error;
		}

    if (div == divisor)
	{
		bignum one(1);
		one.digits[0] = 1;
        div.sign == divisor.sign ? one.sign = POS : one.sign = NEG;
		return one;
	}

	bignum total_quotient(1);
	bignum dividend(1);
                  
	for(int i = div.dim - 1; i >= 0; i--)
	{
		if(dividend >= divisor)
		{
			unsigned short cociente = dividend.calc_coc(divisor);
			total_quotient.digits[0] = cociente;
			bignum resta(dividend.dim);
			resta = divisor * cociente;
			resta.dim = resize(resta.digits, resta.dim);
			dividend = dividend - resta;
		}
		else
    		total_quotient.digits[0] = 0;
		total_quotient.shift();
		dividend.shift();
		dividend.digits[0] = div.digits[i];
		dividend.dim = resize(dividend.digits, dividend.dim); //movi el resize acá

		if (i == 0)
		{
			if(dividend >= divisor)
			{
                 
				unsigned short cociente = dividend.calc_coc(divisor);
				total_quotient.digits[0] = cociente;
			}
			else
                break;
		}
	}

	total_quotient.dim = resize(total_quotient.digits, total_quotient.dim);
                                                    //false, se asume POS          //true, se asume NEG
	div.sign == divisor.sign ? total_quotient.sign = POS : total_quotient.sign = NEG;

	return total_quotient;
}
/**
    * Sobrecarga del operador de ingreso.
    * 
    * Se lee e interpreta un número a partir de un stream de entrada 
    * sin tener ningún tipo de información de contexto.
    * Cualquier argumento incorrecto guarda un bignum 0.
    * Ej: -12 --> -12   /  -123aqw  ---> -123  /  -q1233 ---> 0 
    * @param [out] iss_ Se trabaja sobre el istream, el cual llega por referencia
    * @param [out] in Se carga el bignum cargado por usuario, con la precisión especificada por en el inicio del programa
    * @return El istream iss_
*/
std::istream& operator>>(std::istream& iss_, bignum& in){

    string s, bignum_s;
    iss_>> s;
    string::const_iterator it = s.begin();

    if (*it == '-'){ // si se tiene un bignum negativo, el primer caracter es un '-'
        bignum_s+='-';    
        ++it;
    }
    while(std::isdigit(*it)){ //acumulo todos lo digitos 
	    bignum_s+=*it;
	    ++it;
	}
    if(!bignum_s.length() || ((bignum_s.length() == 1) && (bignum_s[0] == '-'))){ //no almacene nada o solo almacene el '-'/->asigno un cero
        string s_parse="0";
        bignum parse(s_parse, s_parse.length());
        in = parse;
    }else{
        bignum parse(bignum_s, bignum_s.length());
        in = parse;
    }
    return iss_;   
}