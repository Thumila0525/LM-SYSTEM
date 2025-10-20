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
