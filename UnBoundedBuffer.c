//elad fixler 215200684
#include"base.h"
struct Node* new_node(struct News news) {
    struct Node* ret;
    ret = malloc(sizeof(struct Node));
    ret->news = news;
    ret->next = NULL;
    return ret;
}
struct UnBoundedBuffer* new_unBoundedBuffer() {
    struct UnBoundedBuffer* unBoundedBuffer = malloc(sizeof(struct UnBoundedBuffer));
    unBoundedBuffer->head = NULL;
    sem_init(&unBoundedBuffer->semaphore, 0, 1);
    return unBoundedBuffer;
}
void insertU(struct UnBoundedBuffer* unBoundedBuffer , struct News s) {
    sem_wait(&unBoundedBuffer->semaphore);
    if (unBoundedBuffer->head == NULL) {
        unBoundedBuffer->head = new_node(s);
        sem_post(&unBoundedBuffer->semaphore);
        return;
    }
    struct Node* node = unBoundedBuffer->head;
    while (node->next != NULL) {
        node = node->next;
    }
    node->next = new_node(s);
    sem_post(&unBoundedBuffer->semaphore);
}
struct News removeFromBufferU(struct UnBoundedBuffer* unBoundedBuffer) {
    sem_wait(&unBoundedBuffer->semaphore);
    struct News news = unBoundedBuffer->head->news;
    struct Node * second = unBoundedBuffer->head->next;
    free(unBoundedBuffer->head);
    unBoundedBuffer->head = second;
    sem_post(&unBoundedBuffer->semaphore);
    return news;
}
void destroyU(struct UnBoundedBuffer* unBoundedBuffer) {
        sem_destroy(&unBoundedBuffer->semaphore);
}
bool is_emptyU(struct UnBoundedBuffer* unBoundedBuffer) {
    return unBoundedBuffer->head == NULL;
}