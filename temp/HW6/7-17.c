#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUITS 4
#define FACES 13
#define CARDS 52
#define DRAW 5

void shuffle(unsigned int wDeck[][FACES]); // shuffling modifies wDeck
void deal(unsigned int wDeck[][FACES], const char *wFace[],
          const char *wSuit[], unsigned int num,
          unsigned int Ds[], unsigned int Df[]); // dealing doesn't modify the arrays
int comp(const void *a, const void *b);
char apair(const unsigned int Df[]);
char dpair(const unsigned int Df[]);
char ToK(const unsigned int Df[]);
char FoK(const unsigned int Df[]);
char flush(const unsigned int Ds[]);
char straight(const unsigned int Df[]);

int main(void)
{
    unsigned int Ds[DRAW] = {0}, Df[DRAW] = {0};

    // initialize deck array
    unsigned int deck[SUITS][FACES] = {0};

    srand(time(NULL));
    shuffle(deck);

    // initialize suit array
    const char *suit[SUITS] = {"Hearts", "Diamonds", "Clubs", "Spades"};

    // initialize face array
    const char *face[FACES] =
        {"Ace", "Deuce", "Three", "Four",
         "Five", "Six", "Seven", "Eight",
         "Nine", "Ten", "Jack", "Queen", "King"};

    printf("Hand:\n");
    deal(deck, face, suit, DRAW, Ds, Df);
    printf("\n");

    printf("The hand %scontains a pair.\n", apair(Df) == 0 ? "doesn't " : "");
    printf("The hand %scontains two pairs.\n", dpair(Df) == 0 ? "doesn't " : "");
    printf("The hand %scontains three of a kind.\n", ToK(Df) == 0 ? "doesn't " : "");
    printf("The hand %scontains four of a kind.\n", FoK(Df) == 0 ? "doesn't " : "");
    printf("The hand %scontains a flush.\n", flush(Ds) == 0 ? "doesn't " : "");
    printf("The hand %scontains a straight.\n", straight(Df) == 0 ? "doesn't " : "");
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
void deal(unsigned int wDeck[][FACES], const char *wFace[],
          const char *wSuit[], unsigned int num,
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
                // if slot contains current card, display card
                if (wDeck[row][column] == card)
                {
                    Ds[card - 1] = row;
                    Df[card - 1] = column;
                    printf("%5s of %-8s%c", wFace[column], wSuit[row],
                           card % 2 == 0 ? '\n' : '\t'); // 2-column format
                }
            }
        }
    }
    if (num % 2 != 0)
        printf("\n");
    qsort(Df, num, sizeof(*(Df)), comp);
}

int comp(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

char apair(const unsigned int Df[])
{
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
        return 1;
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
    double sum1 = 0, sum2 = 0;
    for (int i = 0; i < DRAW; i++)
        sum1 += Df[i];
    sum2 = 0.5 * DRAW * (Df[0] + Df[DRAW - 1]);
    return (sum1 == sum2 && (Df[1] - Df[0]) == 1);
}