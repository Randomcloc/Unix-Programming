/*
 * -----
 * Code by: Abhijeet Suryawanshi
 * Student Number: 19370773
 * Email: abhijeet.suryawanshi@ucdconnect.ie
 * -----
 */
#ifndef DESERIALIZATION_H
#define DESERIALIZATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int deSerializeInt(unsigned char *buffer, unsigned char **bufferOut);
double deSerializeDouble(unsigned char *buffer, unsigned char **bufferOut);

/* 
 * This function de-serializes the integer value that was sent across networks.
 * The function takes in a buffer and a pointer to the buffer and returns an integer.
 * Since an integer is 4 bytes, a bit-shift of 8 bits after reading occurs 4 times
 * and the buffer is offset by 4 bytes at the end.
 */
int deSerializeInt(unsigned char *buffer, unsigned char **bufferOut)
{

	int num = 0;

	for (size_t i = 0; i < 4; i++)
	{

		num |= buffer[i] << (8 * i);
	}

	*bufferOut = buffer + 4;

	return num;
}

/* 
 * This function de-serializes a double value found in an unsigned char buffer array.
 * It takes in a buffer and a pointer to the buffer and returns a double.
 * The value extracted from the string is first stored in an unsigned long and then is
 * copied into a double variable and the buffer is then offset by 8 bytes.
 */
double deSerializeDouble(unsigned char *buffer, unsigned char **bufferOut)
{

	unsigned long numAsLong = 0;
	double num = 0;

	for (size_t i = 0; i < 8; i++)
	{

		numAsLong |= (unsigned long long)buffer[i] << (8 * i);
	}

	memcpy(&num, &numAsLong, 8);
	*bufferOut = buffer + 8;

	return num;
}

#endif