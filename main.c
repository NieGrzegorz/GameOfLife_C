#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define CLIMATEZONENUMBER 4
#define MINIMALPOPSIZE 4
#define POPULATION_TYPE_G 'G'
#define POPULATION_TYPE_F 'F'

typedef struct _Population
{
    unsigned int size;
    unsigned int numberOfIndividuals;
    char type;
    char* name;
} Population;

typedef struct _Individual
{
    char state;
    unsigned int xPosition;
    unsigned int yPosition;
    Population *population;
} Individual;

typedef struct _GameBoard
{
    Individual **board;
    unsigned int boardSize;
} GameBoard;

typedef struct _ClimateZone
{
    unsigned int startRow;
    unsigned int startCol;
    unsigned int size;
    char zoneType;
} ClimateZone;

void createBoard(GameBoard *gameBoard, unsigned int size)
{
      gameBoard->boardSize = size;
      gameBoard->board =malloc(size*sizeof(Individual*));

      for(unsigned int i = 0; i < size; ++i)
      {
          gameBoard->board[i] = malloc(size*sizeof(Individual));
      }
}

void emptyGameBoard(GameBoard *gameBoard, Population *nonePopulation)
{
    for(unsigned int i = 0; i < gameBoard->boardSize; ++i)
    {
        for(unsigned int j = 0; j < gameBoard->boardSize; ++j)
        {
           gameBoard->board[i][j].state = '.';
           gameBoard->board[i][j].xPosition = i;
           gameBoard->board[i][j].yPosition = j;
           gameBoard->board[i][j].population = nonePopulation;
        }
    }
}

Population* generatePopulation(unsigned int size, char type, char* name)
{
    Population *retValue = NULL;
    if(size < MINIMALPOPSIZE)
    {
        printf("Minimal population size is 4\n");
    }
    else
    {
        Population *newPopulation = malloc(sizeof(Population));
        newPopulation->size = size;
        newPopulation->numberOfIndividuals = (size*size)/2;
        newPopulation->type = type;
        newPopulation->name = name;

        retValue = newPopulation;
    }
    return retValue;
}

int isFreeSpace(GameBoard *gameBoard, unsigned int xPos, unsigned int yPos, unsigned int populationSize)
{
    if((xPos+populationSize <= gameBoard->boardSize) && (yPos+populationSize <= gameBoard->boardSize))
    {
        for(unsigned int i = xPos; i < (xPos+populationSize); ++i)
        {
            for(unsigned int j = yPos; j < (yPos+populationSize); ++j)
            {
                if(gameBoard->board[i][j].state != '.')
                {
                    return -1;
                }
            }
        }
    }
    else
    {
        return -1;
    }
    return 0;
}

void placePopulation(GameBoard *gameBoard, Population *population, unsigned int xPos, unsigned int yPos)
{
    int x, y;
    unsigned int i = 0;
    while( i < population->numberOfIndividuals)
    {
        x = (rand()%population->size)+ xPos;
        y = (rand()%population->size)+ yPos;

        if(gameBoard->board[x][y].state != 'A')
        {
            gameBoard->board[x][y].state ='A';
            gameBoard->board[x][y].population = population;
            ++i;
        }
    }
}

void generateClimateZone(ClimateZone *zone, unsigned int startRow, unsigned int startCol, unsigned int size, char type)
{
    zone->zoneType = type;
    zone->startCol = startCol;
    zone->startRow = startRow;
    zone->size = size;
}

char randomClimatSpotGenerator(char *generated)
{
    char retValue;
    int randIndex;
    int randomized = 0;
    while(randomized != 1)
    {
        randIndex = rand()%CLIMATEZONENUMBER;
        if(generated[randIndex] != '1')
        {
            retValue = generated[randIndex];
            generated[randIndex] = '1';
            randomized = 1;
        }
    }
    return retValue;
}

void placeClimateZones(ClimateZone **climateZones, GameBoard *gameBoard)
{
    char generated[CLIMATEZONENUMBER] = {'C','H','S','M'};
    unsigned int climateZoneSize = gameBoard->boardSize/2;
    for(unsigned int i = 0; i < CLIMATEZONENUMBER; ++i)
    {
        climateZones[i] = malloc(sizeof(ClimateZone*));
    }

    generateClimateZone(climateZones[0], 0, 0, climateZoneSize, randomClimatSpotGenerator(generated));
    generateClimateZone(climateZones[1], climateZoneSize, 0, climateZoneSize, randomClimatSpotGenerator(generated));
    generateClimateZone(climateZones[2], 0, climateZoneSize, climateZoneSize, randomClimatSpotGenerator(generated));
    generateClimateZone(climateZones[3], climateZoneSize, climateZoneSize, climateZoneSize, randomClimatSpotGenerator(generated));
}

int findSpot(GameBoard *gameBoard, Population *population)
{
    for(unsigned int i = 0; i < gameBoard->boardSize; ++i)
    {
        for(unsigned int j = 0; j < gameBoard->boardSize; ++j)
        {
            if(isFreeSpace(gameBoard, i, j, population->size) == 0)
            {
                placePopulation(gameBoard, population, i, j);
                return 0;
            }
        }
    }
    return -1;
}

int countAliveIndividuals(GameBoard *gameBoard, unsigned int xPos, unsigned int yPos, unsigned int areaSize)
{
    int aliveCount = 0;

    for(unsigned int i = xPos; i < (xPos+areaSize); ++i)
    {
        for(unsigned int j = yPos; j < (yPos+areaSize); ++j)
        {
            if(gameBoard->board[i][j].state == 'A') ++aliveCount;
        }
    }
    return aliveCount;
}

int countNeighbours(Individual *currentIndividual, GameBoard *gameBoard, char type)
{
    unsigned int x = currentIndividual->xPosition;
    unsigned int y = currentIndividual->yPosition;
    int numberOfNeighbours = 0;
    if((currentIndividual->xPosition == 0) && ((currentIndividual->yPosition != 0) && (currentIndividual->yPosition != (gameBoard->boardSize-1))))
    {
        // First row
        if((gameBoard->board[x][y+1].state == 'A') && (gameBoard->board[x][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y-1].state == 'A') && (gameBoard->board[x][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y-1].state == 'A') && (gameBoard->board[x+1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y].state == 'A') && (gameBoard->board[x+1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y+1].state == 'A') && (gameBoard->board[x+1][y+1].population->type == type)) numberOfNeighbours++;
    }
    else if(((currentIndividual->xPosition != 0) && (currentIndividual->xPosition != (gameBoard->boardSize-1))) && (currentIndividual->yPosition == 0))
    {
        if((gameBoard->board[x-1][y].state == 'A') && (gameBoard->board[x-1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y].state == 'A') && (gameBoard->board[x+1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y+1].state == 'A') && (gameBoard->board[x-1][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y+1].state == 'A') && (gameBoard->board[x][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y+1].state == 'A') && (gameBoard->board[x+1][y+1].population->type == type)) numberOfNeighbours++;
    }
    else if((currentIndividual->xPosition == (gameBoard->boardSize-1)) && ((currentIndividual->yPosition != 0) && (currentIndividual->yPosition != (gameBoard->boardSize-1))))
    {
        if((gameBoard->board[x][y-1].state == 'A') && (gameBoard->board[x][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y+1].state == 'A') && (gameBoard->board[x][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y-1].state == 'A') && (gameBoard->board[x-1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y].state == 'A') && (gameBoard->board[x-1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y+1].state == 'A') && (gameBoard->board[x-1][y+1].population->type == type)) numberOfNeighbours++;
    }
    else if(((currentIndividual->xPosition != 0) && (currentIndividual->xPosition != (gameBoard->boardSize-1))) && (currentIndividual->yPosition == (gameBoard->boardSize-1)))
    {
        if((gameBoard->board[x-1][y].state == 'A') && (gameBoard->board[x-1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y].state == 'A') && (gameBoard->board[x+1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y-1].state == 'A') && (gameBoard->board[x-1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y-1].state == 'A') && (gameBoard->board[x][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y-1].state == 'A') && (gameBoard->board[x+1][y-1].population->type == type)) numberOfNeighbours++;
    }
    else if((currentIndividual->xPosition == (gameBoard->boardSize-1)) && (currentIndividual->yPosition == (gameBoard->boardSize-1)))
    {
        if((gameBoard->board[x-1][y].state == 'A') && (gameBoard->board[x-1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y-1].state == 'A') && (gameBoard->board[x-1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y-1].state == 'A') && (gameBoard->board[x][y-1].population->type == type)) numberOfNeighbours++;
    }
    else if((currentIndividual->xPosition == 0) && (currentIndividual->yPosition == 0))
    {
        if((gameBoard->board[x+1][y].state == 'A') && (gameBoard->board[x+1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y+1].state == 'A') && (gameBoard->board[x+1][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y+1].state == 'A') && (gameBoard->board[x][y+1].population->type == type)) numberOfNeighbours++;
    }
    else if((currentIndividual->xPosition == 0) && (currentIndividual->yPosition == (gameBoard->boardSize-1)))
    {
        if((gameBoard->board[x+1][y].state == 'A') && (gameBoard->board[x+1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y-1].state == 'A') && (gameBoard->board[x+1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y-1].state == 'A') && (gameBoard->board[x][y-1].population->type == type)) numberOfNeighbours++;
    }
    else if((currentIndividual->xPosition == (gameBoard->boardSize-1)) && (currentIndividual->yPosition == 0))
    {
        if((gameBoard->board[x][y+1].state == 'A') && (gameBoard->board[x][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y+1].state == 'A') && (gameBoard->board[x-1][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y].state == 'A') && (gameBoard->board[x-1][y].population->type == type)) numberOfNeighbours++;
    }
    else
    {
        if((gameBoard->board[x-1][y-1].state == 'A')&& (gameBoard->board[x-1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y].state == 'A') && (gameBoard->board[x-1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y+1].state == 'A') && (gameBoard->board[x-1][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y-1].state == 'A') && (gameBoard->board[x][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y+1].state == 'A') && (gameBoard->board[x][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y-1].state == 'A') && (gameBoard->board[x+1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y].state == 'A') && (gameBoard->board[x+1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y+1].state == 'A') && (gameBoard->board[x+1][y+1].population->type == type)) numberOfNeighbours++;
    }

    return numberOfNeighbours;
}

void printBoard(GameBoard *gameBoard)
{
    printf("\n");
    printf("-----------------------------------------\n");
    for(unsigned int i = 0; i < gameBoard->boardSize; ++i)
    {
        for(unsigned int j = 0; j < gameBoard->boardSize; ++j)
        {
           printf("%c ",gameBoard->board[i][j].state);
        }
        printf("\n");
    }
}

void evolve(Individual *currentIndividual, GameBoard *gameBoard, Population *typeGPopulation, Population *typeFPopulation, Population *typeNonePopulation)
{
    char currentType, otherType;
    unsigned int sameTypeNeighbours, differentTypeNeighbours, numberOfNeighbours;

    if(currentIndividual->population->type == 'G')
    {
        currentType = 'G';
        otherType = 'F';
    }
    else
    {
        currentType = 'F';
        otherType = 'G';
    }

    if(currentIndividual->population->type == 'G')
    {
        sameTypeNeighbours = countNeighbours(currentIndividual, gameBoard, 'G');
        differentTypeNeighbours = countNeighbours(currentIndividual, gameBoard, 'F');
    }
    else
    {
        differentTypeNeighbours = countNeighbours(currentIndividual, gameBoard, 'G');
        sameTypeNeighbours = countNeighbours(currentIndividual, gameBoard, 'F');
    }
    numberOfNeighbours = sameTypeNeighbours + differentTypeNeighbours;

    if((currentIndividual->state == '.') && (numberOfNeighbours == 3))
    {
        if(sameTypeNeighbours > differentTypeNeighbours)
        {
            currentIndividual->state = 'A';
            if(currentType == 'G')
            {
                currentIndividual->population = typeGPopulation;
                typeGPopulation->numberOfIndividuals++;
            }
            else
            {
                currentIndividual->population = typeFPopulation;
                typeFPopulation->numberOfIndividuals++;
            }
        }
    }
    else if((currentIndividual->state == 'A') && ((numberOfNeighbours < 2) || (numberOfNeighbours > 3)))
    {
        currentIndividual->state = '.';
        currentIndividual->population->numberOfIndividuals--;
        currentIndividual->population = typeNonePopulation;
    }
}

void nextGeneration(GameBoard *gameBoard, Population *typeGPopulation, Population *typeFPopulation, Population *typeNonePopulation)
{
    Individual *temp;
    for(unsigned int i = 0; i < gameBoard->boardSize; ++i)
    {
        for(unsigned int j = 0; j < gameBoard->boardSize; ++j)
        {
            temp = &gameBoard->board[i][j];
            evolve(temp, gameBoard, typeGPopulation, typeFPopulation, typeNonePopulation);
        }
    }
}

void executeClimateImpact(GameBoard *gameBoard, ClimateZone **climateZones, Population *noneTypePopulation, Population *typeGPopulation)
{
    for(unsigned int i = 0; i < CLIMATEZONENUMBER; ++i )
    {
        if((climateZones[i]->zoneType == 'C') && (countAliveIndividuals(gameBoard, climateZones[i]->startRow, climateZones[i]->startCol, climateZones[i]->size) >= 2))
        {
            int killStreak = 0;
            while(killStreak < 2)
            {
                int x, y;
                x = (rand()%climateZones[i]->size)+climateZones[i]->startRow;
                y = (rand()%climateZones[i]->size)+climateZones[i]->startCol;

                if(gameBoard->board[x][y].state == 'A')
                {
                    gameBoard->board[x][y].state = '.';
                    gameBoard->board[x][y].population = noneTypePopulation;
                    ++killStreak;
                }
            }
        }
        else if((climateZones[i]->zoneType == 'H')&&(countAliveIndividuals(gameBoard, climateZones[i]->startRow, climateZones[i]->startCol, climateZones[i]->size) >= 1))
        {
            int killStreak = 0;
            while(killStreak < 1)
            {
                int x, y;
                x = (rand()%climateZones[i]->size)+climateZones[i]->startRow;
                y = (rand()%climateZones[i]->size)+climateZones[i]->startCol;

                if(gameBoard->board[x][y].state == 'A')
                {
                    gameBoard->board[x][y].state = '.';
                    gameBoard->board[x][y].population = noneTypePopulation;
                    ++killStreak;
                }
            }
        }
        else if(climateZones[i]->zoneType == 'M')
        {
            int addCount = 0;
            while(addCount < 2)
            {
                int x, y;
                x = (rand()%climateZones[i]->size)+climateZones[i]->startRow;
                y = (rand()%climateZones[i]->size)+climateZones[i]->startCol;

                if(gameBoard->board[x][y].state == '.')
                {
                    gameBoard->board[x][y].state = 'A';
                    gameBoard->board[x][y].population = typeGPopulation;
                    gameBoard->board[x][y].population->numberOfIndividuals++;
                    ++addCount;
                }
            }
        }
        else if(climateZones[i]->zoneType == 'S')
        {
            int addCount = 0;
            while(addCount < 1)
            {
                int x, y;
                x = (rand()%climateZones[i]->size)+climateZones[i]->startRow;
                y = (rand()%climateZones[i]->size)+climateZones[i]->startCol;

                if(gameBoard->board[x][y].state == '.')
                {
                    gameBoard->board[x][y].state = 'A';
                    gameBoard->board[x][y].population = typeGPopulation;
                    gameBoard->board[x][y].population->numberOfIndividuals++;
                    ++addCount;
                }
            }
        }
    }
}

int main()
{
    char key = 'A';
    unsigned int numberOfGenerations = 0;
    srand(time(NULL));
    unsigned int boardSize = 15;
    unsigned int populationSize = 7;
    ClimateZone **climateZones = malloc(CLIMATEZONENUMBER*sizeof(ClimateZone*));


    Population *no1, *no2, *no3;
    no1 = generatePopulation(populationSize, 'G', "No1");
    no2 = generatePopulation(populationSize, 'F', "No2");
    no3 = generatePopulation(populationSize, 'N', "No3");

    GameBoard *board = malloc(sizeof(GameBoard));
    createBoard(board, boardSize);
    emptyGameBoard(board, no3);
    placeClimateZones(climateZones, board);
    printBoard(board);

    findSpot(board, no1);
    findSpot(board, no2);
    printBoard(board);

    while(key != 'q')
    {
        ++numberOfGenerations;
        if((numberOfGenerations%5) == 0)
        {
            executeClimateImpact(board, climateZones, no3, no1);
        }
        nextGeneration(board, no1, no2, no3);
        printBoard(board);
        printf("Press Enter to continue/Enter q to finish: ");
        key = getchar();
        printf("\n");
    }
    printf("-------------SUMMARY-------------\n");
    printf("Individuals in Population type G: %d\n", no1->numberOfIndividuals);
    printf("Individuals in Population type F: %d\n", no2->numberOfIndividuals);
    printf("Number of generations: %d\n", numberOfGenerations);
    return 0;
}
