#ifndef _BIGNUM_H_INCLUDED_
#define _BIGNUM_H_INCLUDED_

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>

#include "product.h"


using namespace std;
extern bool FLAG_CLASSIC; // Hay que definirla false por defecto 
typedef enum {POS, NEG} sign_t;

class karatof_mul;
class classic_mul;
class product;

class bignum
{
private:
	unsigned short *digits;
	int dim;
	sign_t sign;

	class product *p;

	friend class karatof_mul;
	friend class classic_mul;

	friend void copy_array(unsigned short *dest, unsigned short *orig, int n);
	friend bignum karatof(const bignum& a, const bignum& b);
	friend bignum _karatof(bignum& a, bignum& b);
	friend bignum classic(const bignum& a, const bignum& b);

	int resize(unsigned short *&a, int n);
	bool bigger(unsigned short *v1, size_t n1, unsigned short *v2, size_t n2);
	bool equal_module(unsigned short *v1, int n1, unsigned short *v2, int n2);
	unsigned short *substract(unsigned short *a, int na, unsigned short *b, int nb, int &nc);
	bignum add_zeros(int zeros, bool start); 
   	bool is_zero() const;

	unsigned short calc_coc(const bignum&);
	bignum shift();

public:

	bignum();
	bignum(int);
	bignum(const bignum&);
	bignum(std::string&, int); 
	bignum(const unsigned short*, int n, sign_t sign_); 
	~bignum();

	
	bignum& operator=(const bignum&);

	friend bool operator>=(const bignum&, const bignum&);
	friend bool operator==(const bignum&, const bignum&);
	friend bool operator<(const bignum&, const bignum&);
	friend bool operator<=(const bignum&, const bignum&);
	friend bignum operator/(const bignum &, const bignum& );

	friend bignum operator*(const bignum& a, const bignum& b); 
	friend bignum operator*(const bignum& a, const unsigned short b); 
	friend bignum operator+(const bignum& a, const bignum& b);
	friend bignum operator-(const bignum& a, const bignum& b);
	friend std::ostream& operator<<(std::ostream&, const bignum&);
	friend std::istream& operator>>(std::istream&, bignum&);
};
#endif
