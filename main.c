//elad fixler 215200684
#include "base.h"
struct News* lastNew;
struct News* to_free[BIG_NUMBER];
int to_free_news_counter = 0;
struct BoundedBuffer* to_dispatcher[BIG_NUMBER];
struct UnBoundedBuffer* to_co_editors[3];
struct BoundedBuffer* to_screen_manger;
int num_of_producers = 0;

void* producer(void* arg) {
    struct Producer* p = (struct Producer*) arg;
    //printf("new producer number %d\n", p->number);
    for(int i = 0; i < p->num_of_products; i++) {
        usleep(rand() % 10000);
        insertB(to_dispatcher[p->number - 1], *create_news(p->number, &p->num_of_type));
        //printf("send from producer %d to dispacher\n", p->number);
    }
    //printf("all sent\n");
    insertB(to_dispatcher[p->number - 1], *lastNew);
    //printf("sent last\n");
}

void* dispacher(void* arg) {
    //sleep(5);
    //printf("new dispacher\n");
    int counter = 0;
    while (counter != num_of_producers){
        for (int i = 0; i < num_of_producers; i++) {
            usleep(rand() % 10000);
            //printf("producer number %d: read location = %d, write location = %d\n",i, to_dispatcher[i]->read_location, to_dispatcher[i]->write_location);
            if (!is_emptyB(to_dispatcher[i])) {
                //there is something to read
                struct News news = removeFromBufferB(to_dispatcher[i]);
                //printf("disp got news: %s\n", to_srtring(news));
                if (news.type == 4) {
                    //printf("disp say one of the producers ended\n");
                    counter++;
                    continue;
                }
                insertU(to_co_editors[news.type], news);
            }
        }
    }
    struct News lastNew;
    lastNew.type = -1;
    insertU(to_co_editors[0], lastNew);
    insertU(to_co_editors[1], lastNew);
    insertU(to_co_editors[2], lastNew);
    return NULL;
}

void* co_editor(void* args) {
    //sleep(10);
    //printf("new co editor\n");
    int number = *(int*) args;
    while (true)
    {
        if(!is_emptyU(to_co_editors[number])) {
            struct News news = removeFromBufferU(to_co_editors[number]);
            //printf("to co editor number: %d came new: %s\n", number, to_srtring(news));
            insertB(to_screen_manger, news);
        }
        usleep(rand() % 30000);
    }
    
}
void* screen_manger(void* arg) {
    //sleep(20);
    //printf("new screen manager\n");struct News* create_news(struct Producer* p);
    int counter = 0;
    while(counter != 3) {
        if(!is_emptyB(to_screen_manger)) {
            struct News news = removeFromBufferB(to_screen_manger);
            if (news.type == -1) {
                counter++;
                continue;
            } else {
                char* s = to_srtring(news);
                printf("%s\n", s);
                free(s);
            }
        }
    }
    for (int i = 0; i < to_free_news_counter; i++) {
        free(to_free[i]);
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    //printf("start\n");
    if (argc < 2) {
        printf("too low args");
        return -1;
    }
    if (access(argv[1], F_OK) == -1) {
        printf("invalid file");
        return -1;
    }
    FILE* file = fopen(argv[1], "r");
     if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    char* line = NULL;
    size_t bufferSize = 0;
    ssize_t lineLength;
    int lineCount = 0;
    // Count the number of lines in the file
    while ((lineLength = getline(&line, &bufferSize, file)) != -1) {
        lineCount++;
    }
    num_of_producers = ((lineCount -1)/ 4);
    //printf("num of producers: %d\n", num_of_producers);
    struct Producer* producers = malloc(num_of_producers * sizeof(struct Producer));
    // Reset the file pointer to the beginning of the file
    fseek(file, 0, SEEK_SET);
    int a = 0,b = 0,c = 0;
    // Read the lines one by one
    for (int i = 0; i < lineCount - 1; i++) {
        lineLength = getline(&line, &bufferSize, file);
        if (a == 0) {
            sscanf(line, "%d", &a);
        } else if (b == 0) {
            sscanf(line, "%d", &b);
        } else if (c == 0) {
            sscanf(line, "%d", &c);            
        } else {
            struct Producer p;
            p.number = a;
            p.num_of_products =b;
            p.buffer_size = c;
            p.num_of_type[0] = 0;
            p.num_of_type[1] = 0;
            p.num_of_type[2] = 0;
            a = 0;
            b = 0;
            c = 0;
            producers[(i+1)/4 - 1] = p;
        }
    }
    lastNew = malloc(sizeof(struct News));
    lastNew->type = 4;
    lastNew->producer_number = 1110101010101;
    lastNew->number_of_news = 111001010101011;
    for (int i = 0; i < num_of_producers; i++) {
        //printf("producer number %d with buffer long %d and num of news %d\n", producers[i].number, producers[i].buffer_size, producers[i].num_of_products);
    }
    lineLength = getline(&line, &bufferSize, file);
    int co_editor_lenght;
    sscanf(line, "%d", &co_editor_lenght);
    pthread_t producers_threads[BIG_NUMBER];
    for (int i = 0; i < num_of_producers; i++) {
        to_dispatcher[i] = boundedBuffer(producers[i].buffer_size);
        pthread_create(&producers_threads[i], NULL, producer, &producers[i]);
        //end to create all threads of producers
    }
    pthread_t dispacher_thred;
    pthread_create(&dispacher_thred, NULL, dispacher, NULL);

    to_co_editors[0] = new_unBoundedBuffer();
    to_co_editors[1] = new_unBoundedBuffer();
    to_co_editors[2] = new_unBoundedBuffer();

    pthread_t co_editor_theads[3];
    int arg1 = 0;
    int arg2 = 1;
    int arg3 = 2;
    pthread_create(&co_editor_theads[0], NULL, co_editor, &arg1);
    pthread_create(&co_editor_theads[1], NULL, co_editor, &arg2);
    pthread_create(&co_editor_theads[2], NULL, co_editor, &arg3);

    to_screen_manger = boundedBuffer(co_editor_lenght);

    pthread_t screen_manger_thread;
    pthread_create(&screen_manger_thread, NULL, screen_manger, NULL);
    pthread_join(screen_manger_thread, NULL);
    free(producers);
    for (int i = 0; i < BIG_NUMBER; i++) {
        if (to_dispatcher[i] != NULL) {
            destroyB(to_dispatcher[i]);
            free(to_dispatcher[i]);
        }
    }

    destroyU(to_co_editors[0]);
    free(to_co_editors[0]);
    destroyU(to_co_editors[1]);
    free(to_co_editors[1]);
    destroyU(to_co_editors[2]);
    free(to_co_editors[2]);

    destroyB(to_screen_manger);
    free(to_screen_manger);
    free(lastNew);
    printf("DONE");
}
