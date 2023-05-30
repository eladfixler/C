//elad fixler 215200684
#ifndef UBB
#define UBB
#include "base.h"
struct Node
{   
    struct News news;
    struct Node* next;
};
struct UnBoundedBuffer
{
    struct Node* head;
    sem_t semaphore;
};
struct Node* new_node(struct News news);
struct UnBoundedBuffer* new_unBoundedBuffer();
void insertU(struct UnBoundedBuffer* unBoundedBuffer , struct News s);
struct News removeFromBufferU(struct UnBoundedBuffer* unBoundedBuffer);
void destroyU(struct UnBoundedBuffer* unBoundedBuffer);
bool is_emptyU(struct UnBoundedBuffer* unBoundedBuffer);
#endif