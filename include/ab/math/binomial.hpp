#pragma once



namespace ab{


template<typename T, typename IDX=int>
struct binomial
{
	binomial() : n(0), k(0), val(1) {}
	binomial(const IDX& nn, const IDX& kk) : n(nn), val(1){
		ASS(0<=kk);
		if(2*kk<nn){
			k = 0;
			while(k<kk)moveR();
		}
		else{
			k=nn;
			while(k>kk)moveL();
		}
	}
	IDX n,k;
	T val;
	
	operator T(){
		return val;
	}

	bool outside(){
		bool ret = n<0 || k<0 || k>n;
		if(ret)val=0;
		return ret;
	}

	void moveL(){
		--k;
		if(outside()) return;

		if(k==n) val=1;
		else
		{
			val*=(k+1);
			val/=(n-k);
		}
	}
	void moveR(){
		++k;
		if(outside()) return;

		if(k==0) val=1;
		else
		{
			val*=(n-k+1);
			val/=(k);
		}
	}
	void moveD(){
		++n;
		if(outside()) return;

		if(n==k) val=1;
		else
		{
			val*=n;
			val/=(n-k);
		}
	}
	void moveU(){
		--n;
		if(outside()) return;

		val*=(n-k+1);
		val/=(n+1);
	}
};



}


