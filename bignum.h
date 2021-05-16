#include <iostream>
#include <stdio.h>
#include "cmdline.h"


class bignum
{
private:
	unsigned short *digits;
	size_t precision;


public:
	bignum ();
	bignum (bignum&);
	bignum (std::string&); //chequear
	bignum (size_t);  //para chequear nosotros en el main
// falta destructor //////////////
	
	/*
	 *
	 * cambiar en el cmdline las letras y funciones de opcion_t
	 *
	 *
	 * agregar en el mismo la opcion -p para la precision
	 *
	 *
	 */


	bignum operator+(const bignum&);
	bignum operator-(const bignum&);
	bignum operator*(const bignum&);
	bignum& operator=(const bignum&);


	bignum convertir_bignum(string::string&);
	void emitir_bignum(); //usar para chequear, despues borrarlo
	friend std::ostream& operator<<(std::ostream&, const bignum&);
	friend std::istream& operator>>(std::istream&, bignum&);


};

