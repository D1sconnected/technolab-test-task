#ifndef __CIRCULARBUFFER_H
#define __CIRCULARBUFFER_H

typedef unsigned char byte;

#define RECORD_COUNT 10
#define RECORD_SIZE  11 // "ADC0,E,OFF\r"

#include <stdbool.h>

typedef struct MsgStruct
{
	int  msg_size;
	byte msg[RECORD_SIZE];
} MsgStruct;

typedef struct CircularBufferStruct
{
    int count;
    int index;
    int outdex;
    int capacity;
    int record_size;
//    byte *msg;
//    int  *msg_size;
    MsgStruct record[RECORD_COUNT]; // [RECORD_SIZE] bytes for msg + 4 bytes for msg size
} CircularBufferStruct;

typedef struct CircularBufferStruct * CircularBuffer;

CircularBuffer CircularBuffer_Create(int capacity, int record_size);
void CircularBuffer_Destroy(CircularBuffer);
int CircularBuffer_IsEmpty(CircularBuffer);
int CircularBuffer_IsFull(CircularBuffer);
int CircularBuffer_Put(CircularBuffer, void *, int size);
int CircularBuffer_Get(CircularBuffer, void *);
int CircularBuffer_Capacity(CircularBuffer);
void CircularBuffer_Print(CircularBuffer);
int CircularBuffer_VerifyIntegrity(CircularBuffer);

#endif // __CIRCULARBUFFER_H
