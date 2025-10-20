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



void displayDeliveryEstimate(char cities[][MAX_NAME], int source, int dest, int dist,
                             int weight, int vehicleType)
{
    int rate = vehicleRate[vehicleType];
    int speed = vehicleSpeed[vehicleType];
    int efficiency = vehicleEfficiency[vehicleType];

    float cost = dist * rate * (1 + (weight / 10000.0));
    float time = (float)dist / speed;
    float fuelUsed = (float)dist / efficiency;
    float fuelCost = fuelUsed * FUEL_PRICE;
    float totalCost = cost + fuelCost;
    float profit = cost * 0.25;
    float customerCharge = totalCost + profit;

    printf("\n======================================================\n");
    printf("            DELIVERY COST ESTIMATION\n");
    printf("------------------------------------------------------\n");
    printf("From: %s\n", cities[source]);
    printf("To: %s\n", cities[dest]);
    printf("Minimum Distance: %d km\n", dist);
    printf("Vehicle: %s\n", vehicleNames[vehicleType]);
    printf("Weight: %d kg\n", weight);
    printf("------------------------------------------------------\n");
    printf("Base Cost: %d x %d x (1 + %d/10000) = %.2f LKR\n",
           dist, rate, weight, cost);
    printf("Fuel Used: %.2f L\n", fuelUsed);
    printf("Fuel Cost: %.2f LKR\n", fuelCost);
    printf("Operational Cost: %.2f LKR\n", totalCost);
    printf("Profit (25%%): %.2f LKR\n", profit);
    printf("Customer Charge: %.2f LKR\n", customerCharge);
    printf("Estimated Time: %.2f hours\n", time);
    printf("======================================================\n");
}



int handleDelivery(char cities[][MAX_NAME], int cityCount, int distance[][MAX_CITIES],
                   int deliveryCount, float deliveries[][9])
{
    if (cityCount < 2)
    {
        printf("\nNeed at least 2 cities for delivery!\n");
        return deliveryCount;
    }

    if (deliveryCount >= MAX_DELIVERIES)
    {
        printf("\nMaximum delivery limit reached!\n");
        return deliveryCount;
    }

    printf("\nCities:\n");
    for (int i = 0; i < cityCount; i++)
    {
        printf("%d. %s\n", i + 1, cities[i]);
    }

    int source, dest, weight, vehicleType;
    printf("\nEnter source city number: ");
    scanf("%d", &source);
    printf("Enter destination city number: ");
    scanf("%d", &dest);

    if (source < 1 || source > cityCount || dest < 1 || dest > cityCount)
    {
        printf("\nInvalid city numbers!\n");
        return deliveryCount;
    }

    if (source == dest)
    {
        printf("\nSource and destination cannot be same!\n");
        return deliveryCount;
    }

    printf("Enter weight (kg): ");
    scanf("%d", &weight);

    printf("\nVehicle Types:\n");
    printf("1. Van (Capacity: 1000 kg)\n");
    printf("2. Truck (Capacity: 5000 kg)\n");
    printf("3. Lorry (Capacity: 10000 kg)\n");
    printf("Select vehicle (1-3): ");
    scanf("%d", &vehicleType);

    if (vehicleType < 1 || vehicleType > 3)
    {
        printf("\nInvalid vehicle type!\n");
        return deliveryCount;
    }

    vehicleType--;

    if (weight > vehicleCapacity[vehicleType])
    {
        printf("\nWeight exceeds vehicle capacity!\n");
        return deliveryCount;
    }

    int visited[MAX_CITIES] = {0};
    int minDist[1] = {INT_MAX};
    int currentPath[MAX_CITIES];
    int bestPath[MAX_CITIES];
    int bestPathLen[1] = {0};

    findShortestPath(distance, cityCount, source - 1, dest - 1, visited, 0,
                     minDist, currentPath, 0, bestPath, bestPathLen);

    if (minDist[0] == INT_MAX)
    {
        printf("\nNo route available between these cities!\n");
        return deliveryCount;
    }

    printf("\nOptimal Route: ");
    for (int i = 0; i < bestPathLen[0]; i++)
    {
        printf("%s", cities[bestPath[i]]);
        if (i < bestPathLen[0] - 1) printf(" -> ");
    }
    printf("\n");

    displayDeliveryEstimate(cities, source - 1, dest - 1, minDist[0], weight, vehicleType);

    int rate = vehicleRate[vehicleType];
    int speed = vehicleSpeed[vehicleType];
    int efficiency = vehicleEfficiency[vehicleType];

    float cost = minDist[0] * rate * (1 + (weight / 10000.0));
    float time = (float)minDist[0] / speed;
    float fuelUsed = (float)minDist[0] / efficiency;
    float fuelCost = fuelUsed * FUEL_PRICE;
    float totalCost = cost + fuelCost;
    float profit = cost * 0.25;
    float customerCharge = totalCost + profit;

    deliveries[deliveryCount][0] = source - 1;
    deliveries[deliveryCount][1] = dest - 1;
    deliveries[deliveryCount][2] = minDist[0];
    deliveries[deliveryCount][3] = weight;
    deliveries[deliveryCount][4] = vehicleType;
    deliveries[deliveryCount][5] = totalCost;
    deliveries[deliveryCount][6] = time;
    deliveries[deliveryCount][7] = profit;
    deliveries[deliveryCount][8] = customerCharge;

    deliveryCount++;
    printf("\nDelivery recorded successfully!\n");

    return deliveryCount;
}



void showReports(int deliveryCount, float deliveries[][9], char cities[][MAX_NAME])
{
    if (deliveryCount == 0)
    {
        printf("\nNo deliveries recorded yet!\n");
        return;
    }

    float totalDistance = 0;
    float totalTime = 0;
    float totalRevenue = 0;
    float totalProfit = 0;
    float maxDist = 0;
    float minDist = 999999;
    int maxIdx = 0;
    int minIdx = 0;

    for (int i = 0; i < deliveryCount; i++)
    {
        totalDistance += deliveries[i][2];
        totalTime += deliveries[i][6];
        totalRevenue += deliveries[i][8];
        totalProfit += deliveries[i][7];

        if (deliveries[i][2] > maxDist)
        {
            maxDist = deliveries[i][2];
            maxIdx = i;
        }
        if (deliveries[i][2] < minDist)
        {
            minDist = deliveries[i][2];
            minIdx = i;
        }
    }

    printf("\n========================================\n");
    printf("        PERFORMANCE REPORTS\n");
    printf("========================================\n");
    printf("Total Deliveries: %d\n", deliveryCount);
    printf("Total Distance Covered: %.2f km\n", totalDistance);
    printf("Average Delivery Time: %.2f hours\n", totalTime / deliveryCount);
    printf("Total Revenue: %.2f LKR\n", totalRevenue);
    printf("Total Profit: %.2f LKR\n", totalProfit);
    printf("----------------------------------------\n");
    printf("Longest Route: %s to %s (%.0f km)\n",
           cities[(int)deliveries[maxIdx][0]],
           cities[(int)deliveries[maxIdx][1]],
           maxDist);
    printf("Shortest Route: %s to %s (%.0f km)\n",
           cities[(int)deliveries[minIdx][0]],
           cities[(int)deliveries[minIdx][1]],
           minDist);
    printf("========================================\n");
}




void saveToFile(char cities[][MAX_NAME], int cityCount, int distance[][MAX_CITIES],
                int deliveryCount, float deliveries[][9])
{
    FILE *fp = fopen("routes.txt", "w");
    if (fp == NULL)
    {
        printf("\nError saving routes!\n");
        return;
    }

    fprintf(fp, "%d\n", cityCount);
    for (int i = 0; i < cityCount; i++)
    {
        fprintf(fp, "%s\n", cities[i]);
    }

    for (int i = 0; i < cityCount; i++)
    {
        for (int j = 0; j < cityCount; j++)
        {
            fprintf(fp, "%d ", distance[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);

    fp = fopen("deliveries.txt", "w");
    if (fp == NULL)
    {
        printf("\nError saving deliveries!\n");
        return;
    }

    fprintf(fp, "%d\n", deliveryCount);
    for (int i = 0; i < deliveryCount; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fprintf(fp, "%.2f ", deliveries[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int loadFromFile(char cities[][MAX_NAME], int cityCount, int distance[][MAX_CITIES],
                 int deliveryCount, float deliveries[][9])
{
    FILE *fp = fopen("routes.txt", "r");
    if (fp == NULL)
    {
        return 0;
    }

    fscanf(fp, "%d", &cityCount);
    for (int i = 0; i < cityCount; i++)
    {
        fscanf(fp, " %[^\n]", cities[i]);
    }

    for (int i = 0; i < cityCount; i++)
    {
        for (int j = 0; j < cityCount; j++)
        {
            fscanf(fp, "%d", &distance[i][j]);
        }
    }
    fclose(fp);

    fp = fopen("deliveries.txt", "r");
    if (fp == NULL)
    {
        return cityCount * 1000000;
    }

    fscanf(fp, "%d", &deliveryCount);
    for (int i = 0; i < deliveryCount; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fscanf(fp, "%f", &deliveries[i][j]);
        }
    }
    fclose(fp);

    printf("\nData loaded successfully!\n");
    return cityCount * 1000000 + deliveryCount;
}
