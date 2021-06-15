#include "bignum.h"

bignum::bignum()
{
    digits = nullptr;
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


unsigned short 
bignum::calc_coc(const bignum & lhs)
{
	//n es la dimension de lhs
	bignum num(this->dim + 2);
	num = lhs;
	unsigned short cuenta = 0;
	while(num <= *this)
	{
		cuenta++;
		num = num + lhs;
	}
	return cuenta;
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
		bignum cero(1);
		cero.digits[0] = 0;
		return cero;
	}

	if (div == divisor)
	{
		bignum uno(1);
		uno.digits[0] = 1;
		return uno;
	}

	for (int i = divisor.dim - 1; divisor.digits[i] == 0; i--)
		if (i == 0)
		{
			std::cerr<<"división por cero"<<std::endl;
			bignum error(1);
			error.digits[0] = 1;
			error.sign = true;
			return error;
		}

	bignum cociente_total(1);
	bignum dividendo(1);

	dividendo.digits[dividendo.dim - 1] = div.digits[div.dim - 1];


	for(int i = div.dim - 2; i >= 0; i--)
	{
		if(dividendo >= divisor)
		{
			unsigned short cociente = dividendo.calc_coc(divisor);
			cociente_total.digits[0] = cociente;
			bignum resta(dividendo.dim);
			resta = divisor * cociente;
			resta.dim = resize(resta.digits, resta.dim);
			dividendo = dividendo - resta;
		}
		else
			cociente_total.digits[0] = 0;
		std::cout<<"cociente total: "<<cociente_total;
		cociente_total.shift();
		dividendo.shift();
		dividendo.digits[0] = div.digits[i];
		dividendo.dim = resize(dividendo.digits, dividendo.dim); //movi el resize acá
		std::cout<<"dividendo: "<<dividendo;

		if (i == 0)
		{
			if(dividendo >= divisor)
			{
				unsigned short cociente = dividendo.calc_coc(divisor);
				cociente_total.digits[0] = cociente;
			}
			else break;
		}
	}

	cociente_total.dim = resize(cociente_total.digits, cociente_total.dim);

	div.sign == divisor.sign ? cociente_total.sign = false : cociente_total.sign = true;

	return cociente_total;
}


