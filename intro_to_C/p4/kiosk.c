/**
    @file kiosk.c
    @author Maggie Lin (mclin)
    This file contains the struct for Order and OrderItem. The file also contains
    functions that initialize the Order fields, free the memory used to store Order
    and OrderItems, compare MenuItems and OrderItems to help order them,
    and order and list the OrderItems in Order in the correct format. Finally, this file 
    contains the main method that reads menu files and user commands and respond
    with the appropriate response for the commands based on whether they are valid or 
    invalid command. The valid command allowed by the main methods are "list menu",
    "list category", "list order", "add", "remove", and "quit."
*/

#include "input.h"
#include "menu.h"

/** The item goes before the item it is compared against. */
#define BEFORE -1

/** The item goes after the item it is compared against. */
#define AFTER 1

/** The first command line argument. */
#define FIRST_COMMAND 1

/** The placeholder for a count that is invalid unless changed. */
#define INVALID_COUNT -1

/**
    Struct for oItem, typedef to OrderItem, which represent individual items that can be in an Order.
    An OrderItem contains the properties of quanity and MenuItem.
*/
struct oItem
{
    MenuItem *item;
    int quantity;
};

/**
    Struct for oList, typedef to Order, which represent an Order.
    An Order contains the properties of count, capacity, and list to hold
    all OrderItems ordered by the client.
*/
struct oList
{
    struct oItem **list;
    int count;
    int capacity;
};

/**
    Typedef that renames the struct oOrder to Order.
*/
typedef struct oList Order;

/**
    Typedef that renames the struct oItem to OrderItem.
*/
typedef struct oItem OrderItem;

/**
    Dynamically allocate storage for an Order and initializes its fields.
    @return a pointer to the new Order.
*/
static Order *makeOrder() 
{
    Order *order = (Order *)malloc(sizeof(Order));
    order->capacity = INITIAL_CAPACITY;
    order->count = COUNT_START;
    order->list = (OrderItem **)malloc(order->capacity * sizeof(OrderItem *));
    return order;
}

/**
    Frees the memory used to store the given Order. This includes freeing space
    for all the OrderItems, freeing the resizable array of pointers, and freeing
    space for the Order struct.
    @param order the Order struc to free all allocated memory.
*/
static void freeOrder(Order *order) 
{
    for (int i = COUNT_START; i < order->count; i++) {
        free(order->list[i]);
    }
    free(order->list);
    free(order);
}

/**
    Compare two MenuItems against each other based on category and ID.
    @param va the first MenuItem that is being compared.
    @param vb the second MenuItem that is being compared.
    @return a number which tells if the first MenuItem should be moved
    before or after the second MenuItem.
*/
static int compareMenu(void const *va, void const *vb) 
{
    MenuItem const **a = (MenuItem const **)va;
    MenuItem const **b = (MenuItem const **)vb;
    int comparision = strcmp((*a)->category, (*b)->category);
    if (comparision < EQUAL) {
        return BEFORE;
    } else if (comparision > EQUAL) {
        return AFTER;
    } else {
        int comparision = strcmp((*a)->id, (*b)->id);
        if (comparision < EQUAL) {
            return BEFORE;
        } else if (comparision > EQUAL) {
            return AFTER;
        } else {
            return EQUAL;
        }
    }
}

/**
    Test if the given MenuItem is in the given category.
    @param item the MenuItem to check the category of.
    @param str the category of the MenuItem that is needed.
    @return if MenuItem belongs in the given category or not.
*/
static bool test(MenuItem const *item, char const *str) 
{
    if (str == NULL) {
        return true;
    }
    return strcmp(item->category, str) == EQUAL;
}

/**
    Compare two OrderItems against each other based on their price and ID.
    @param va the first OrderItem that is being compared.
    @param vb the second OrderItem that is being compared.
    @return a number which tells if the first OrderItem should be moved
    before or after the second OrderItem.
*/
static int compareOrder(void const *va, void const *vb) 
{
    OrderItem const **a = (OrderItem const **)va;
    OrderItem const **b = (OrderItem const **)vb;
    int costA = (*a)->item->cost * (*a)->quantity;
    int costB = (*b)->item->cost * (*b)->quantity;
    int comparision = costA - costB;
    if (comparision < EQUAL) {
        return AFTER;
    } else if (comparision > EQUAL) {
        return BEFORE;
    } else {
        int comparision = strcmp((*a)->item->id, (*b)->item->id);
        if (comparision < EQUAL) {
            return BEFORE;
        } else if (comparision > EQUAL) {
            return AFTER;
        } else {
            return EQUAL;
        }
    }
}

/**
    Takes the given Order and sort the OrderItems it contains in the correct order and print
    them in the correct format.
    @param order the Order with OrderItems to sort and print.
    @param compare the function that helps sort the OrderItems in the correct order.
*/
void listOrderItems(Order *order, int (*compare)(void const *va, void const *vb)) 
{
    int total = COUNT_START;
    qsort(order->list, order->count, sizeof(order->list[COUNT_START]), compareOrder);
    printf("ID   Name                 Quantity Category        Cost\n");
    for (int i = COUNT_START; i < order->count; i++) {
        total = total + (order->list[i]->item->cost * order->list[i]->quantity);
        printf("%s %-20s %8d %-15s $%3d.%02d\n", order->list[i]->item->id, order->list[i]->item->name,
            order->list[i]->quantity, order->list[i]->item->category,
            (order->list[i]->item->cost * order->list[i]->quantity) / DOLLAR, 
            (order->list[i]->item->cost * order->list[i]->quantity) % DOLLAR);
    }
    printf("Total                                              $%3d.%02d\n", total / DOLLAR, total % DOLLAR);
    printf("\n");
}

/** 
  Program starting point, reads filename from the command-line arguments.
  Create a menu from the files, allow user to input commands which can list menu
  items, list menu items by category, add menu items to their order, remove menu
  items from their order, list their order, and quit the program. 
  @param argc number of command-line arguments.
  @param argv list of command-line arguments.
  @return program exit status
*/
int main(int argc, char *argv[]) 
{
    Order *newOrder;
    Menu *newMenu;
    if (argc <= FIRST_COMMAND) {
        fprintf(stderr, "usage: kiosk <menu-file>*\n");
        exit(EXIT_FAILURE);
    } else {
        newOrder = makeOrder();
        newMenu = makeMenu();
        for (int i = FIRST_COMMAND; i < argc; i++) {
            readMenuItems(argv[i], newMenu);
        }
    }
    char *inputLine = readLine(stdin);
    char command[strlen(inputLine)];
    int currentPos = COUNT_START;
    int addPos = COUNT_START;
    printf("cmd> ");
    while (inputLine != NULL) {
        // print command given
        printf("%s\n", inputLine);
        // "quit" Command
        if (sscanf(inputLine, "%s%n", command, &addPos) == 1 && strcmp(command, "quit") == 0) {
            currentPos = currentPos + addPos;
            if (sscanf(inputLine + currentPos, "%s%n", command, &addPos) == READ) {
                printf("Invalid command\n\n");
            } else {
                free(inputLine);
                freeMenu(newMenu);
                freeOrder(newOrder);
                return EXIT_SUCCESS;
            }
        } else if (strcmp(command, "list") == EQUAL) {
            // "list" Command
            currentPos = currentPos + addPos;
            if (sscanf(inputLine + currentPos, "%s%n", command, &addPos) != READ) {
                // only cmd> list
                printf("Invalid command\n\n");
            } else {
                if (strcmp(command, "menu") == EQUAL) {
                    currentPos = currentPos + addPos;
                    if (sscanf(inputLine + currentPos, "%s%n", command, &addPos) == READ) {
                        // cmd> list menu ...
                        printf("Invalid command\n\n");
                    } else {
                        // cmd> list menu
                        listMenuItems(newMenu, compareMenu, test, NULL);
                    }
                } else if (strcmp(command, "order") == EQUAL) {
                    currentPos = currentPos + addPos;
                    if (sscanf(inputLine + currentPos, "%s%n", command, &addPos) == READ) {
                        // cmd> list order ...
                        printf("Invalid command\n\n");
                    } else {
                        // cmd> list order
                        listOrderItems(newOrder, compareMenu);
                    }
                } else if (strcmp(command, "category") == EQUAL) {
                    currentPos = currentPos + addPos;
                    if (sscanf(inputLine + currentPos, "%s%n", command, &addPos) != READ) {
                        // only cmd> list category ...
                        printf("Invalid command\n\n");
                    } else {
                        // cmd> list category (valid category)
                        char givenCategory[strlen(command)];
                        strcpy(givenCategory, command);
                        // printf("current cat: %s\n", givenCategory);
                        currentPos = currentPos + addPos;
                        if (sscanf(inputLine + currentPos, "%s%n", command, &addPos) == READ) {
                            // cmd> list category (valid category)...
                            printf("Invalid command\n\n");
                        } else {
                            listMenuItems(newMenu, compareMenu, test, givenCategory);
                        }
                    }
                } else {
                    printf("Invalid command\n\n");
                }
            }
        } else if (strcmp(command, "add") == EQUAL) {
            // add command
            currentPos = currentPos + addPos;
            if (sscanf(inputLine + currentPos, "%s%n", command, &addPos) != READ) {
                // only cmd> add
                printf("Invalid command\n\n");
            } else {
                // cmd> add (id)
                if (strlen(command) != ID_LENGTH) {
                    // not id
                    printf("Invalid command\n\n");
                } else {
                    currentPos = currentPos + addPos;
                    char givenID[strlen(command)];
                    strcpy(givenID, command);
                    int count = COUNT_START;
                    if (sscanf(inputLine + currentPos, "%d%n", &count, &addPos) != READ) {
                        // no quantity given after id
                        printf("Invalid command\n\n");
                    } else {
                        if (count <= COUNT_START) {
                            printf("Invalid command\n");
                        } else {
                            bool inMenu = false;
                            for (int i = 0; i < newMenu->count; i++) {
                                if (strcmp(givenID, newMenu->list[i]->id) == EQUAL) {
                                    inMenu = true;
                                    OrderItem *orderItem = (OrderItem *)malloc(sizeof(OrderItem));
                                    orderItem->quantity = count;
                                    orderItem->item = newMenu->list[i];
                                    if (newOrder->count >= newOrder->capacity) {
                                        newOrder->capacity *= DOUBLE;
                                        newOrder->list = (OrderItem **)realloc(newOrder->list, newOrder->capacity * sizeof(OrderItem *));
                                    }
                                    newOrder->list[newOrder->count++] = orderItem;
                                }
                            }
                            if (!inMenu) {
                                printf("Invalid command\n");
                            }
                            printf("\n");
                        }
                    }
                }
            }
        } else if (strcmp(command, "remove") == EQUAL) {
            // remove command
            currentPos = currentPos + addPos;
            if (sscanf(inputLine + currentPos, "%s%n", command, &addPos) != READ) {
                // only "remove"
                printf("Invalid command\n\n");
            } else {
                // cmd> remove (id)
                if (strlen(command) != ID_LENGTH) {
                    // not id
                    printf("Invalid command\n\n");
                } else {
                    currentPos = currentPos + addPos;
                    char givenID[strlen(command)];
                    strcpy(givenID, command);
                    int count = INVALID_COUNT;
                    if (sscanf(inputLine + currentPos, "%d%n", &count, &addPos) != READ) {
                        // no quantity given after id
                        printf("Invalid command\n\n");
                    } else {
                        int orderPlace = INVALID_COUNT;
                        for (int i = COUNT_START; i < newOrder->count; i++) {
                            if (strcmp(newOrder->list[i]->item->id, givenID) == EQUAL) {
                                orderPlace = i;
                                break;
                            }
                        }
                        if (orderPlace != INVALID_COUNT && newOrder->list[orderPlace]->quantity >= count) {
                            if (newOrder->list[orderPlace]->quantity == count) {
                                free(newOrder->list[orderPlace]);
                                for (int i = orderPlace; i < newOrder->count - ITEM; i++) {
                                    newOrder->list[i] = newOrder->list[i + ITEM];
                                }
                                newOrder->count--;
                            } else {
                                newOrder->list[orderPlace]->quantity = newOrder->list[orderPlace]->quantity - count;
                            }
                            printf("\n");
                        }
                    }
                }
            }
        } else {
            printf("Invalid command\n\n");
        }
        free(inputLine);
        currentPos = COUNT_START;
        addPos = COUNT_START;
        inputLine = readLine(stdin);
        printf("cmd> ");
        if (inputLine == NULL) {
            free(inputLine);
            freeMenu(newMenu);
            freeOrder(newOrder);
            return EXIT_SUCCESS;
        }
    }
    free(inputLine);
    freeMenu(newMenu);
    freeOrder(newOrder);
    return EXIT_SUCCESS;
}
