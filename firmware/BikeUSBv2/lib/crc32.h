//was too layzy to do this my self, instead used " simplest crc32 c++ implementation "
//available at https://gist.github.com/timepp/1f678e200d9e0f2a043a9ec6b3690635
//update single added

#pragma once

#include <stdint.h>

struct crc32
{
	static void generate_table(uint32_t(&table)[256])
	{
		uint32_t polynomial = 0xEDB88320;
		for (uint32_t i = 0; i < 256; i++) 
		{
			uint32_t c = i;
			for (size_t j = 0; j < 8; j++) 
			{
				if (c & 1) {
					c = polynomial ^ (c >> 1);
				}
				else {
					c >>= 1;
				}
			}
			table[i] = c;
		}
	}

	static uint32_t update(uint32_t (&table)[256], uint32_t initial, const void* buf, size_t len)
	{
		uint32_t c = initial ^ 0xFFFFFFFF;
		const uint8_t* u = static_cast<const uint8_t*>(buf);
		for (size_t i = 0; i < len; ++i) 
		{
			c = table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
		}
		return c ^ 0xFFFFFFFF;
	}

	static uint32_t update(uint32_t (&table)[256], uint32_t initial, const uint8_t val)
	{
		uint32_t c = initial ^ 0xFFFFFFFF;
		c = table[(c ^ val) & 0xFF] ^ (c >> 8);
		return c ^ 0xFFFFFFFF;
	}
};
