#include "product.h"
/*Implementacion de las sub-clases derivadas de la clase product*/

bignum karatof_mul::multi(const bignum& a, const bignum& b){ 
    bignum aa(a);
    bignum bb(b);
    bignum result;
    sign_t sign;
    b.sign == a.sign ? sign = POS : sign = NEG;
    
    if(aa.dim == 1)
    {
        result = bb * aa.digits[0];   //Para mejorar eficiencia: bignum * digito
        result.sign = sign;
        return result;
    }
    if(bb.dim == 1)
    {
        result = aa * bb.digits[0];
        result.sign = sign;
        return result;
    }
    aa.sign = POS;                 //Operacion con ambos numeros positivos  
    bb.sign = POS;
    result = _karatof(aa, bb);
    result.sign = sign;            //cout << "Multiplicacion Karatsuba" << endl;
    return result;
}
bignum classic_mul::multi(const bignum& a, const bignum& b){ 

    int large = a.dim + b.dim;
    bignum r(large);
    for (int k = 0; k < b.dim; k++)
    {
        bignum multi(a.dim + 2 + k);
        multi = a * b.digits[k];
        r = r + multi.add_zeros(k, true);
    }
    b.sign == a.sign ? r.sign = POS : r.sign = NEG; //cout << "Multiplicacion Classic" << endl;
    return r;
}
bignum _karatof(bignum& a, bignum& b) 
{
    if(a.is_zero() || b.is_zero())     //Para mayor eficiencia, bignum de dim > 1 con todos zeros
    {    
        bignum zero(1);                  //Retorna un bignum 0 de dim 1
        return zero;
    }
    if(a.dim == 1 && b.dim > 1)             //Caso base
        return b * a.digits[0];
    if(b.dim == 1 && a.dim > 1)             //Caso base
        return a * b.digits[0];

    int n = a.dim;
    if(a.dim > b.dim)          
    {
        n = a.dim;
        if(n & 1)               // si es impar hace n+1 y cambia las dos dim
        {
            n++;
            a.add_zeros(n - a.dim, false);
            b.add_zeros(n - b.dim, false);
        }
        else
            b.add_zeros(n - b.dim, false);
    }
    if(b.dim > a.dim)
    {
        n = b.dim;
        if(n & 1)
        {
            n++;
            a.add_zeros(n - a.dim, false);
            b.add_zeros(n - b.dim, false);
        }
        else
            a.add_zeros(n - a.dim, false);
    } 
    if((n & 1) && (n > 1))      // caso en que dim a = dim b y son impares
    { 
        n++;             
        a.add_zeros(n - a.dim, false);
        b.add_zeros(n - b.dim, false);     
    }
    int k = n / 2;
    if(n == 1)                           //CONDICION BASE: dim == 1, multiplicacion entre 2 digitos
    {
        unsigned short mul = a.digits[0] * b.digits[0];
        if(mul > 9)
        {
            bignum m(2);
            m.digits[0] = mul % 10;
            m.digits[1] = mul / 10;
            return m;
        }
        bignum m(1);
        m.digits[0] = mul;
        return m;
    }
    bignum a0(a.digits + k, n - k, POS);     // a = |---a0---||---a1---|
    bignum a1(a.digits, n - k, POS);     
    bignum b0(b.digits + k, n - k, POS);     // b = |---b0---||---b1---|
    bignum b1(b.digits, n - k, POS);
    bignum c1 = a0 + a1;
    bignum c2 = b0 + b1;
    bignum c = _karatof(c1, c2);
    bignum d = _karatof(a0, b0);
    bignum e = _karatof(a1, b1);
    bignum dd(d);
    dd.add_zeros(n, true);       // dd = d * 10^2k = d * 10^n 
    bignum subs = c - d - e;        
    subs.add_zeros(k, true);     // subs = (c - d - e) * 10^k 
    return dd + subs + e; 
}