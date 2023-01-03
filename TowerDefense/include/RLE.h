#pragma once

class RLE
{
public:
	/// <summary>
	/// Compresses data treated as bytes with RLE into a specified file
	/// </summary>
	/// <param name="src">Source pointer</param>
	/// <param name="length">Length of data to compress</param>
	/// <param name="dst">Destination file name</param>
	/// <returns>True if success, false otherwise (error logged in console)</returns>
	static bool compress(void* src, size_t length, const char* dst);

	/// <summary>
	/// Decompresses a RLE compressed file into a buffer
	/// </summary>
	/// <param name="dst">Destination pointer</param>
	/// <param name="src">Source file name</param>
	/// <returns>True if success, 0 otherwise (error logged in console)</returns>
	static bool decompress(void* dst, const char* src);
};
