/**
    @file menu.c
    @author Maggie Lin (mclin)
    This file contains the struct for Menu and MenuItem. The file also contains 
    functions that initialize the Menu fields, free the memory used to store Menu
    and MenuItems, read a file stream and create a Menu along with MenuItems based on the file,
    and order and list the MenuItems in Menu in the correct format.
*/

#include "menu.h"

Menu *makeMenu()
{
    Menu *menu = (Menu *)malloc(sizeof(Menu));
    menu->capacity = INITIAL_CAPACITY;
    menu->count = COUNT_START;
    menu->list = (MenuItem **)malloc(menu->capacity * sizeof(MenuItem *));
    return menu;
}

void readMenuItems(char const *filename, Menu *menu)
{
    // Check to see if the file is valid
    FILE *src = fopen(filename, "r");
    if (src == NULL) {
        fprintf(stderr, "Can't open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    char *currentLine = readLine(src);
    int length = strlen(currentLine);
    int currentPosition, addPosition;
    char current[length];
    char final[length];
    int price = COUNT_START;
    while (currentLine != NULL) {
        MenuItem *menuItem = (MenuItem *)malloc(sizeof(MenuItem));
        // Check ID
        if (sscanf(currentLine, "%s%n", current, &currentPosition) != READ || strlen(current) != ID_LENGTH) {
            fprintf(stderr, "Invalid menu file: %s\n", filename);
            exit(EXIT_FAILURE);
        }
        for (int i = COUNT_START; i < strlen(current); i++) {
            if (!isdigit(current[i])) {
                fprintf(stderr, "Invalid menu file: %s\n", filename);
                exit(EXIT_FAILURE);
            }
        }
        snprintf(menuItem->id, sizeof(menuItem->id), "%s", current);

        // Check category length
        if (sscanf(currentLine + currentPosition, "%s%n", current, &addPosition) != READ || strlen(current) > CATEGORY_LENGTH) {
            fprintf(stderr, "Invalid menu file: %s\n", filename);
            exit(EXIT_FAILURE);
        }
        currentPosition = currentPosition + addPosition;
        snprintf(menuItem->category, sizeof(menuItem->category), "%s", current);

        // Check price
        if (sscanf(currentLine + currentPosition, "%d%n", &price, &addPosition) != READ || price < COUNT_START) {
            fprintf(stderr, "Invalid menu file: %s\n", filename);
            exit(EXIT_FAILURE);
        }
        currentPosition = currentPosition + addPosition;
        menuItem->cost = price;

        // Check name
        if (sscanf(currentLine + currentPosition, "%s%n", current, &addPosition) != READ) {
            fprintf(stderr, "Invalid menu file: %s\n", filename);
            exit(EXIT_FAILURE);
        } else {
            snprintf(menuItem->name, sizeof(menuItem->name), "%s", current);
            strcpy(final, menuItem->name);
            currentPosition = currentPosition + addPosition;
            strcpy(current, currentLine + currentPosition);
            strcat(final, current);
        }

        // Check name length
        if (strlen(final) > NAME_LENGTH) {
            fprintf(stderr, "Invalid menu file: %s\n", filename);
            exit(EXIT_FAILURE);
        }
        snprintf(menuItem->name, sizeof(menuItem->name), "%s", final);
        
        if (menu->count >= menu->capacity) {
            menu->capacity *= DOUBLE;
            menu->list = (MenuItem **)realloc(menu->list, menu->capacity * sizeof(MenuItem *));
        }
        menu->list[menu->count++] = menuItem;
        free(currentLine);
        currentLine = readLine(src);
    }
    free(currentLine);
    fclose(src);

    // Check for duplcate IDs
    for (int i = COUNT_START; i < menu->count; i++) {
        for (int j = i + ITEM; j < menu->count; j++) {
            if (strcmp(menu->list[i]->id, menu->list[j]->id) == EQUAL) {
                fprintf(stderr, "Invalid menu file: %s\n", filename);
                exit(EXIT_FAILURE);
            }
        }
    }
}

void listMenuItems(Menu *menu, int (*compare)(void const *va, void const *vb),
    bool (*test)(MenuItem const *item, char const *str), char const *str) {
    
    // Sort the MenuItems in the correct order
    qsort(menu->list, menu->count, sizeof(menu->list[COUNT_START]), compare);

    // Print the MenuItems
    printf("ID   Name                 Category        Cost\n");
    for (int i = COUNT_START; i < menu->count; i++) {
        if (test(menu->list[i], str)) {
            printf("%s %-20s %-15s $%3d.%02d\n", menu->list[i]->id, menu->list[i]->name,
                menu->list[i]->category, menu->list[i]->cost / DOLLAR, menu->list[i]->cost % DOLLAR);
        }
    }
    printf("\n");
}

void freeMenu(Menu *menu) {
    for (int i = COUNT_START; i < menu->count; i++) {
        free(menu->list[i]);
    }
    free(menu->list);
    free(menu);
}
