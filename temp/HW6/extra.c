#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUITS 4
#define FACES 13
#define CARDS 52
#define DRAW 5
#define TOTAL 2598960

void shuffle(unsigned int wDeck[][FACES]); // shuffling modifies wDeck
void deal(unsigned int wDeck[][FACES], unsigned int num,
          unsigned int Ds[], unsigned int Df[]);
int comp(const void *a, const void *b);
char apair(const unsigned int Df[]);
char dpair(const unsigned int Df[]); // ? Does two pairs contain a pair? -assuming not
char ToK(const unsigned int Df[]);   // ? Does Tuple contain a pair? -assuming not
char FoK(const unsigned int Df[]);   // ? Does Four of a kind contain Three of a kind? -assuming not
char flush(const unsigned int Ds[]);
char straight(const unsigned int Df[]);
char RF(const unsigned int Ds[], const unsigned int Df[]);
char SF(const unsigned int Ds[], const unsigned int Df[]);
char FH(const unsigned int Df[]);
char check(const unsigned int Ds[], const unsigned int Df[]);
void initialize(unsigned int deck[][FACES]);

int main(void)
{
    unsigned int Ds[DRAW] = {0}, Df[DRAW] = {0}, count[11] = {0}, t0 = 0;

    // initialize deck array
    unsigned int deck[SUITS][FACES] = {0};

    t0 = time(NULL);
    srand(t0);
    shuffle(deck);

    for (int i = 0; i < TOTAL; i++)
    {
        deal(deck, DRAW, Ds, Df);
        count[check(Ds, Df)]++;
        srand(t0 + i);
        initialize(deck);
        shuffle(deck);
    }
    printf("%-33s%9d\n", "Total number of hands dealt:", TOTAL);
    for (int i = 1; i <= 10; i++)
    {
        printf("The occurrence of ");
        switch (i)
        {
        case 1:
            printf("%-16s%8d\n", "Royal Flush:", count[i]);
            break;
        case 2:
            printf("%-16s%8d\n", "Straight Flush:", count[i]);
            break;
        case 3:
            printf("%-16s%8d\n", "Four of a Kind:", count[i]);
            break;
        case 4:
            printf("%-16s%8d\n", "Full House:", count[i]);
            break;
        case 5:
            printf("%-16s%8d\n", "Flush:", count[i]);
            break;
        case 6:
            printf("%-16s%8d\n", "Straight:", count[i]);
            break;
        case 7:
            printf("%-16s%8d\n", "Three of a Kind:", count[i]);
            break;
        case 8:
            printf("%-16s%8d\n", "Two pairs:", count[i]);
            break;
        case 9:
            printf("%-16s%8d\n", "One pairs:", count[i]);
            break;
        case 10:
            printf("%-16s%8d\n", "Nothing:", count[i]);
            break;
        }
    }
    printf("\nTime elapsed in second: %d\n", time(NULL) - t0);
}

// shuffle cards in deck
void shuffle(unsigned int wDeck[][FACES])
{
    // for each of the cards, choose slot of deck randomly
    for (size_t card = 1; card <= CARDS; card++)
    {
        size_t row;    // row number
        size_t column; // column number

        // choose new random location until unoccupied slot found
        do
        {
            row = rand() % SUITS;
            column = rand() % FACES;
        } while (wDeck[row][column] != 0);

        // place card number in chosen slot of deck
        wDeck[row][column] = card;
    }
}

// deal cards in deck
void deal(unsigned int wDeck[][FACES], unsigned int num,
          unsigned int Ds[], unsigned int Df[])
{
    // deal (num) of cards
    for (size_t card = 1; card <= num; card++)
    {
        // loop through rows of wDeck
        for (size_t row = 0; row < SUITS; row++)
        {
            // loop through columns of wDeck for current row
            for (size_t column = 0; column < FACES; column++)
            {
                // if slot contains current card, draw the card
                if (wDeck[row][column] == card)
                {
                    Ds[card - 1] = row;
                    Df[card - 1] = column;
                }
            }
        }
    }
    qsort(Df, num, sizeof(*(Df)), comp);
}

int comp(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

char apair(const unsigned int Df[])
{
    // counter: number of times Face appears, index = Face ; value = frequency
    // scouter: number of times same Face appears, index = number of the same Face ; value = frequency
    unsigned int counter[FACES] = {0}, scounter[DRAW + 1] = {0};
    int i;
    for (i = 0; i < DRAW; i++)
        counter[Df[i]]++;
    for (i = 0; i < FACES; i++)
        scounter[counter[i]]++;
    if (scounter[2] == 1)
        return 1;
    return 0;
}

char dpair(const unsigned int Df[])
{
    unsigned int counter[FACES] = {0}, scounter[DRAW + 1] = {0};
    int i;
    for (i = 0; i < DRAW; i++)
        counter[Df[i]]++;
    for (i = 0; i < FACES; i++)
        scounter[counter[i]]++;
    if (scounter[2] == 2)
    {
        // for (i = 0; i < DRAW; i++)
        //     printf("%d ", Df[i]);
        // printf("\n");
        return 1;
    }
    return 0;
}

char ToK(const unsigned int Df[])
{
    unsigned int counter[FACES] = {0}, scounter[DRAW + 1] = {0};
    int i;
    for (i = 0; i < DRAW; i++)
        counter[Df[i]]++;
    for (i = 0; i < FACES; i++)
        scounter[counter[i]]++;
    if (scounter[3] == 1)
        return 1;
    return 0;
}

char FoK(const unsigned int Df[])
{
    unsigned int counter[FACES] = {0}, scounter[DRAW + 1] = {0};
    int i;
    for (i = 0; i < DRAW; i++)
        counter[Df[i]]++;
    for (i = 0; i < FACES; i++)
        scounter[counter[i]]++;
    if (scounter[4] == 1)
        return 1;
    return 0;
}

char flush(const unsigned int Ds[])
{
    unsigned int counter[SUITS] = {0};
    for (int i = 0; i < DRAW; i++)
        counter[Ds[i]]++;
    for (int i = 0; i < SUITS; i++)
    {
        if (counter[i] == 5)
            return 1;
    }
    return 0;
}

char straight(const unsigned int Df[])
{
    double sum1 = 0, sum2 = 0, d = 0;

    if (Df[0] == 0 && Df[DRAW - 1] == 12)
    {
        for (int i = 1; i < DRAW; i++)
            sum1 += Df[i];
        sum2 = 0.5 * (DRAW - 1) * (Df[1] + Df[DRAW - 1]);
        d = 1.0 * (Df[DRAW - 1] - Df[1]) / (DRAW - 2);
        return (sum1 == sum2 && d == 1.0);
    }
    else
    {
        for (int i = 0; i < DRAW; i++)
            sum1 += Df[i];
        sum2 = 0.5 * DRAW * (Df[0] + Df[DRAW - 1]);
        d = 1.0 * (Df[DRAW - 1] - Df[0]) / (DRAW - 1); // !! int to double
        return (sum1 == sum2 && d == 1.0);
    }
}

char RF(const unsigned int Ds[], const unsigned int Df[])
{
    double sum1 = 0, sum2 = 0, d = 0;
    if (flush(Ds))
    {
        if (Df[0] == 0 && Df[DRAW - 1] == 12)
        {
            for (int i = 1; i < DRAW; i++)
                sum1 += Df[i];
            sum2 = 0.5 * (DRAW - 1) * (Df[1] + Df[DRAW - 1]);
            d = 1.0 * (Df[DRAW - 1] - Df[1]) / (DRAW - 2);
            return (sum1 == sum2 && d == 1.0);
        }
    }
    return 0;
}

char SF(const unsigned int Ds[], const unsigned int Df[])
{
    return (flush(Ds) && straight(Df));
}

char FH(const unsigned int Df[])
{
    unsigned int counter[FACES] = {0}, scounter[DRAW + 1] = {0};
    int i;
    for (i = 0; i < DRAW; i++)
        counter[Df[i]]++;
    for (i = 0; i < FACES; i++)
        scounter[counter[i]]++;
    if (scounter[3] == 1 && scounter[2] == 1)
    {
        // for (i = 0; i < DRAW; i++)
        //     printf("%d ", Df[i]);
        // printf("\n");
        return 1;
    }
    return 0;
}

void initialize(unsigned int deck[][FACES])
{
    for (int i = 0; i < SUITS; i++)
        for (int j = 0; j < FACES; j++)
            deck[i][j] = 0;
}
char check(const unsigned int Ds[], const unsigned int Df[])
{
    if (RF(Ds, Df))
    {
        // for (int i = 0; i < DRAW; i++)
        //     printf("%d ", Df[i]);
        // printf("|");
        // for (int i = 0; i < DRAW; i++)
        //     printf("%d ", Ds[i]);
        // printf("\n");
        return 1;
    }
    else if (SF(Ds, Df))
    {
        return 2;
    }
    else if (FoK(Df))
    {
        return 3;
    }
    else if (FH(Df))
    {
        return 4;
    }
    else if (flush(Ds))
    {
        return 5;
    }
    else if (straight(Df))
    {
        return 6;
    }
    else if (ToK(Df))
    {
        return 7;
    }
    else if (dpair(Df))
    {
        return 8;
    }
    else if (apair(Df))
    {
        return 9;
    }
    else
    {
        return 10;
    }
}