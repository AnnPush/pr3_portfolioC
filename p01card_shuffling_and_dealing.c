#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUITS 4
#define FACES 13
#define CARDS 52
#define SIZE 5

//тасовать карты
void shuffle(unsigned int wDeck[][FACES]);
//раздать карты игрокам
void deal(unsigned int wDeck[][FACES], unsigned int wPlayer[][2], unsigned int *wStart,  unsigned int *wFinish);
//распечатать карты игрока
void printArray(unsigned int wPlayer[][2], const char *wFace[], const char *wSuit[], unsigned int pd);
//определение приоритета карт игроков
int fPriority(unsigned int wPlayer[][2], unsigned int *nominal);
//сортировка карт, которые находятся на руках у каждого игрока
void sort(unsigned int wPlayer[][2]);
//функция, обменивающая карты в ячейках
void swap(unsigned int * element1Ptr, unsigned int * element2Ptr);
//сколько еще взять карт
void takeCards(unsigned int *prioritet, unsigned int *takeCard);
//передвинуть карты с комбинацией в конец массива
void sortTakeCards(unsigned int wPlayer[][2], unsigned int *prioritet, unsigned int *takeCard, unsigned int *nominal);
//оценка карт игроков
void mark(unsigned int wPlayer1[][2], unsigned int wPlayer2[][2], unsigned int *nominal1, unsigned int *nominal2, unsigned int *p1, unsigned int *p2);

int main(void)
{   
    unsigned int deck[SUITS][FACES] = {0};//инициализировать массив колоды карт

	unsigned int player1[5][2];//карты на руках у первого игрока
	unsigned int player2[5][2];//карты на руках у второго игрока
	
	unsigned int pl_dl = 0;//если дилер - 1, если игрок - 0  
	
	unsigned int nominal1;//сумма номиналов карт первого игрока
    unsigned int nominal2;//сумма номиналов карт второго игрока
	
	unsigned int start = 1;//с какой карты начинается раздача карт
	unsigned int finish = 5; //какая карта последняя при раздаче карт
	
	unsigned int p1, p2;//приоритет 1 и 2 игрока
	
	srand(time(NULL)); 
    shuffle(deck);//перетасовать карты

    // инициализация массива мастей
    const char *suit[SUITS] = {"Hearts", "Diamonds", "Clubs", "Spades"};
	
	// инициализация массива номиналов(значений)
    const char *face[FACES] = {"Ace", "Deuce", "Three", "Four",
                                "Five", "Six", "Seven", "Eight",
                                "Nine", "Ten", "Jack", "Queen", "King"};
	
    //сдать карты первому игроку
    printf("\nDEALER_________________________\n\n");
	deal(deck, player1, &start, &finish); 
	printArray(player1, face, suit, 1);
	//printf("\nDEALER_sort\n\n");
	
	//сортировка массива карт дилера
	sort(player1);
	//printArray(player1, face, suit, 0);
	
	//сдать карты второму игроку 
	printf("\nPLAYER\n\n");
	finish = 5;
	deal(deck, player2, &start, &finish);
	printArray(player2, face, suit, 0);
	
	//сортировка массива карт игрока
	printf("\nPLAYER_sort\n\n");
	sort(player2);
	printArray(player2, face, suit, 0);
	
	//определить приоритет карт дилера
	p1 = fPriority(player1,&nominal1);
	//printf("\nPriority_1 = %u\n\n", p1);
	
	//автоматически определить, сколько карт возьмет дилер
    takeCards(&p1, &finish);
	//printf("\nHand1_sort\n\n");
	sortTakeCards(player1, &p1, &finish, &nominal1);
	//printArray(player1, face, suit, 0);
    printf("\nDEALER\n\n");
	printf("\nTake %u cards!\n\n",finish);
	
	//раздать новые карты дилеру
	if(finish != 0)
	{
		deal(deck, player1,  &start, &finish);	 
		//printf("\nNew_array_1\n");
		//printArray(player1, face, suit, 0);
		//printf("\nHand1_sort_new\n\n");
		
		//сортировка новых карт дилера
	    sort(player1);
	    //printArray(player1, face, suit, 0);
	}
	
	//опрределить приоритет новых карт дилера
	p1 = fPriority(player1, &nominal1);
	//printf("\nPriority_1_new = %u\n\n", p1);
	
	
    //приоритет карт игрока
	printf("\nPLAYER-------------------\n\n");
	p2 = fPriority(player2,  &nominal2);
	printf("\nPriority_2 = %u\n\n", p2);
	
			
	//сколько карт желает взять игрок
	printf("\nHow many cards do you want to take?\n\n");
	scanf("%u", &finish);
	printf("\nPLAYER_sort_new\n\n");
			//выпавшие комбинации карт передвинуть в конец массива
	sortTakeCards(player2, &p2, &finish, &nominal2);
	printArray(player2, face, suit, 0);
	if(finish != 0)
	{
		//выдать новые карты игроку
		deal(deck, player2, &start, &finish);
		printf("\nPLAYER_New_array_2\n");
		printArray(player2, face, suit, 0);
		//сортировать новые карты
		printf("\nPLAYER_sort_new\n\n");
	    sort(player2);
	    printArray(player2, face, suit, 0);
	}
	//определить приоритет карт игрока
	p2 = fPriority(player2,  &nominal2);
	printf("\nPriority_2_new = %u\n\n", p2);

    //распечатать карты, которые на руках у играющих
	printf("\nMark\n\n");
	printf("\nDEALER\n\n");
	printArray(player1, face, suit, 0);
	printf("\nPLAYER\n\n");
	printArray(player2, face, suit, 0);
	//оценить карты игроков
	mark(player1, player2,&nominal1, &nominal2, &p1, &p2);		
}

void mark(unsigned int wPlayer1[][2], unsigned int wPlayer2[][2], unsigned int *nominal1, unsigned int *nominal2, unsigned int *p1, unsigned int *p2)
{//печать результата оценивания карт
int  printRating(unsigned int*, unsigned int*);
    int flag = 0;
	
	if(!printRating(p1, p2))//сравнить комбинации карт
	{
	    if(!printRating(nominal1, nominal2))//если комбинации равны, сравнить номиналы
		{
		    for(size_t i = 5 ; i > 0; --i)
			{
				if(!printRating(&wPlayer1[i][1], &wPlayer2[i][1]))//если номиналы равны, сравнение идет по сортированному массиву по старшей карте
		        {
					flag++;
				}
				else
				{
					break;
				}
				if( flag == 5)
				{
                    printf("\nCARD_DEALER = CARD_PLAYER\n");//карты игроков равны
			    }
			}
		}
	} 
}

int printRating(unsigned  int* ptr1, unsigned  int* ptr2)
{
	if(*ptr1 > *ptr2)
	{
		printf("\nDEALER is the better hand\n");
		return 1;
	}
	 else if(*ptr1 < *ptr2)
	{
        printf("\nPLAYER is the better hand\n");
		return 1;
	}
    return 0;
}

void takeCards(unsigned int *prioritet, unsigned int *takeCard)
{
	
	unsigned int take_card = 0;//счетчик карт, которые составляют комбинацию
	
    if( *prioritet < 7 && *prioritet > 3)//если выпало 4 одинаковые карты флеш, стрит
	{
		take_card = 0;//берем 0 карт
	}
	else if(*prioritet == 0 || *prioritet == 1)//если нет комбинаций или выпала одна пара
	{
		take_card = 3;//взять 3 карты
	}
	else if(*prioritet == 3)//если выпало 3 карты
	{
		take_card = 2;//берем 2 карты
	}
	else if(*prioritet == 2)//если выпало 2 карты
	{
		take_card = 1;//берем 1 карту
	} 
	*takeCard = take_card;//запоминаем количество карт, которые будут взяты
   
    
}
void sortTakeCards(unsigned int wPlayer[][2], unsigned int *prioritet, unsigned int *takeCard, unsigned int *nominal)
{
	unsigned int wSortPlayer[5][2] = {0};//временный массив
	unsigned int obmen = 0;//счетчик для карт, которые будут обмениваться
	unsigned int take_card = *takeCard;//счетчик карт, которые составляют комбинацию
	//передвинуть выпавшие комбинации карт в конец массива
    if(*prioritet == 2 )
	{
		if(wPlayer[3][1] != wPlayer[4][1])
		{
			swap(&wPlayer[0][1], &wPlayer[4][1]);
			swap(&wPlayer[0][0], &wPlayer[4][0]);
			swap(&wPlayer[1][1], &wPlayer[3][1]);
			swap(&wPlayer[1][0], &wPlayer[3][0]);
		}
		else if( wPlayer[2][1] != wPlayer[1][1])
		{
			swap(&wPlayer[0][1], &wPlayer[2][1]);
			swap(&wPlayer[0][0], &wPlayer[2][0]);
		}
	}
	else if(*prioritet != 2 && *prioritet < 4)//все варианты, кроме флеш, 4 карты, стрит
    {
       for( size_t i = 0; i < 5; ++i)
	   {  
			if(wPlayer[i][1] != *nominal )// элемент массива не равен номиналу комбинации
		    {
				//карта отправляется в начало массива
			    wSortPlayer[obmen][1] = wPlayer[i][1];
				wSortPlayer[obmen][0] = wPlayer[i][0];
			    obmen++;
		    }
			else 
		    { 
				wSortPlayer[take_card][1] = wPlayer[i][1];
				wSortPlayer[take_card][0] = wPlayer[i][0];
				++take_card;
		    }
	   }
	   //перезаписать из временного массива карты
	   for( size_t i = 0; i < 5; ++i)
	   {
	        wPlayer[i][1]=wSortPlayer[i][1];
	        wPlayer[i][0]=wSortPlayer[i][0];
	   }
	}
}
//распечатать карты игрока
void printArray(unsigned int wPlayer[][2], const char *wFace[], const char *wSuit[], unsigned int pd)
{
	for( size_t i = 0; i < 5; ++i)
    {		
	     if(pd == 0)
	     {
		     printf("%5s of %-8s\n ", wFace[ wPlayer[i][1]],  wSuit[ wPlayer[i][0]] );
	     }
	     else
	     {
             printf("%s\n","***");
	     }
    }
    printf("\n ");
}


void  sort(unsigned int wPlayer[][2])
{
	for( size_t i = 0; i < 4; ++i)
    {
		for( size_t j = 0; j < 4; ++j)
        {
			if(wPlayer[j][1] > wPlayer[j+1][1])
			{
				swap(&wPlayer[j][1], &wPlayer[j+1][1]);
			    swap(&wPlayer[j][0], &wPlayer[j+1][0]);//обмен элементов
			}			
		}
	}
}
//функция обменивающая карты в ячейках, на которые показывают указатели
void swap(unsigned int * element1Ptr, unsigned int * element2Ptr)
{
	unsigned int temp = * element1Ptr;
    * element1Ptr = * element2Ptr;
    * element2Ptr = temp;
}


int fPriority(unsigned int wPlayer[][2],  unsigned  int *nominal)
{
	int oneOfPair(unsigned int wPlayer[][2],  unsigned int *nominal);
    int twoOfPair(unsigned int wPlayer[][2],  unsigned int *nominal);
    int treeOfCard(unsigned int wPlayer[][2],  unsigned int *nominal);
    int fourOfCard(unsigned int wPlayer[][2],  unsigned int *nominal);
    int flashOfCard(unsigned int wPlayer[][2],  unsigned int *nominal);
    int straighOfCard(unsigned int wPlayer[][2], unsigned int *nominal);

    int (*ptr_f[6])(unsigned int [][2], unsigned int *) = {oneOfPair, twoOfPair, treeOfCard, straighOfCard, flashOfCard, fourOfCard};

    unsigned int priority;
	
	for(int i = 0; i < 6; ++i)
	{
		priority =(*ptr_f[i])(wPlayer, nominal);
		if(priority > 0)
		{
			return priority;
		}
	}
	return 0;
}

void shuffle(unsigned int wDeck[][FACES])
{
    for (size_t card = 1; card <= CARDS; ++card)
	{
        size_t row;
        size_t column;

        do 
		{
            row = rand() % SUITS;
            column = rand() % FACES;
        } while(wDeck[row][column] != 0); 

        wDeck[row][column] = card;
    } 
} 
// сдать карты
void deal(unsigned int wDeck[][FACES], unsigned int wPlayer[][2],  unsigned int *wStart,  unsigned int *wFinish)
{
	 unsigned int r = 0;//счетчик определяющий номинал и масть карт
	 *wFinish += *wStart;//определить конечную карту при раздаче
     
	 // сдать карты в указанных пределах
     for (size_t card = *wStart; card < *wFinish; ++card)
	 {
		// цикл по строкам
         for (size_t row = 0; row < SUITS; ++row)
		 {
			 // цикл по столбцам в текущей строке
             for (size_t column = 0; column < FACES; ++column)
		     {
                 if (wDeck[row][column] == card) 
				 {
					 wPlayer[r][0] = row;//запомнить масть карты, полученной игроком
					 wPlayer[r][1] = column;//запомнить номинал карты, полученной игроком
	                 //printf("%5s of %-8s\t \n", wFace[column], wSuit[row]);
					 ++r;
                 }
             }
         }
     }
	 *wStart = *wFinish;
}

int oneOfPair(unsigned int wPlayer[][2], unsigned int *nominal)
{
     unsigned int counter[FACES]={0};
     
     int  i;
	 size_t n = 0;//количество одинаковых карт на руках у игрока
     
     for( i = 0; i < 5; ++i)
     {
         ++counter[wPlayer[i][1]];
     }
     
     for( i = 0; i < FACES; ++i)
     {
         if(counter[i] == 2 )
         {
             ++n;
			 if(n == 1)
				*nominal = i;
         }
     }
     if(n == 1)
     {
		 
		return 1;
	 }
	 return 0;
}

int twoOfPair(unsigned int wPlayer[][2], unsigned int *nominal)
{
    unsigned int counter[FACES]={0};
    *nominal = 0;
	 
    size_t i;
    size_t n = 0;//количество одинаковых карт на руках у игрока
	  
    for( i = 0; i < 5; ++i)
    {
        ++counter[wPlayer[i][1]];
    }
     
    for( i = 0; i < FACES; ++i)
    {
	    if(counter[i] == 2 )
        {  
           ++n;
           *nominal += i;
		   
		}		 	
    } 
    if(n == 2)
	{
	    return 2;
	}
	return 0;
}

int treeOfCard(unsigned int wPlayer[][2], unsigned int *nominal)
{
    unsigned int counter[FACES]={0};
     
    size_t i;
     
    for( i = 0; i < 5; ++i)
    {
        ++counter[wPlayer[i][1]];
    }
     
    for( i = 0; i < FACES; ++i)
    {
		if(counter[i] == 3 )
        {
			*nominal = i;
			
			return 3;
		}
    }
    return 0;	 
}

int fourOfCard(unsigned int wPlayer[][2],  unsigned int *nominal)
{
    unsigned int counter[FACES]={0};
     
    size_t i;
     
    for( i = 0; i < 5; ++i)
    {
        ++counter[wPlayer[i][1]];
    }
     
    for( i = 0; i < FACES; ++i)
    {
		if(counter[i] == 4 )
        {
			*nominal = i;
			
			return 6;
		}
    } 
    return 0;	 
}

int flashOfCard(unsigned int wPlayer[][2], unsigned int *nominal)
{
    unsigned int counter[SUITS]={0};
    size_t i;
     
    for( i = 0; i < 5; ++i)
    {
        ++counter[wPlayer[i][0]];
		*nominal += wPlayer[i][1];//вычисляем сумму номиналов карт одной масти
    }
    for( i = 0; i < FACES; ++i)
    {
		if(counter[i] == 5 )
        {
			
			return 5;
		}
    } 
    return 0;	 
}

int straighOfCard(unsigned int wPlayer[][2],  unsigned int *nominal)
{ 
    size_t i;
     
    for( i = 0; i < 5; ++i)
    {
		*nominal += wPlayer[i][1];
    }
     
	if(wPlayer[4][1] - 1 == wPlayer[3][1] && wPlayer[3][1] - 1 == wPlayer[2][1] && wPlayer[2][1] - 1 == wPlayer[1][1] && wPlayer[1][1] - 1 == wPlayer[0][1])
	{
		
		return 4;
	}	
    return 0;	
}
