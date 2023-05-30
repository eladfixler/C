//elad fixler 215200684
#include"base.h"
struct BoundedBuffer* boundedBuffer(int size) {
    struct BoundedBuffer* ret = malloc(sizeof(struct BoundedBuffer));
    ret->read_location = 0;
    ret->write_location = 0;
    ret->buffer = malloc(size * sizeof(struct News));
    ret->size = size;
    sem_init(&ret->semaphore, 0, 1);
    return ret;
}
void insertB(struct BoundedBuffer* boundedBuffer , struct News s) {

    while((boundedBuffer->read_location == (boundedBuffer->write_location + 1) % boundedBuffer->size ));
    sem_wait(&boundedBuffer->semaphore);
    boundedBuffer->write_location = (boundedBuffer->write_location + 1) % boundedBuffer->size;
    boundedBuffer->buffer[boundedBuffer->write_location] = s;
    //printf("added to buufer size %d and wire in %d the new is %s\n", boundedBuffer->size, boundedBuffer->write_location, to_srtring(s));
    sem_post(&boundedBuffer->semaphore);
}
struct News removeFromBufferB(struct BoundedBuffer* boundedBuffer) {
    sem_wait(&boundedBuffer->semaphore);
    struct News news;
    boundedBuffer->read_location = (boundedBuffer->read_location + 1) % boundedBuffer->size;
    news = boundedBuffer->buffer[boundedBuffer->read_location];
    sem_post(&boundedBuffer->semaphore);
    return news;
}
bool is_emptyB(struct BoundedBuffer* boundedBuffer) {
    return boundedBuffer->read_location == boundedBuffer->write_location;
}
void destroyB(struct BoundedBuffer* boundedBuffer) {
    free(boundedBuffer->buffer);
    sem_destroy(&boundedBuffer->semaphore);
}