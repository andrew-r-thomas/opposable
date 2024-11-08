#include "sha256.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	uint8_t message[] = "hello, my name is macintosh!, and yeah, idk man, its hard to say";
	uint64_t message_bits = (sizeof(message) - 1) * 8;

	uint64_t message_block_bytes = get_message_block_bytes(message_bits);
	uint8_t* message_block = malloc(message_block_bytes);

	prepare(message, message_bits, message_block, message_block_bytes);

	printf("message bits: %d\n\n", message_bits);
	printf("message block bytes: %d\n\n", message_block_bytes);
	printf("message block:\n");
	for (int i = 0; i < message_block_bytes / 4; i++)
	{
		for (int j = i * 4; j < (i * 4) + 4; j++)
		{
			char val = message_block[j];
			
			for (int i = 7; 0 <= i; i--)
			{
				printf("%u", (val >> i) & 1);
			}
			printf(" ");
		}
		printf("\n");
	}
}

