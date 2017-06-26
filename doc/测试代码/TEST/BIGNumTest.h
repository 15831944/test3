#ifndef __BIGNUM_H__
#define __BIGNUM_H__

class CBigNum
{
public:
	CBigNum(){ len = 1;memset(a,0,sizeof(a)); } 
	CBigNum(const int);
	CBigNum(const char*);
	CBigNum(const CBigNum &);
	CBigNum &operator=(const CBigNum &);
	
	friend 	istream& operator>>(istream&,  CBigNum&);
	friend 	ostream& operator<<(ostream&,  CBigNum&);
	
	CBigNum 	operator+(const CBigNum &)	const; 
	CBigNum 	operator-(const CBigNum &)	const;
	CBigNum 	operator*(const CBigNum &)	const;
	CBigNum 	operator/(const int   &)	const;
	
	CBigNum 	operator^(const int  &)		const;
	int    	operator%(const int  &)		const;
	bool   	operator>(const CBigNum & T)	const;
	bool    operator<(const CBigNum & T)	const;
	bool   	operator>(const char*  &t)	const;
	bool	operator<(const char*  &t)	const;
	
private:
	int 	a[500];	
	int 	len; 
};

#endif