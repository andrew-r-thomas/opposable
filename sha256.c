#include <stdint.h>

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
	// PERF: figure out if we can do mem copy
	for (uint64_t i = 0; i < message_bits / 8; i++)
	{
		message_block[i] = message[i];
	}

	message_block[message_bits / 8] |= (1 << 7);

	for (uint64_t i = 0; i < 8; i++)
	{
		message_block[message_block_bytes - 1 - i] = (message_bits >> (i * 8)) & 0xff;
	}
}

// message_schedule is 64 32 bit ints
void hash(uint32_t* message_schedule)
{}
