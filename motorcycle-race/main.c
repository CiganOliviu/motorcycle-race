#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBER_OF_MOTORCYCLES 1000

typedef struct
{
    char Brand[30];
    float MaxSpeedInStraightLine;
    float MaxSpeedInCurve;
} MotorcycleFeatures;

FILE * OpenFile(const char * FileName)
{
    FILE * File = fopen(FileName, "r");

    return File;
}

FILE * ValidateFile(const char * FileName)
{
    FILE * File = OpenFile(FileName);

    if (!File)
    {
        printf("File not found.");
        exit(5);
    }

    return File;
}

int GetNumberOfMotorcycles(FILE * File)
{
    int NumberOfMotorcycles = 0;

    char Line[50];

    while (fgets(Line, 50, File))
        NumberOfMotorcycles += 1;

    return NumberOfMotorcycles;
}

void GetPositionInStruct(MotorcycleFeatures * Motorcycles, int Index, int Position, char * Word)
{
    if (Position == 1)
        strcpy(Motorcycles[Index].Brand, Word);

    if (Position == 2)
        Motorcycles[Index].MaxSpeedInStraightLine = (float)strtod(Word, NULL);

    if (Position == 3)
        Motorcycles[Index].MaxSpeedInCurve = (float)strtod(Word, NULL);
}

MotorcycleFeatures * GetMotorcycles(FILE * File)
{
    MotorcycleFeatures * Motorcycles =
            (MotorcycleFeatures *)malloc(MAX_NUMBER_OF_MOTORCYCLES * sizeof(MotorcycleFeatures));

    char Line[50];
    int Index = 0;

    while (fgets(Line, 50, File))
    {

        int Position = 1;
        char * Word = strtok(Line, " ");

        while (Word != NULL)
        {
            GetPositionInStruct(Motorcycles, Index, Position, Word);

            Position += 1;
            Word = strtok(NULL, " ");
        }

        Index += 1;
    }

    return Motorcycles;
}

char * GetCircuit(FILE * CircuitFile)
{
    char * Circuit = (char *)malloc(50 * sizeof(char));
    fgets(Circuit, 50, CircuitFile);

    return Circuit;
}

int CountStraightPortionsOfCircuit(char * Circuit)
{
    int StraightPortions = 0;

    for (int i = 0; i < strlen(Circuit); ++i)
        if (Circuit[i] == '_')
            StraightPortions += 1;

    return StraightPortions;
}

int CountCurvedPortionsOfCircuit(char * Circuit)
{
    int CurvedPortions = 0;

    for (int i = 0; i < strlen(Circuit); ++i)
        if (Circuit[i] == 's')
            CurvedPortions += 1;

    return CurvedPortions;
}

float GetAverageSpeedOfMotorcycle(char * Circuit, MotorcycleFeatures Motorcycle)
{
    int StraightPortions = CountStraightPortionsOfCircuit(Circuit);
    int CurvedPortions = CountCurvedPortionsOfCircuit(Circuit);
    int SumOfPortions = StraightPortions + CurvedPortions;

    float AverageSpeedOfMotorcycle = ((StraightPortions * Motorcycle.MaxSpeedInStraightLine) +
            (CurvedPortions * Motorcycle.MaxSpeedInCurve)) / SumOfPortions;

    return AverageSpeedOfMotorcycle;
}

void SwapMotorcycles(MotorcycleFeatures * FirstMotorcycle, MotorcycleFeatures * SecondMotorcycle)
{
    MotorcycleFeatures Auxiliary;

    Auxiliary = *FirstMotorcycle;
    *FirstMotorcycle = *SecondMotorcycle;
    *SecondMotorcycle = Auxiliary;
}

void SortMotorcycles(MotorcycleFeatures * Motorcycles, int NumberOfMotorcycles, char * Circuit)
{
    for (int i = 0; i < NumberOfMotorcycles; ++i)
        for (int j = i + 1; j < NumberOfMotorcycles; ++j)
        {
            float SpeedOfFirstMotorcycle = GetAverageSpeedOfMotorcycle(Circuit, Motorcycles[i]);
            float SpeedOfSecondMotorcycle = GetAverageSpeedOfMotorcycle(Circuit, Motorcycles[j]);

            if (SpeedOfFirstMotorcycle < SpeedOfSecondMotorcycle)
                SwapMotorcycles(&Motorcycles[i], &Motorcycles[j]);
        }
}

void DisplayMotorcycles(MotorcycleFeatures * Motorcycles, int NumberOfMotorcycles)
{
    for (int i = 0; i < NumberOfMotorcycles; ++i)
        printf("%s %0.2f %0.2f \n", Motorcycles[i].Brand, Motorcycles[i].MaxSpeedInStraightLine,
               Motorcycles[i].MaxSpeedInCurve);

    printf("\n");
}

void PrepareCompetition(MotorcycleFeatures ** Motorcycles, int * NumberOfMotorcycles)
{
    FILE * MotorcyclesFile = ValidateFile("Motorcycles.txt");

    *NumberOfMotorcycles = GetNumberOfMotorcycles(MotorcyclesFile);

    fseek(MotorcyclesFile, 0, SEEK_SET);

    *Motorcycles = GetMotorcycles(MotorcyclesFile);
    DisplayMotorcycles(*Motorcycles, *NumberOfMotorcycles);

    fclose(MotorcyclesFile);
}

char * EstablishesTheCircuit()
{
    FILE * CircuitFile = ValidateFile("Circuit.txt");

    char * Circuit = GetCircuit(CircuitFile);

    fclose(CircuitFile);

    return Circuit;
}

void EstablishesTheRanking(MotorcycleFeatures * Motorcycles, int NumberOfMotorcycles, char * Circuit)
{
    SortMotorcycles(Motorcycles, NumberOfMotorcycles, Circuit);
    DisplayMotorcycles(Motorcycles, NumberOfMotorcycles);
}

void EstablishesTheWinner(MotorcycleFeatures * Motorcycles)
{
    printf("The winner is : %s.", Motorcycles[0].Brand);
}

int main()
{
    FILE * MotorcyclesFile = ValidateFile("Motorcycles.txt");

    int NumberOfMotorcycles;
    MotorcycleFeatures * Motorcycles =
            (MotorcycleFeatures *)malloc(MAX_NUMBER_OF_MOTORCYCLES * sizeof(MotorcycleFeatures));

    PrepareCompetition(&Motorcycles, &NumberOfMotorcycles);

    char * Circuit = EstablishesTheCircuit();

    EstablishesTheRanking(Motorcycles, NumberOfMotorcycles, Circuit);

    EstablishesTheWinner(Motorcycles);

    return 0;
}

