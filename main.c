#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_CITIES 30
#define MAX_DELIVERIES 50
#define MAX_NAME 50
#define FUEL_PRICE 310.0

// Global arrays
const int vehicleCapacity[] = {1000, 5000, 10000};
const int vehicleRate[] = {30, 40, 80};
const int vehicleSpeed[] = {60, 50, 45};
const int vehicleEfficiency[] = {12, 6, 4};
const char vehicleNames[][10] = {"Van", "Truck", "Lorry"};

// Function declarations
void displayMenu();
int addCity(char cities[][MAX_NAME], int cityCount);
int manageCities(char cities[][MAX_NAME], int cityCount);
void manageDistances(char cities[][MAX_NAME], int cityCount, int distance[][MAX_CITIES]);
void displayDistanceTable(char cities[][MAX_NAME], int cityCount, int distance[][MAX_CITIES]);
int handleDelivery(char cities[][MAX_NAME], int cityCount, int distance[][MAX_CITIES],int deliveryCount, float deliveries[][9]);
void displayDeliveryEstimate(char cities[][MAX_NAME], int source, int dest, int dist,int weight, int vehicleType);
void findShortestPath(int distance[][MAX_CITIES], int cityCount, int source, int dest,int visited[], int currentDist, int minDist[], int currentPath[],int pathLen, int bestPath[], int bestPathLen[]);
void showReports(int deliveryCount, float deliveries[][9], char cities[][MAX_NAME]);
void saveToFile(char cities[][MAX_NAME], int cityCount, int distance[][MAX_CITIES],int deliveryCount, float deliveries[][9]);
int loadFromFile(char cities[][MAX_NAME], int cityCount, int distance[][MAX_CITIES],int deliveryCount, float deliveries[][9]);

int main()
{
    char cities[MAX_CITIES][MAX_NAME];
    int distance[MAX_CITIES][MAX_CITIES];
    float deliveries[MAX_DELIVERIES][9];
    int cityCount = 0;
    int deliveryCount = 0;
    int choice;

    for (int i = 0; i < MAX_CITIES; i++)
    {
        for (int j = 0; j < MAX_CITIES; j++)
        {
            distance[i][j] = 0;
        }
    }
    for (int i = 0; i < MAX_DELIVERIES; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            deliveries[i][j] = 0;
        }
    }

    int loaded = loadFromFile(cities, cityCount, distance, deliveryCount, deliveries);
    if (loaded > 0)
    {
        cityCount = loaded / 1000000;
        deliveryCount = loaded % 1000000;
    }

    printf("\n*** LOGISTICS MANAGEMENT SYSTEM ***\n");

    while (1)
    {
        displayMenu();
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            cityCount = manageCities(cities, cityCount);
            break;
        case 2:
            manageDistances(cities, cityCount, distance);
            break;
        case 3:
            displayDistanceTable(cities, cityCount, distance);
            break;
        case 4:
            deliveryCount = handleDelivery(cities, cityCount, distance, deliveryCount, deliveries);
            break;
        case 5:
            showReports(deliveryCount, deliveries, cities);
            break;
        case 6:
            saveToFile(cities, cityCount, distance, deliveryCount, deliveries);
            printf("\nData saved successfully!\n");
            break;
        case 0:
            saveToFile(cities, cityCount, distance, deliveryCount, deliveries);
            printf("\nExiting... Data saved.\n");
            return 0;
        default:
            printf("\nInvalid choice! Try again.\n");
        }
    }

    return 0;
}
