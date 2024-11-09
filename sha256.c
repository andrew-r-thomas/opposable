/*
	 TODO:
	 - handle message blocks larger than one 512 bit chunk
	 - figure out libc stuff like stdint and memcpy etc
	 - add function to tell how much mem will be needed for whole hash,
	   then allocate everything you will need all at start (on stack)
*/

#include <stdint.h>
#include <stdio.h>

const uint32_t K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};
const uint32_t H[8] = {
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

uint32_t rotate_right(uint32_t i, uint8_t d) { return (i >> d) | (i << (32 - d)); }

// message len is the number of bits in the message
// returns the number of bytes for the message block
uint64_t get_message_block_bytes(uint64_t message_bits)
{
	uint64_t message_block_bits = 512;
	message_bits += 67; // adding room for 1 bit and the 64 bit message len
	while (message_block_bits < message_bits) {
		message_block_bits += 512;
	}
	return message_block_bits / 8;
}

// message block should be zeroed
void prepare(uint8_t* message, uint64_t message_bits, uint8_t* message_block, uint64_t message_block_bytes)
{
	for (uint8_t i = 0; i < message_bits / 8; i++)
	{
		message_block[i] = message[i];
	}

	message_block[message_bits / 8] |= (1 << 7);

	for (uint8_t i = 0; i < 8; i++)
	{
		message_block[message_block_bytes - 1 - i] = (message_bits >> (i * 8)) & 0xff;
	}
}

// message_schedule is 64 32 bit ints
// TODO: for now we are assuming message_block is exactly 512 bits
void hash(uint32_t* message_schedule, uint8_t* message_block, uint8_t message_block_chunks, uint8_t* out)
{
	uint32_t h[8];
	for (uint8_t i = 0; i < 8; i++)
	{
		h[i] = H[i];
	}

	for (uint8_t c = 0; c < message_block_chunks; c++) // 🤮
	{
		uint32_t w[8];
		for (uint8_t i = 0; i < 8; i++)
		{
			w[i] = h[i];
		}
		uint8_t offset = c * 64;
		for (uint8_t i = 0; i < 16; i++)
		{
			message_schedule[i] = 
				(message_block[i*4 + offset] << 24) |
				(message_block[(i*4)+1 + offset] << 16) |
				(message_block[(i*4)+2 + offset] << 8) |
				message_block[(i*4)+3 + offset];
		}

		for (uint8_t word_i = 16; word_i < 64; word_i++)
		{
			uint32_t sig0 = rotate_right(message_schedule[word_i - 15], 7)
				^ rotate_right(message_schedule[word_i - 15], 18)
				^ (message_schedule[word_i - 15] >> 3);
			uint32_t sig1 = rotate_right(message_schedule[word_i - 2], 17)
				^ rotate_right(message_schedule[word_i - 2], 19)
				^ (message_schedule[word_i - 2] >> 10);
			message_schedule[word_i] = message_schedule[word_i - 16] + sig0 + message_schedule[word_i - 7] + sig1;
		}


		for (uint8_t i = 0; i < 64; i++)
		{
			uint32_t sig0 = rotate_right(h[0], 2) ^ rotate_right(h[0], 13) ^ rotate_right(h[0], 22);
			uint32_t sig1 = rotate_right(h[4], 6) ^ rotate_right(h[4], 11) ^ rotate_right(h[4], 25);
			uint32_t choice = (h[4] & h[5]) ^ ((~ h[4]) & h[6]);
			uint32_t maj = (h[0] & h[1]) ^ (h[0] & h[2]) ^ (h[1] & h[2]);
			uint32_t temp1 = h[7] + sig1 + choice + K[i] + message_schedule[i];
			uint32_t temp2 = sig0 + maj;

			h[7] = h[6];
			h[6] = h[5];
			h[5] = h[4];
			h[4] = h[3] + temp1;
			h[3] = h[2];
			h[2] = h[1];
			h[1] = h[0];
			h[0] = temp1 + temp2;
		}

		for (uint8_t i = 0; i < 8; i++)
		{
			h[i] += w[i];
		}
	}



	for (uint8_t i = 0; i < 8; i++)
	{
		uint32_t val = h[i];
		for (uint8_t j = 0; j < 4; j++)
		{
			out[(i*4) + 3 - j] = (val >> (j * 8)) & 0xff;
		}
	}
}
