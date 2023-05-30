//elad fixler 215200684
#include"base.h"
int a;
int b;
struct News* create_news(int producer_number) {
    struct News* news = malloc(sizeof(struct News));
    news->number_of_news = (b++) % 3;
    news->producer_number = producer_number;
    news->type = (a++) % 3;
    return news;
}
char* to_srtring(struct News news) {
    char *ret = malloc(MAX_LENGHT * sizeof(char)); 
    strcpy(ret, "producer ");
    char numbers[20];
    sprintf(numbers, "%d", news.producer_number);
        strcat(ret, numbers);
                switch (news.type)
                {
                case SPORT:
                    strcat(ret, " SPORT ");
                    break;
                case NEWS:
                    strcat(ret, " NEWS ");
                    break;
                case WEATHER: 
                    strcat(ret, " WEATHER ");
                    break;
                default:
                    strcat(ret, " LAST ");
                    break;
                }
    sprintf(numbers, "%d", news.number_of_news);
    strcat(ret, numbers);
    return ret;
}