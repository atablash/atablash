#pragma once

#include <atablash/types.hpp>

namespace ab{


// returns number of bytes of utf8 character with first byte 'c'
inline ui8 utf8bytes(const ui8& c){
	if ((c&0x80) == 0x00) return 1;
	if ((c&0xE0) == 0xC0) return 2;
	if ((c&0xF0) == 0xE0) return 3;
	ASS((c&0xF8) == 0xF0);
	return 4;
}


// iterator
class utf8i{
public:
	template<typename T>
	utf8i(const T* _p) : p((const ui8*)_p){}
	
	bool operator<(const utf8i&o)const{
		return p < o.p;
	}

	char32_t operator*(){
		ASS(p);
		ui32 c = *p;
		if((c&0x80) == 0x00) return c;
		
		ui32 c2 = *(p+1);
		if((c&0xE0) == 0xC0) return ((c&0x1F) << 6) + (c2&0x3F);
		
		ui32 c3 = *(p+2);
		if((c&0xF0) == 0xE0) return ((c&0x0F) << 12) + ((c2&0x3F) << 6) + (c3&0x3F);
		
		ASS((c&0xF8) == 0xF0);
		ui32 c4 = *(p+3);
		return ((c&0x07) << 18) + ((c2&0x3F) << 12) + ((c3&0x3F) << 6) + (c4&0x3F);
	}

	utf8i& operator++(){
		ASS(p);
		p += utf8bytes(*p);
		return *this;
	}
	
	// slower!
	// dangerous at the end of iteration
	utf8i& operator--(){
		ASS(p);
		do{
			--p;
		}while(((*p)&0xC0) == 0x80);
		return *this;
	}
	
private:
	const ui8* p = nullptr;
};

}

