#ifndef BB
#define BB
#include "stdbool.h"
#include "base.h"
struct BoundedBuffer
{
    int write_location;
    int read_location;
    int size;
    sem_t semaphore;
    struct News* buffer;
};
bool is_emptyB(struct BoundedBuffer* boundedBuffer);
struct BoundedBuffer* boundedBuffer(int size);
void insertB(struct BoundedBuffer* boundedBuffer , struct News s);
struct News removeFromBufferB(struct BoundedBuffer* boundedBuffer);
void destroyB(struct BoundedBuffer* boundedBuffer);
#endif