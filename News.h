//elad fixler 215200684
#ifndef N_H
#define N_H
struct News
{
    int producer_number;
    int type;
    int number_of_news;
};
struct News* create_news(int producer_number);
char* to_srtring(struct News);
#endif