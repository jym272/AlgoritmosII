#ifndef _BIGNUM_H_INCLUDED_
#define _BIGNUM_H_INCLUDED_

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <regex>

struct precision_t {
  int value;
  bool isSet = false; // esta seteado por linea de argumento?
} ;
// La precision si no es especificada el ejecturar el programa, se ajustará de acuerdo a la dimensión
// del string que se asigna a bignum
static precision_t precision;

using namespace std;
class bignum
{
private:
	unsigned short *digits;
	int dim;
	bool sign;

public:
	bignum ();
	bignum (const bignum&);
	bignum (std::string&, int); //chequear
	bignum (int);  //para chequear nosotros en el main

	~bignum();

	bignum agregar_ceros(int pos, int n);
	bignum& operator=(const bignum&);

	friend bignum operator*(const bignum& a, const unsigned short b); 
	friend bignum operator*(const bignum& a, const bignum& b); 
	friend bignum operator+(const bignum& a, const bignum& b);
	friend bignum operator-(const bignum& a, const bignum& b);

	bignum convertir_bignum(std::string&);
	void emitir_bignum(); //usar para chequear, despues borrarlo
	friend std::ostream& operator<<(std::ostream&, const bignum&);
	friend std::istream& operator>>(std::istream&, bignum&);
};
#endif