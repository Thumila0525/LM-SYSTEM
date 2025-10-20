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
