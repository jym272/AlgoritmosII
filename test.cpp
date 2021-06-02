#include <stack>
#include <iostream>
#include <set>
#include <cstring>
#include <queue>
#include "bignum.h"
using namespace std;
#include <stack>

//Base class
class cola_bignum
{
	public:
	cola_bignum(){

	}
	
	
	virtual void Print(ostream& os)=0;

	~cola_bignum(){

	}
	virtual bool isBignum()=0;
};

std::ostream& operator<<(ostream& os, cola_bignum& loc) 
{ 
    loc.Print(os); 
    return os; 
}
// Sub class inheriting from Base Class(Parent)
class bignum_child : public cola_bignum
{
	int bignum;
    public:
      bignum_child(int a){
        bignum=a;
      }
	void Print(ostream& os){
		os << this->bignum;
	}
	bool isBginum(){
		return true;
	}
};

class bignum_operator : public cola_bignum
{
	char op;
  public:
  bignum_operator(char c){
    op = c;
  }
  void Print(ostream& os){
		os << this->op;
  }
  bool isBignum(){
	  return false;
  }
};


//main function
int main()
   {
	
    queue<cola_bignum*> cola;
	stack<cola_bignum*> pila;
	queue<bignum_op*> colaB;
	
	string s="1110";
	bignum B1(s,1000);
	bignum_operacion B1op('+');

	colaB.push(&B1);
	colaB.push(&B1op);

	bignum_child b1(4545456);
	bignum_operator b2('+');

	if(B1){
		cout<< "soy un bignum"<<endl;
	}

	if(cola.front()->isBignum()){
		cout<<"soy un Bignum"<<endl;
	}

		cola.push(&b1);
		cola.push(&b2);

		pila.push(&b2);
		pila.push(&b1);

		while(!pila.empty()){
			cout<<*pila.top()<<endl;
			pila.pop();
		}
		while(!cola.empty()){
   			 cout<<*cola.front()<<endl;
   			 cola.pop();
  		}

        
        return 0;
   }