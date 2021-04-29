/*
 * -----
 * Code by: Abhijeet Suryawanshi
 * Student Number: 19370773
 * Email: abhijeet.suryawanshi@ucdconnect.ie
 * -----
 */
#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <stdio.h>
#include <stdlib.h>

unsigned char *serializeInt(unsigned char *buffer, int num);
unsigned char *serializeDouble(unsigned char *buffer, double num);

/* 
 * This function is used to serialize the integer values that need to be sent across networks.
 * It takes in a buffer to store the serialized data and an integer to serialize.
 * The loop goes on for 4 iterations since the size of an int is 4 bytes, and
 * every iteration, the buffer is incremented and a bit-shift of 8 bits occurs. 
 */
unsigned char *serializeInt(unsigned char *buffer, int num)
{

	for (size_t i = 0; i < 4; i++)
	{

		*buffer++ = num >> (8 * i);
	}

	return buffer;
}

/* 
 * This function works in a similar way that integer serialization works, but in
 * this case, the double is first copied to an unsigned long value and then the 
 * bit-shifting takes place for a total of 8 iterations since a double value is stored
 * using 8 bytes.
 */
unsigned char *serializeDouble(unsigned char *buffer, double num)
{

	unsigned long numAsLong;

	memcpy(&numAsLong, &num, 8);

	for (size_t i = 0; i < 8; i++)
	{

		*buffer++ = numAsLong >> (8 * i);
	}

	return buffer;
}

#endif