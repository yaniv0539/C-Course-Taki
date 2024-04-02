// 206593444
// Yaniv Zamir

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NONE -1
#define MAX_FIRST_NAME_LEN 20
#define START_NUM_OF_CARDS 4
#define PULL_CARD 0
#define PLUS 10
#define STOP 11
#define REVERSE 12
#define COLOR 13
#define TAKI 14
#define YELLOW 1
#define RED 2
#define BLUE 3
#define GREEN 4
#define CARD_WIDTH 9
#define CARD_HEIGHT 6
#define NUM_OF_CARDS 14

typedef struct Card
{
	char color;
	int num;
} CARD;

typedef struct Player
{
	char firstName[MAX_FIRST_NAME_LEN];
	CARD* deck;
	int deckLogSize;
	int deckPycSize;
} PLAYER;

typedef struct ArrayItem
{
	int cardNum;
	int counter;
} ArrayItem;

PLAYER* readPlayers(int* pNumOfPlayers);
void initDeck(PLAYER* players, int numOfPlayers);
CARD initUpperCard();
void randCard(CARD* cards);
void randInitCards(CARD* cards);
char typeColor(int cardColor);
void printCard(CARD* cards);
void changeCardIntToString(int sCard, char cardName[]);
void printStringInCard(char string[]);
void printPlayerDeck(PLAYER* players);
int* playGame(PLAYER* players, CARD* pUpperCard, int* pNumOfPlayers);
void removeCard(CARD* pCard, PLAYER* pPlayer);
bool IsValidNumber(CARD* pCardsList, int chosenCard, CARD* pUpperCard);
bool IsValidColor(CARD* pCardsList, int chosenCard, CARD* pUpperCard);
bool IsChangeColor(CARD* pCardsList, int chosenCard, CARD* pUpperCard);
bool IsValidCard(CARD* pCardsList, int chosenCard, CARD* pUpperCard);
bool IsExistCard(int chosenCard, int cardsLogSize);
void enterNewCard(PLAYER* pPlayer, int* statsArray);
int getNextPlayerNumber(int curPlayerNumber, int totalNumPlayers, bool fwd, bool stop);
void statistics(int* statsArray);
void indexMergeSort(ArrayItem arr[], int size);
void indexMerge(ArrayItem a1[], int n1, ArrayItem a2[], int n2, ArrayItem res[]);
void copyIndicesArr(ArrayItem dest[], ArrayItem src[], int size);
void freeData(PLAYER* players, int numOfPlayers);

void main()
{
	srand(time(NULL));
	int numOfPlayers;
	PLAYER* players = NULL;
	CARD upperCard;
	int* statsArray = NULL;

	printf("************  Welcome to TAKI game !!! ***********\n");

	players = readPlayers(&numOfPlayers);
	
	upperCard = initUpperCard();

	initDeck(players, numOfPlayers);

	statsArray = playGame(players, &upperCard, &numOfPlayers);
	
	freeData(players, numOfPlayers);

	statistics(statsArray);
}

//This function prompts the user to enter the number of players,
// then allocates memory to store that number of players and prompts the user to enter the first name of each player.
// The function returns a pointer to the array of players.
PLAYER* readPlayers(int* pNumOfPlayers)
{
	int i;
	PLAYER* players;

	printf("Please enter the number of players:\n");
	scanf("%d", pNumOfPlayers);
	
	players = (PLAYER*)malloc(*pNumOfPlayers * sizeof(PLAYER));
	
	if (!players)
		exit(1);
	
	for (i = 0; i < *pNumOfPlayers; i++)
	{
		printf("Please enter the first name of player #%d:\n", i + 1);
		scanf("%s", players[i].firstName);
	}

	return players;
}

//This function takes in an array of players and the number of players, and allocates memory for each player's deck of cards,
// initializes the size of the deck, and calls a function to randomly initialize the cards in the deck.
void initDeck(PLAYER* players, int numOfPlayers)
{
	int i;

	for (i = 0; i < numOfPlayers; i++)
	{
		players[i].deck = (CARD*)malloc(START_NUM_OF_CARDS * sizeof(CARD));

		if (!players[i].deck)
			exit(1);

		players[i].deckLogSize = START_NUM_OF_CARDS;
		players[i].deckPycSize = START_NUM_OF_CARDS;

		randInitCards(players[i].deck);
	}
}

//This function generates and returns a randomly generated card to be used as the "upper card" in the game.
CARD initUpperCard()
{
	CARD upperCard;
	int cardType = (rand() % 9) + 1;
	upperCard.num = cardType;

	int cardColor = (rand() % 4) + 1;
	upperCard.color = typeColor(cardColor);

	return upperCard;
}

//This function takes in an array of cards and initializes each card with a randomly generated number and color.
void randInitCards(CARD* card)
{
	int i;

	for (i = 0; i < START_NUM_OF_CARDS; i++)
		randCard(card + i);
}

//This function takes in a single card and initializes it with a randomly generated number and color.
void randCard(CARD* card)
{
	int cardType = (rand() % 14) + 1;

	card->num = cardType;

	if (cardType == COLOR)
	{
		card->color = ' ';
		return;
	}

	int cardColor = (rand() % 4) + 1;

	card->color = typeColor(cardColor);
}

// This function takes in an integer representing a card color and returns the corresponding character for that color.
char typeColor(int cardColor)
{
	char color = ' ';
	switch (cardColor)
	{
		case RED:
		{
			color = 'R';
			break;
		}
		case BLUE:
		{
			color = 'B';
			break;
		}
		case GREEN:
		{
			color = 'G';
			break;
		}
		case YELLOW:
		{
			color = 'Y';
			break;
		}
		default:
		{
			printf("Huston we have a problem!");
			break;
		}
	}
	return color;
}

// This function takes in a pointer to a card and prints out a visual representation of the card to the console.
void printCard(CARD* card)
{
	int i, j;
	int midHeight = CARD_HEIGHT / 2;
	int midWidth = CARD_WIDTH / 2;
	char specialCard[6];
	for (i = 0; i < CARD_HEIGHT; i++)
	{
		if (i == 0 || i == CARD_HEIGHT - 1)
			for (j = 0; j < CARD_WIDTH; j++)
				printf("*");
		
		else if (i == midHeight - 1)
		{
			if (card->num < 10)
			{
				for (j = 0; j < CARD_WIDTH; j++)
				{
					if (j == 0 || j == CARD_WIDTH - 1)
						printf("*");
					else if (j == midWidth)
						printf("%d", card->num);
					else
						printf(" ");
				}
			}
			else
			{
				changeCardIntToString(card->num, specialCard);
				printStringInCard(specialCard);
			}
		}

		else if (i == midHeight)
		{
			for (j = 0; j < CARD_WIDTH; j++)
			{
				if (j == 0 || j == CARD_WIDTH - 1)
					printf("*");
				else if (j == midWidth)
					printf("%c", card->color);
				else
					printf(" ");
			}
		}
		
		else
		{
			for (j = 0; j < CARD_WIDTH; j++)
			{
				if (j == 0 || j == CARD_WIDTH - 1)
					printf("*");
				else
					printf(" ");
			}
		}
		printf("\n");
	}
}

// This function takes an integer argument, sCard and a character array, cardName as arguments.
// It converts the integer value of a card to a string representation of the card.
// It uses a switch statement to map the integer value to the corresponding string for the card.
void changeCardIntToString(int sCard, char cardName[])
{
	switch (sCard)
	{
		case PLUS:
		{
			strcpy(cardName, "+");
			break;
		}
		case STOP:
		{
			strcpy(cardName, "STOP");
			break;
		}
		case REVERSE:
		{
			strcpy(cardName, "<->");
			break;
		}
		case COLOR:
		{
			strcpy(cardName, "COLOR");
			break;
		}
		case TAKI:
		{
			strcpy(cardName, "TAKI");
			break;
		}
		default:
			break;
	}
}

// This function takes a character array, string as an argument and prints the string in the center of a card.
// It finds the starting point of the string in the card by calculating the center of the card,
// and the ending point of the string by adding the length of the string to the starting point.
// It then uses for loops to print the card with the string in the center.
void printStringInCard(char string[])
{
	size_t i;
	size_t len = strlen(string);
	size_t startWrite = (CARD_WIDTH / 2) - (len / 2);
	size_t endWrite = startWrite + len;

	printf("*");

	for (i = 1; i < startWrite; i++)
		printf(" ");

	printf("%s", string);

	for (i = endWrite; i < CARD_WIDTH - 1; i++)
		printf(" ");

	printf("*");
}

// This function takes a pointer to a PLAYER struct as an argument, and prints the cards in the player's deck to the console.
// It uses a for loop to iterate through the player's deck, and calls the printCard function to print each card.
void printPlayerDeck(PLAYER* player)
{
	for (int i = 0; i < player->deckLogSize; i++)
	{
		printf("\nCard #%d\n", i + 1);
		printCard(player->deck + i);
	}
}

// This function takes a pointer to a PLAYER struct and a pointer to an integer array as arguments.
// It randomly generates a new card using the randCard function, and adds it to the player's deck.
// If the player's deck is full, the function uses the realloc function to resize the deck to accommodate the new card.
// It also updates the statistics array to keep track of the number of cards of each type that have been played.
void enterNewCard(PLAYER* pPlayer, int* statsArray)
{
	CARD newCard;
	randCard(&newCard);

	if (pPlayer->deckLogSize >= pPlayer->deckPycSize && pPlayer->deck)
	{
		CARD* p = (CARD*)realloc(pPlayer->deck, (pPlayer->deckLogSize * 2) * sizeof(CARD));
		if (!p)
			exit(1);

		pPlayer->deckPycSize = pPlayer->deckLogSize * 2;
		pPlayer->deck = p;
	}

	pPlayer->deck[pPlayer->deckLogSize] = newCard;
	pPlayer->deckLogSize++;
	
	statsArray[newCard.num - 1]++;
}

// This function takes pointers to an array of PLAYER structs, a pointer to a CARD struct, and a pointer to an integer as arguments.
// It simulates a game of Taki by allowing players to take turns and make various moves according to the rules of the game.
// It keeps track of the statistics of the game, such as the number of cards of each type that have been played,
// and returns a pointer to an array of integers containing the statistics of the game.
int* playGame(PLAYER* players, CARD* pUpperCard, int* pNumOfPlayers)
{
	int playerNum = 0;
	int chosenColor;
	bool validChoice;
	bool forwardDir = true;
	bool endGame = false;
	bool openTAKI = false;
	int selectedAction = NONE;
	int cardIndex = 0;
	int* statsArray = NULL;

	CARD selectedCard = *pUpperCard;

	statsArray = (int*)malloc(NUM_OF_CARDS * sizeof(int));

	if (!statsArray)
		exit(1);

	for (int i = 0; i < NUM_OF_CARDS; i++)
		statsArray[i] = 0;

	while (!endGame)
	{
		printf("\nUpper card:\n");
		printCard(&selectedCard);

		printf("\n%s's turn:\n", players[playerNum].firstName);

		printPlayerDeck(players + playerNum);

		validChoice = false;
		while (!validChoice)
		{
			if (openTAKI)
				printf("\nPlease enter 0 if you want to finish your turn\n"
					"or 1-%d if you want to put one of your cards in the middle:\n", players[playerNum].deckLogSize);
			else
				printf("\nPlease enter 0 if you want to take a card from the deck\n"
					"or 1-%d if you want to put one of your cards in the middle:\n", players[playerNum].deckLogSize);

			scanf("%d", &cardIndex);

			validChoice = IsExistCard(cardIndex, players[playerNum].deckLogSize) &&
				IsValidCard(players[playerNum].deck, cardIndex, &selectedCard);

			if (!validChoice)
			{
				printf("Invalid card! Try again.");
				continue;
			}

			if (cardIndex != 0)
			{
				selectedCard = players[playerNum].deck[cardIndex - 1];
				switch (selectedCard.num)
				{
					case PLUS:
						selectedAction = PLUS;
						break;
					case STOP:
						selectedAction = STOP;
						break;
					case REVERSE:
						selectedAction = REVERSE;
						break;
					case COLOR:
						selectedAction = COLOR;
						break;
					case TAKI:
						selectedAction = TAKI;
						break;
					default:
						selectedAction = NONE;
						break;
				}
			}
			else 
			{
				if (openTAKI)
					openTAKI = false;
				else
					selectedAction = PULL_CARD;
			}
		} // end while validChoice

		switch (selectedAction)
		{
			case PLUS:
				removeCard(&players[playerNum].deck[cardIndex - 1], &players[playerNum]);
				if (players[playerNum].deckLogSize == 0)
				{
					enterNewCard(&players[playerNum], statsArray);
					playerNum = getNextPlayerNumber(playerNum, *pNumOfPlayers, forwardDir, false);
				}
			break;

			case STOP:
				removeCard(&players[playerNum].deck[cardIndex - 1], &players[playerNum]);
				if (*pNumOfPlayers != 2 && players[playerNum].deckLogSize == 0)
					endGame = true;
				else if (players[playerNum].deckLogSize == 0)
				{
					enterNewCard(&players[playerNum], statsArray);
					playerNum = getNextPlayerNumber(playerNum, *pNumOfPlayers, forwardDir, false);
				}
				else if (!openTAKI)
					playerNum = getNextPlayerNumber(playerNum, *pNumOfPlayers, forwardDir, true);
			break;

			case REVERSE:
				removeCard(&players[playerNum].deck[cardIndex - 1], &players[playerNum]);
				if (players[playerNum].deckLogSize == 0)
					endGame = true;
				else if (!openTAKI)
				{
					forwardDir = !forwardDir;
					playerNum = getNextPlayerNumber(playerNum, *pNumOfPlayers, forwardDir, false);
				}
			break;

			case COLOR:
				removeCard(&players[playerNum].deck[cardIndex - 1], &players[playerNum]);
				printf("Please enter your color choice:\n"
					"1 - Yellow\n"
					"2 - Red\n"
					"3 - Blue\n"
					"4 - Green\n");
				scanf("%d", &chosenColor);
				selectedCard.color = typeColor(chosenColor);
				openTAKI = false;
				playerNum = getNextPlayerNumber(playerNum, *pNumOfPlayers, forwardDir, false);
				if (players[playerNum].deckLogSize == 0)
					endGame = true;
			break;
			
			case TAKI:
				removeCard(&players[playerNum].deck[cardIndex - 1], &players[playerNum]);
				if (players[playerNum].deckLogSize == 0)
					endGame = true;
				else if (cardIndex == 0)
					playerNum = getNextPlayerNumber(playerNum, *pNumOfPlayers, forwardDir, false);
				else if (!openTAKI)
					openTAKI = true;
			break;

			case PULL_CARD:
				enterNewCard(&players[playerNum], statsArray);
				playerNum = getNextPlayerNumber(playerNum, *pNumOfPlayers, forwardDir, false);
			break;
			
			case NONE:
				removeCard(&players[playerNum].deck[cardIndex - 1], &players[playerNum]);
				if (players[playerNum].deckLogSize == 0)
					endGame = true;
				else if (!openTAKI)
					playerNum = getNextPlayerNumber(playerNum, *pNumOfPlayers, forwardDir, false);
			break;
		} // end switch selected action
	} // while endGame

	printf("The winner is... %s! Congratulations!\n", players[playerNum].firstName);

	return statsArray;
}

// This function takes in the current player number, total number of players, a boolean for the direction of play (forward or backward),
// and a boolean for whether to "stop" the game. It calculates the next player number based on the input values and returns it.
int getNextPlayerNumber(int curPlayerNumber, int totalNumPlayers, bool fwd, bool stop)
{
	int step = 1;

	if (totalNumPlayers == 0)
		return 0;
	if (stop)
		step = 2;
	if (fwd)
		curPlayerNumber = (curPlayerNumber + step) % totalNumPlayers;
	else
	{
		curPlayerNumber = (curPlayerNumber - step);
		if (curPlayerNumber < 0)
			curPlayerNumber += totalNumPlayers;
	}

	return curPlayerNumber;
}

// This function takes in a pointer to a list of cards, the chosen card, and the upper card.
// It checks if the number of the chosen card is the same as the number of the upper card, and returns a boolean indicating whether it is valid or not.
bool IsValidNumber(CARD* pCardsList, int chosenCard, CARD* pUpperCard)
{
	if (pCardsList[chosenCard - 1].num == pUpperCard->num)
		return true;

	return false;
}

// This function takes in a pointer to a list of cards, the chosen card, and the upper card.
// It checks if the color of the chosen card is the same as the color of the upper card, and returns a boolean indicating whether it is valid or not.
bool IsValidColor(CARD* pCardsList, int chosenCard, CARD* pUpperCard)
{
	if (pCardsList[chosenCard - 1].color == pUpperCard->color)
		return true;

	return false;
}

// This function takes in a pointer to a list of cards, the chosen card, and the upper card.
// It checks if the chosen card is the "change color" card, and returns a boolean indicating whether it is valid or not.
bool IsChangeColor(CARD* pCardsList, int chosenCard, CARD* pUpperCard)
{
	if (pCardsList[chosenCard - 1].num == COLOR)
		return true;

	return false;
}

// This function takes in a pointer to a list of cards, the chosen card, and the upper card,
// and checks if the chosen card is a valid card to play, by calling the previous three functions.
bool IsValidCard(CARD* pCardsList, int chosenCard, CARD* pUpperCard)
{
	if (chosenCard == PULL_CARD ||
		IsValidNumber(pCardsList, chosenCard, pUpperCard) ||
		IsValidColor(pCardsList, chosenCard, pUpperCard) ||
		IsChangeColor(pCardsList, chosenCard, pUpperCard))
		return true;
	
	return false;
}

// This function takes in the chosen card and the size of the cards list and returns a boolean indicating whether the chosen card exists in the list or not.
bool IsExistCard(int chosenCard, int cardsLogSize)
{
	if (chosenCard > cardsLogSize || chosenCard < 0)
		return false;
	return true;
}

// This function takes in a pointer to a card and a pointer to a player, and removes the card from the player's deck.
void removeCard(CARD* pCard, PLAYER* pPlayer)
{
	CARD* p = pPlayer->deck;

	for (int i = 0; i < pPlayer->deckLogSize; i++, p++)
	{
		if (p == pCard)
		{
			for (int j = i; j < pPlayer->deckLogSize - 1; j++)
			{
				*p = *(p + 1);
				p++;
			}
			pPlayer->deckLogSize--;
			return;
		}
	}
}

// This function takes in a pointer to an array of integers and prints the game statistics, which are the frequency of each card that pulled out by a player in the game.
// It sorts the array and then prints the results.
void statistics(int* statsArray)
{
	ArrayItem arr[14];

	printf("\n************ Game Statistics ************\n"
		"Card # | Frequency\n"
		"__________________\n");
	for (int i = 0; i < NUM_OF_CARDS; i++)
	{
		arr[i].cardNum = i;
		arr[i].counter = statsArray[i];
	}
	free(statsArray);
	indexMergeSort(arr, NUM_OF_CARDS);
	
	for (int i = NUM_OF_CARDS - 1; i >= 0; i--)
	{
		if (arr[i].cardNum + 1 > 9)
		{
			switch (arr[i].cardNum + 1)
			{
			case PLUS:
				printf("   +   |");
				break;
			case STOP:
				printf(" STOP  |");
				break;
			case REVERSE:
				printf("  <->  |");
				break;
			case COLOR:
				printf(" COLOR |");
				break;
			case TAKI:
				printf(" TAKI  |");
				break;
			default:
				break;
			}
		}
		else
			printf("   %d   |", arr[i].cardNum + 1);

		printf("    %d\n", arr[i].counter);
	}
}

void indexMergeSort(ArrayItem arr[], int size)
{
	ArrayItem* tmpArr = NULL;

	if (size <= 1)
		return;

	indexMergeSort(arr, size / 2);
	indexMergeSort(arr + size / 2, size - size / 2);

	tmpArr = (ArrayItem*)malloc(size * sizeof(ArrayItem));
	if (tmpArr)
	{
		indexMerge(arr, size / 2, arr + size / 2, size - size / 2, tmpArr);
		copyIndicesArr(arr, tmpArr, size);
		free(tmpArr);
	}
	else
		exit(1);
}

void indexMerge(ArrayItem a1[], int n1, ArrayItem a2[], int n2, ArrayItem res[])
{
	int ind1, ind2, resInd;
	ind1 = ind2 = resInd = 0;

	while ((ind1 < n1) && (ind2 < n2)) {
		if (a1[ind1].counter <= a2[ind2].counter) {
			res[resInd] = a1[ind1];
			ind1++;
		}
		else {
			res[resInd] = a2[ind2];
			ind2++;
		}
		resInd++;
	}

	while (ind1 < n1) {
		res[resInd] = a1[ind1];
		ind1++;
		resInd++;
	}
	while (ind2 < n2) {
		res[resInd] = a2[ind2];
		ind2++;
		resInd++;
	}
}

void copyIndicesArr(ArrayItem dest[], ArrayItem src[], int size)
{
	int i;

	for (i = 0; i < size; i++)
		dest[i] = src[i];
}

//This function frees all the remain allocated data in the program
void freeData(PLAYER* players, int numOfPlayers)
{
	for (int i = 0; i < numOfPlayers; i++)
	{
		free(players[i].deck);
	}
	free(players);
}