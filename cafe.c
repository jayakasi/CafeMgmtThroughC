#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 

struct Menu {
    int nCount;
    char itemName[64];
    double price;
};

struct Menu availableItems[] = {
    {0, "Americano", 60.50},
    {0, "Cappuccino", 50.0},
    {0, "Latte", 80.0},
    {0, "Hot chocolate", 120.0},
    {0, "Mocha", 110.0},
    {0, "Iris Coffee", 70.0},
    {0, "Cold Coffee", 80.0},
    {0, "Irid Delight", 100.0},
    {0, "Mocha Shake", 80.0},
    {0, "Rich Coffee", 80.0},
    {0, "Brownie Shake", 160.0},
    {0, "Oreo Shake", 180.0}
};

#define HYPHEN_COUNT 96
#define MAX_ORDERABLE_ITEMS 100
#define MAX_HOW_MANY_PER_CHOICE 30

int orderedIndex = -1;
struct Menu orderedItems[MAX_ORDERABLE_ITEMS];

int getAvailableItemsCount() {
    return sizeof(availableItems) / sizeof(struct Menu);
}

double getIgstPercent(double price) {
    return price * 0.12;
}

double getCgstPercent(double price) {
    return price * 0.12;
}

double getGstPrice(double price) {
    return price + getIgstPercent(price) + getCgstPercent(price);
}

void printHyphenLine() {
    for (int i = 0; i < HYPHEN_COUNT; i++) {
        printf("-");
    }
    printf("\n");
}

void displayMenu() {
    int size = getAvailableItemsCount();
    printHyphenLine();
    printf("\t\tHotel Menucard\n");
    printHyphenLine();
    printf("%-10s\t%-30s\t%s\n", "ItemNumber", "ItemName", "ItemPrice");
    printHyphenLine();
    for (int index = 0; index < size; index++) {
        printf("%-10d\t%-30s\t%0.2f\n", (index + 1), availableItems[index].itemName, availableItems[index].price);
    }
    printHyphenLine();
}

void addOrder(int nChoice, int nCount) {
    if ((orderedIndex + 1) >= 0 && (orderedIndex + 1) < MAX_ORDERABLE_ITEMS) {
        orderedIndex++;
        orderedItems[orderedIndex].nCount = nCount;
        strcpy(orderedItems[orderedIndex].itemName, availableItems[nChoice].itemName);
        orderedItems[orderedIndex].price = availableItems[nChoice].price;
    }
}

void printOrderSummary() {
    int nItemsTotal = 0;
    double gstTotal = 0.0;
    double nTotalPrice = 0.0;
    printf("\n");
    printf("Here is Summary of Your Order\n");
    printHyphenLine();
    printf("%-30s\t\t%-8s\t%-8s\t\t%s\n", "Item", "Count", "GSTPrice", "Total");
    printHyphenLine();
    for (int index = 0; index <= orderedIndex; index++) {
        nItemsTotal += orderedItems[index].nCount;
        double currentGSTPrice = getGstPrice(orderedItems[index].price);
        double nGstPrice = currentGSTPrice * orderedItems[index].nCount;
        gstTotal += currentGSTPrice;
        nTotalPrice += nGstPrice;
        printf("%-30s\t\t%-8d\t%0.2f Rs\t\t%0.2f Rs\n",
            orderedItems[index].itemName, orderedItems[index].nCount, currentGSTPrice, nGstPrice);
    }
    printHyphenLine();
    printf("\n%-30s\t\t\t\t\t\t\t%0.2f Rs\n", "Grand Total", nTotalPrice);
    printHyphenLine();
    printf("\n");
}

int takeOrder() {
    printf("\nYour Choice:");
    int nChoice = -1;
    char sChoice[128];
    fgets(sChoice, 128, stdin);
    nChoice = atoi(sChoice);
    nChoice--;
    if (nChoice >= 0 && nChoice < getAvailableItemsCount()) {
        int nHowMany = 0;
        printf("\nHow Many:");
        char sHowMany[128];
        fgets(sHowMany, 128, stdin);
        nHowMany = atoi(sHowMany);
        if (nHowMany > 0 && nHowMany <= MAX_HOW_MANY_PER_CHOICE) {
            printf("%d\t\t%s\t\t%d\n", nChoice + 1, availableItems[nChoice].itemName, nHowMany);
            addOrder(nChoice, nHowMany);
            return 0;
        } else {
            printf("\n%d many items not available for order\n", nHowMany);
        }
    }
    printf("Invalid Choice, please try again\n");
    displayMenu();
    return takeOrder();
}

int main() {
    int nContinue = 0;
    do {
        displayMenu();
        int result = takeOrder();
        if (result == 0) {
            printf("\nDo you want to order more items? y/n:");
            char sContinue[128];
            fgets(sContinue, 128, stdin);
            nContinue = sContinue[0];
        }
    } while (nContinue == 'y' || nContinue == 'Y');
    printOrderSummary();
    return 0;
}

