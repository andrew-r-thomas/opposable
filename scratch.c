#include "sha256.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void print_bytes(uint8_t* bytes, uint64_t len)
{

	for (int i = 0; i < len / 4; i++)
	{
		for (int j = i * 4; j < (i * 4) + 4; j++)
		{
			uint8_t val = bytes[j];
			
			for (int i = 7; 0 <= i; i--)
			{
				printf("%u", (val >> i) & 1);
			}
			printf(" ");
		}
		printf("\n");
	}
}

void print_words(uint32_t* words, uint64_t len)
{
	for (int i = 0; i < len; i++)
	{
		uint32_t word = words[i];
		for (int i = 31; 0 <= i; i--)
		{
			printf("%u", (word >> i) & 1);
		}
		printf(": %d\n", i);
	}
}

void print_hex(uint8_t* bytes, uint64_t len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%02X", bytes[i]);
	}
}

int main()
{
	uint8_t message[] = "howdy, im andrew, and this is a long message that will take more than one block";
	uint64_t message_bits = (sizeof(message) - 1) * 8;

	uint64_t message_block_bytes = get_message_block_bytes(message_bits);
	uint8_t* message_block = malloc(message_block_bytes);

	prepare(message, message_bits, message_block, message_block_bytes);

	uint32_t message_schedule[64] = {0};
	uint8_t out[32] = {0};
	uint8_t message_chunks = message_block_bytes / 64;
	printf("message chunks: %d\n", message_chunks);
	hash(message_schedule, message_block,message_chunks, out);

	printf("hash: ");
	print_hex(out, 32);
	printf("\n");
}
