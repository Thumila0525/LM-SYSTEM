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


void displayMenu()
{
    printf("\n========================================\n");
    printf("           MAIN MENU\n");
    printf("========================================\n");
    printf("1. Manage Cities\n");
    printf("2. Manage Distances\n");
    printf("3. View Distance Table\n");
    printf("4. New Delivery Request\n");
    printf("5. View Reports\n");
    printf("6. Save Data\n");
    printf("0. Exit\n");
    printf("========================================\n");
}

int addCity(char cities[][MAX_NAME], int cityCount)
{
    if (cityCount >= MAX_CITIES)
    {
        printf("\nMaximum city limit reached!\n");
        return cityCount;
    }

    char newCity[MAX_NAME];
    printf("Enter city name: ");
    scanf(" %s", newCity);


    for (int i = 0; i < cityCount; i++)
    {
        if (strcmp(cities[i], newCity) == 0)
        {
            printf("\nCity already exists!\n");
            return cityCount;
        }
    }

    strcpy(cities[cityCount], newCity);
    cityCount++;
    printf("\nCity '%s' added successfully!\n", newCity);
    return cityCount;
}

int manageCities(char cities[][MAX_NAME], int cityCount)
{
    int choice;
    printf("\n--- City Management ---\n");
    printf("1. Add City\n");
    printf("2. Rename City\n");
    printf("3. Remove City\n");
    printf("4. List All Cities\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        return addCity(cities, cityCount);
    }
    else if (choice == 2)
    {
        if (cityCount == 0)
        {
            printf("\nNo cities available!\n");
            return cityCount;
        }
        printf("\nCities:\n");
        for (int i = 0; i < cityCount; i++)
        {
            printf("%d. %s\n", i + 1, cities[i]);
        }
        int idx;
        printf("Enter city number to rename: ");
        scanf("%d", &idx);
        if (idx < 1 || idx > cityCount)
        {
            printf("\nInvalid city number!\n");
            return cityCount;
        }
        char newName[MAX_NAME];
        printf("Enter new name: ");
        scanf(" %s", newName);
        strcpy(cities[idx - 1], newName);
        printf("\nCity renamed successfully!\n");
    }
    else if (choice == 3)
    {
        if (cityCount == 0)
        {
            printf("\nNo cities available!\n");
            return cityCount;
        }
        printf("\nCities:\n");
        for (int i = 0; i < cityCount; i++)
        {
            printf("%d. %s\n", i + 1, cities[i]);
        }
        int idx;
        printf("Enter city number to remove: ");
        scanf("%d", &idx);
        if (idx < 1 || idx > cityCount)
        {
            printf("\nInvalid city number!\n");
            return cityCount;
        }
        for (int i = idx - 1; i < cityCount - 1; i++)
        {
            strcpy(cities[i], cities[i + 1]);
        }
        cityCount--;
        printf("\nCity removed successfully!\n");
        return cityCount;
    }
    else if (choice == 4)
    {
        if (cityCount == 0)
        {
            printf("\nNo cities available!\n");
            return cityCount;
        }
        printf("\nAll Cities:\n");
        for (int i = 0; i < cityCount; i++)
        {
            printf("%d. %s\n", i + 1, cities[i]);
        }
    }
    return cityCount;
}



void manageDistances(char cities[][MAX_NAME], int cityCount, int distance[][MAX_CITIES])
{
    if (cityCount < 2)
    {
        printf("\nNeed at least 2 cities to set distances!\n");
        return;
    }

    printf("\nCities:\n");
    for (int i = 0; i < cityCount; i++)
    {
        printf("%d. %s\n", i + 1, cities[i]);
    }

    int city1, city2, dist;
    printf("Enter first city number: ");
    scanf("%d", &city1);
    printf("Enter second city number: ");
    scanf("%d", &city2);

    if (city1 < 1 || city1 > cityCount || city2 < 1 || city2 > cityCount)
    {
        printf("\nInvalid city numbers!\n");
        return;
    }

    if (city1 == city2)
    {
        printf("\nCannot set distance from city to itself!\n");
        return;
    }

    printf("Enter distance (km): ");
    scanf("%d", &dist);

    if (dist < 0)
    {
        printf("\nDistance cannot be negative!\n");
        return;
    }

    distance[city1 - 1][city2 - 1] = dist;
    distance[city2 - 1][city1 - 1] = dist;

    printf("\nDistance set: %s <-> %s = %d km\n", cities[city1 - 1], cities[city2 - 1], dist);
}


void displayDistanceTable(char cities[][MAX_NAME], int cityCount, int distance[][MAX_CITIES])
{
    if (cityCount == 0)
    {
        printf("\nNo cities available!\n");
        return;
    }

    printf("\n%-15s", "City");
    for (int i = 0; i < cityCount; i++)
    {
        printf("%-10s", cities[i]);
    }
    printf("\n");

    for (int i = 0; i < cityCount; i++)
    {
        printf("%-15s", cities[i]);
        for (int j = 0; j < cityCount; j++)
        {
            printf("%-10d", distance[i][j]);
        }
        printf("\n");
    }
}

void findShortestPath(int distance[][MAX_CITIES], int cityCount, int source, int dest,
                      int visited[], int currentDist, int minDist[], int currentPath[],
                      int pathLen, int bestPath[], int bestPathLen[])
{

    visited[source] = 1;
    currentPath[pathLen] = source;
    pathLen++;


    if (source == dest)
    {
        if (currentDist < minDist[0])
        {
            minDist[0] = currentDist;
            bestPathLen[0] = pathLen;
            for (int i = 0; i < pathLen; i++)
            {
                bestPath[i] = currentPath[i];
            }
        }
    }
    else
    {

        for (int i = 0; i < cityCount; i++)
        {
            if (visited[i] == 0 && distance[source][i] > 0)
            {
                findShortestPath(distance, cityCount, i, dest, visited,
                                 currentDist + distance[source][i], minDist,
                                 currentPath, pathLen, bestPath, bestPathLen);
            }
        }
    }

    visited[source] = 0;
}
