
// zlib
#include <zlib.h>

// this
#include <atablash/zlib/zlib.hpp>
#include <atablash/debug.hpp>

// c
#include <cstring>
#include <cstdint>



#define CHUNK 16384
void Inflate(void* dest, unsigned int destSize, void* src, unsigned int srcSize)
{
	z_stream s;
	memset(&s, 0, sizeof(z_stream));

	s.next_out = (uint8_t*)dest;
	s.avail_out = destSize;

	s.next_in = (uint8_t*)src;
	s.avail_in = srcSize;

	int r = inflateInit(&s);
	ASS(r == Z_OK);
	if (r != Z_OK) return;


	r = inflate(&s, Z_FINISH);
	ASS(r == Z_STREAM_END);

	inflateEnd(&s);
}


