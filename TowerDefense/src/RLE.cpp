#include "RLE.h"

#include <iostream>

/*
 * Level file format :
 * 
 * 0 | 2 | Grid width
 * 2 | 2 | Grid height
 * 4 | 1 | Tileset ID (TODO)
 * 5 | 1X | Clipdata
 * X | 2X | Layer 0 tilemap
 * X | 2X | Layer 1 tilemap
 * 
*/

bool RLE::compressLevel(PlayField* pf, const char* dst)
{
	if (!pf)
	{
		std::cerr << "ERROR - RLE Level Compression : PlayField pointer was null" << std::endl;
		return false;
	}

	FILE* f;
	fopen_s(&f, dst, "wb");

	if (!f)
	{
		std::cerr << "ERROR - RLE Level Compression : Couldn't open " << dst << " file" << std::endl;
		return false;
	}

	// Write size
	fwrite(&pf->m_gridWidth, sizeof(pf->m_gridWidth), 1, f);
	fwrite(&pf->m_gridHeight, sizeof(pf->m_gridHeight), 1, f);

	// TODO tileset IDs
	uint8_t zero = 0;
	fwrite(&zero, sizeof(zero), 1, f);

	size_t length = (size_t)pf->m_gridWidth * pf->m_gridHeight;

	uint8_t* _src8 = (uint8_t*)(pf->getClipdataPointer());

	size_t i = 0;
	while (i < length)
	{
		uint8_t value = _src8[i++];
		uint8_t size = 1;

		while (value == _src8[i])
		{
			if (i > length || size == UCHAR_MAX)
				break;

			i++;
			size++;
		}

		fwrite(&size, sizeof(size), 1, f);
		fwrite(&value, sizeof(value), 1, f);
	}

	uint16_t* _src16 = pf->getTilemapPointer(0);

	i = 0;
	while (i < length)
	{
		uint16_t value = _src16[i++];
		uint8_t size = 1;

		while (value == _src16[i])
		{
			if (i > length || size == UCHAR_MAX)
				break;

			i++;
			size++;
		}

		fwrite(&size, sizeof(size), 1, f);
		fwrite(&value, sizeof(value), 1, f);
	}

	_src16 = pf->getTilemapPointer(1);

	i = 0;
	while (i < length)
	{
		uint16_t value = _src16[i++];
		uint8_t size = 1;

		while (value == _src16[i])
		{
			if (i > length || size == UCHAR_MAX)
				break;

			i++;
			size++;
		}

		fwrite(&size, sizeof(size), 1, f);
		fwrite(&value, sizeof(value), 1, f);
	}

	fclose(f);
	return true;
}

bool RLE::decompressLevel(PlayField* pf, const char* src)
{
	if (!pf)
	{
		std::cerr << "ERROR - RLE Level Decompression : PlayField pointer was null" << std::endl;
		return false;
	}

	FILE* f;
	fopen_s(&f, src, "rb");

	if (!f)
	{
		std::cerr << "ERROR - RLE Level Decompression : Couldn't open " << src << " file" << std::endl;
		return false;
	}

	uint16_t width;
	fread(&width, sizeof(width), 1, f);
	uint16_t height;
	fread(&height, sizeof(width), 1, f);

	pf->resize(width, height);

	size_t length = (size_t)width * height;

	// TODO tileset
	uint8_t tileset = fgetc(f);

	uint8_t* _dst8 = (uint8_t*)(pf->getClipdataPointer());
	uint32_t i = 0;

	while (i < length)
	{
		uint8_t size = fgetc(f);
		uint8_t value = fgetc(f);

		while (size--)
			_dst8[i++] = value;
	}

	uint16_t* _dst16 = pf->getTilemapPointer(0);
	i = 0;

	while (i < length)
	{
		uint8_t size = fgetc(f);

		uint16_t value;
		fread(&value, sizeof(value), 1, f);

		while (size--)
			_dst16[i++] = value;
	}

	_dst16 = pf->getTilemapPointer(1);
	i = 0;

	while (i < length)
	{
		uint8_t size = fgetc(f);

		uint16_t value;
		fread(&value, sizeof(value), 1, f);

		while (size--)
			_dst16[i++] = value;
	}

	fclose(f);
	return true;
}

bool RLE::compress(void* src, size_t length, const char* dst)
{
	FILE* f;
	fopen_s(&f, dst, "wb");

	if (!f)
	{
		std::cerr << "ERROR - RLE Compression : Couldn't open " << dst << " file" << std::endl;
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
		std::cerr << "ERROR - RLE Decompression : Couldn't open " << src << " file" << std::endl;
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
