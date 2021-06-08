#ifndef _BIGNUM_H_INCLUDED_
#define _BIGNUM_H_INCLUDED_

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <regex>
#define MAX_PRECISION 10000

using namespace std;

struct precision_t {
  int value;
  bool isSet; // esta seteado por linea de argumento?
  precision_t(bool set){
	  this->isSet = set;
  }
};
extern precision_t preciseness;

enum sg {POS, NEG};

class bignum
{
private:
	unsigned short *digits;
	int dim;
	enum sg sign;

	friend void copy_array(unsigned short *dest, unsigned short *orig, int n);	
	int resize(unsigned short *&a, int n);
	bool mayor(unsigned short *v1, size_t n1, unsigned short *v2, size_t n2);
	bool modulo_igual(unsigned short *v1, int n1, unsigned short *v2, int n2);
	unsigned short *resta(unsigned short *a, int na, unsigned short *b, int nb, int &nc);
	bignum add_zeros(int pos, int n);

public:
	bignum ();
	bignum (const bignum&);
	bignum (std::string&, int); 
	bignum (int); 
	~bignum ();

	bignum& operator=(const bignum&);
	friend bignum operator*(const bignum& a, const unsigned short b); 
	friend bignum operator*(const bignum& a, const bignum& b); 
	friend bignum operator+(const bignum& a, const bignum& b);
	friend bignum operator-(const bignum& a, const bignum& b);
	friend std::ostream& operator<<(std::ostream&, const bignum&);
	friend std::istream& operator>>(std::istream&, bignum&);
};
#endif