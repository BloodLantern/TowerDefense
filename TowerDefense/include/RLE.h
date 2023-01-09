#pragma once

#include "PlayField.h"

namespace RLE
{
	/// <summary>
	/// Compresses a level to a file
	/// </summary>
	/// <param name="pf">Playfield pointer</param>
	/// <param name="dst">Destination file name</param>
	/// <returns>True if success, false otherwise (error logged in console)</returns>
	bool CompressLevel(PlayField* pf, const char* dst);

	/// <summary>
	/// Decompresses a level from a file
	/// </summary>
	/// <param name="pf">Playfield pointer</param>
	/// <param name="dst">Source file name</param>
	/// <returns>True if success, false otherwise (error logged in console)</returns>
	bool DecompressLevel(PlayField* pf, const char* src);

	/// <summary>
	/// Compresses data treated as bytes with RLE into a specified file
	/// </summary>
	/// <param name="src">Source pointer</param>
	/// <param name="length">Length of data to compress</param>
	/// <param name="dst">Destination file name</param>
	/// <returns>True if success, false otherwise (error logged in console)</returns>
	bool Compress(void* src, size_t length, const char* dst);

	/// <summary>
	/// Decompresses a RLE compressed file into a buffer
	/// </summary>
	/// <param name="dst">Destination pointer</param>
	/// <param name="src">Source file name</param>
	/// <returns>True if success, 0 otherwise (error logged in console)</returns>
	bool Decompress(void* dst, const char* src);
};
