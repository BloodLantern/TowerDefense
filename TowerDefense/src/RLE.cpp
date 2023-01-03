#include "RLE.h"
#include <iostream>

bool RLE::compress(void* src, size_t length, const char* dst)
{
	FILE* f;
	fopen_s(&f, dst, "wb");

	if (!f)
	{
		std::cout << "ERROR - RLE Compression : Couldn't open " << dst << " file" << std::endl;
		return false;
	}

	uint8_t* _src = static_cast<uint8_t*>(src);
	
	size_t i = 0;
	while (i < length)
	{
		uint8_t value = _src[i++];
		uint8_t size = 1;

		while (value == _src[i])
		{
			i++;
			if (i > length || size == UCHAR_MAX)
				break;

			size++;
		}

		fwrite(&size, sizeof(size), 1, f);
		fwrite(&value, sizeof(value), 1, f);
	}

	fclose(f);
	return true;
}

bool RLE::decompress(void* dst, const char* src)
{
	FILE* f;
	fopen_s(&f, src, "rb");

	if (!f)
	{
		std::cout << "ERROR - RLE Decompression : Couldn't open " << src << " file" << std::endl;
		return false;
	}

	uint8_t* _dst = static_cast<uint8_t*>(dst);
	while (true)
	{
		uint8_t size;
		uint8_t value;
		fread(&size, sizeof(size), 1, f);
		if (size == UCHAR_MAX)
			break;

		fread(&value, sizeof(value), 1, f);

		while (size--)
			*_dst++ = value;
	}

	fclose(f);

	return true;
}
