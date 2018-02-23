#pragma once

template <unsigned long t> struct Polynome { static const unsigned long value = t & 1 ? (t >> 1) ^ 0xedb88320 : t >> 1; };
template <unsigned long t, int i> struct For { static const unsigned long value = For<Polynome<t>::value, i - 1 >::value; };
template <unsigned long t> struct For<t, 0> { static const unsigned long value = Polynome<t>::value; };
template <unsigned long t> struct Hash { static const unsigned long value = For<t, 7>::value; };

template<int r, int t> struct Table : Table<r + 1, t - 1>
{
	Table() { values[t] = Hash<t>::value; }
};
template<int r> struct Table<r, 0>
{
	int values[r + 1];
	Table() { values[0] = Hash<0>::value; }
	int operator[](int i) { return values[i]; }
};

typedef Table<0, 255> CRC_TABLE;

class Crc32Hasher
{
	CRC_TABLE crc_table;
public:
	unsigned long GetHashCode(const void* pObj, size_t length) {
		auto* buf = static_cast<const char*>(pObj);
		unsigned long crc32 = 0xffffffff;
		for (size_t i = 0; i < length; i++)
			crc32 = crc_table[(crc32 ^ (*buf++)) & 0xff] ^ (crc32 >> 8);
		crc32 = crc32 ^ 0xffffffff;
		return crc32;
	}
};