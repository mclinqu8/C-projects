/**
    @file menu.h
    @author Maggie Lin (mclin)
    This header contains the struct for Menu and MenuItem. The header file also contains 
    function prototypes that initialize the Menu fields, free the memory used to store Menu
    and MenuItems, read a file stream and create a Menu along with MenuItems based on the file,
    and order and list the MenuItems in Menu in the correct format.
*/

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "input.h"

/** The length of a MenuItem ID. */
#define ID_LENGTH 4

/** The length of a MenuItem name. */
#define NAME_LENGTH 20

/** The length of a MenuItem category. */
#define CATEGORY_LENGTH 15

/** The length to store a null terminator. */
#define NULL_TERMINATE 1

/** The count number before any incrementation. */
#define COUNT_START 0

/** The number to represent valid string got read. */
#define READ 1

/** If the two items being compared are equal. */
#define EQUAL 0

/** The item index before or after the current. */
#define ITEM 1

/** The number to get dollar notation from cents. */
#define DOLLAR 100

/**
    Struct for mItem, typedef to MenuItem, which represent individual items that can be in a Menu.
    A MenuItem contains the properties of ID, name, category, and cost.
*/
struct mItem {
    char id[ID_LENGTH + NULL_TERMINATE];
    char name[NAME_LENGTH + NULL_TERMINATE];
    char category[CATEGORY_LENGTH + NULL_TERMINATE];
    int cost;
};

/**
    Typedef that renames the struct mItem to MenuItem.
*/
typedef struct mItem MenuItem;

/**
    Struct for mList, typedef to Menu, which represent a Menu.
    A Menu contains the properties of capacity, count, and a list which contains MenuItems.
*/
struct mList {
    int capacity;
    int count;
    MenuItem **list;
};

/**
    Typedef that renames the struct mList to Menu.
*/
typedef struct mList Menu;

/**
    Dynamically allocate storage for a Menu and initializes its fields.
    @return a pointer to the new Menu.
*/
Menu *makeMenu();

/**
    Frees the memory used to store the given Menu. This includes freeing space
    for all the MenuItems, freeing the resizable array of pointers, and freeing
    space for the Menu struct.
    @param menu the Menu struc to free all allocated memory.
*/
void freeMenu(Menu *menu);

/**
    Reads a file which contains menu items, make instance of the MenuItem struct for each
    item and store them in the given Menu.
    @param filename the input file name to read from.
    @param menu the Menu with MenuItems read from the file with the given file name.
*/
void readMenuItems(char const *filename, Menu *menu);

/**
    Takes the given Menu and sort the MenuItems it contains in the correct order and print
    them in the correct format.
    @param menu the Menu with MenuItems to sort and print.
    @param compare the function that helps sort the MenuItems in the correct order.
    @param test the function to check if the MenuItem should be printed.
    @param str the string for which category of MenuItems should be printed.
*/
void listMenuItems(Menu *menu, int (*compare)(void const *va, void const *vb),
    bool (*test)(MenuItem const *item, char const *str), char const *str);
