#include "CircularBuffer.h"
#include <stdlib.h>
#include <string.h>

enum { BUFFER_GUARD = 99 };

CircularBuffer CircularBuffer_Create(int capacity, int record_size)
{
    CircularBuffer self = calloc(1, sizeof(CircularBufferStruct));
    if(self == NULL)
        return NULL;
    
    self->capacity = capacity;
    self->record_size = record_size;
    
//    self->msg = calloc(capacity + 1, record_size); // allocate mem for msg
//    self->msg_size = calloc(capacity + 1, sizeof(int)); // allocate mem for msg size
//    if(self->msg == NULL)
//    {
//        free(self);
//        return NULL;
//    }
    //self->record[capacity * record_size + capacity * sizeof(int)] = BUFFER_GUARD;
    
    return self;
}

void CircularBuffer_Destroy(CircularBuffer self)
{
    free(self);
}

//int CircularBuffer_VerifyIntegrity(CircularBuffer self)
//{
//    return self->msg[self->capacity * self->record_size + self->capacity * sizeof(int)] == BUFFER_GUARD;
//}

int CircularBuffer_IsEmpty(CircularBuffer self)
{
    return self->count == 0;
}

int CircularBuffer_IsFull(CircularBuffer self)
{
    return self->count == self->capacity;
}

int CircularBuffer_Put(CircularBuffer self, void *record, int size)
{
	if (self == NULL || (self->outdex - self->index) == 1)
	{
		return 0;
	}

    self->record[self->index].msg_size = size;
    memcpy(&self->record[self->index].msg, record, self->record[self->index].msg_size);
    self->index++;
    if (self->index >= self->capacity)
        self->index = 0;

    return 1;
}

int CircularBuffer_Get(CircularBuffer self, void *record)
{
    if (self == NULL || (self->outdex == self->index))
    {
        return 0;
    }

    memcpy(record, &self->record[self->outdex].msg, self->record[self->outdex].msg_size);
    self->outdex++;

    if (self->outdex >= self->capacity)
    {
        self->outdex = 0;
    }

    return 1;
}

int CircularBuffer_Capacity(CircularBuffer self)
{
    return self->capacity;
}

//void CircularBuffer_Print(CircularBuffer self)
//{
//    int i;
//    int currentValue;
//
//    currentValue = self->outdex;
//
//    FormatOutput("Circular buffer content:\n<");
//
//    for (i = 0; i < self->count; i++) {
//        if (i != 0)
//            FormatOutput(", ");
//        FormatOutput("%d", self->records[currentValue++]);
//        if (currentValue >= self->capacity)
//            currentValue = 0;
//    }
//
//    FormatOutput(">\n");
//}
