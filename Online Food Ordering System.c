#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Food {
    int id;
    char name[30];
    float price;
} menuItems[10] = {
    {1, "Burger", 150.0},
    {2, "Pizza", 250.0},
    {3, "Pasta", 200.0},
    {4, "Fries", 100.0},
    {5, "Coke", 50.0}
};
int menuCount = 5;

struct CartNode {
    int id;
    char name[30];
    float price;
    int quantity;
    struct CartNode* next;
};
struct CartNode* cartHead = NULL;

struct OrderNode {
    char details[200];
    struct OrderNode* next;
};
struct OrderNode* ordersFront = NULL;
struct OrderNode* ordersRear = NULL;

struct CanceledNode {
    char name[30];
    struct CanceledNode* next;
};
struct CanceledNode* canceledTop = NULL;

struct UserAccount {
    char username[20];
    char password[20];
} user1 = {"user1", "123"};

struct AdminAccount {
    char username[20];
    char password[20];
} admin1 = {"admin", "1234"};

void showMenu() {
    if(menuCount == 0) {
        printf("\nMenu is empty!\n");
        return;
    }
    printf("\n--- FOOD MENU ---\n");
    for(int i = 0; i < menuCount; i++) {
        printf("%d. %s - %.2f Tk\n", menuItems[i].id, menuItems[i].name, menuItems[i].price);
    }
}

void addMenuItem() {
    if(menuCount >= 10) {
        printf("Menu is full! Cannot add more items.\n");
        return;
    }
    printf("Enter food name: ");
    scanf(" %[^\n]%*c", menuItems[menuCount].name);
    printf("Enter price: ");
    scanf("%f", &menuItems[menuCount].price);

    menuCount++;
    for(int i = 0; i < menuCount; i++)
        menuItems[i].id = i + 1;

    printf("Item added successfully!\n");
}

void removeMenuItem() {
    int id, found = 0;
    printf("Enter food ID to remove: ");
    scanf("%d", &id);

    for(int i = 0; i < menuCount; i++) {
        if(menuItems[i].id == id) {
            found = 1;
            for(int j = i; j < menuCount-1; j++)
                menuItems[j] = menuItems[j+1];
            menuCount--;
            for(int k = 0; k < menuCount; k++)
                menuItems[k].id = k+1;
            printf("Item removed successfully!\n");
            break;
        }
    }
    if(!found) printf("Food ID not found!\n");
}

void addToCart() {
    int id, qty;
    showMenu();
    if(menuCount == 0) return;

    printf("Enter food ID to add to cart: ");
    scanf("%d", &id);
    if(id < 1 || id > menuCount) {
        printf("Invalid food ID!\n");
        return;
    }
    printf("Enter quantity: ");
    scanf("%d", &qty);
    if(qty < 1) {
        printf("Invalid quantity!\n");
        return;
    }

    struct CartNode* temp = cartHead;
    while(temp) {
        if(temp->id == id) {
            temp->quantity += qty;
            printf("Added %d more of %s to cart.\n", qty, temp->name);
            return;
        }
        temp = temp->next;
    }

    struct CartNode* newNode = (struct CartNode*)malloc(sizeof(struct CartNode));
    newNode->id = menuItems[id-1].id;
    strcpy(newNode->name, menuItems[id-1].name);
    newNode->price = menuItems[id-1].price;
    newNode->quantity = qty;
    newNode->next = NULL;

    if(cartHead == NULL)
        cartHead = newNode;
    else {
        temp = cartHead;
        while(temp->next) temp = temp->next;
        temp->next = newNode;
    }
    printf("Item added to cart successfully!\n");
}

void showCart() {
    if(cartHead == NULL) {
        printf("\nCart is empty!\n");
        return;
    }
    printf("\n--- YOUR CART ---\n");
    struct CartNode* temp = cartHead;
    float total = 0;
    while(temp) {
        printf("%d. %s x%d - %.2f Tk\n", temp->id, temp->name, temp->quantity, temp->price*temp->quantity);
        total += temp->price * temp->quantity;
        temp = temp->next;
    }
    printf("Total: %.2f Tk\n", total);
}

void removeCartItem() {
    if(cartHead == NULL) {
        printf("Cart is empty!\n");
        return;
    }

    int id, found = 0;
    printf("\n--- YOUR CART ---\n");
    struct CartNode* temp = cartHead;
    while(temp) {
        printf("%d. %s x%d - %.2f Tk\n", temp->id, temp->name, temp->quantity, temp->price*temp->quantity);
        temp = temp->next;
    }

    printf("Enter Food ID to remove from cart: ");
    scanf("%d", &id);

    struct CartNode* current = cartHead;
    struct CartNode* previous = NULL;

    while(current) {
        if(current->id == id) {
            found = 1;

            // Add to canceled stack
            struct CanceledNode* newCanceled = (struct CanceledNode*)malloc(sizeof(struct CanceledNode));
            strcpy(newCanceled->name, current->name);
            newCanceled->next = canceledTop;
            canceledTop = newCanceled;

            // Remove from linked list
            if(previous == NULL) {
                cartHead = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Item removed from cart!\n");
            break;
        }
        previous = current;
        current = current->next;
    }

    if(!found) printf("Food ID not found in cart!\n");
}

void placeOrder() {
    if(cartHead == NULL) {
        printf("Cart is empty! Cannot place order.\n");
        return;
    }

    char orderDetails[200] = "";
    struct CartNode* temp = cartHead;
    while(temp) {
        char item[50];
        sprintf(item, "%s x%d", temp->name, temp->quantity);
        strcat(orderDetails, item);
        if(temp->next) strcat(orderDetails, ", ");
        temp = temp->next;
    }

    struct OrderNode* newOrder = (struct OrderNode*)malloc(sizeof(struct OrderNode));
    strcpy(newOrder->details, orderDetails);
    newOrder->next = NULL;

    if(ordersFront == NULL) ordersFront = ordersRear = newOrder;
    else {
        ordersRear->next = newOrder;
        ordersRear = newOrder;
    }

    while(cartHead) {
        temp = cartHead;
        cartHead = cartHead->next;
        free(temp);
    }
    printf("Order placed successfully!\n");
}

void showOrders() {
    if(ordersFront == NULL) {
        printf("No orders in queue!\n");
        return;
    }

    struct OrderNode* temp = ordersFront;
    printf("\n--- PENDING ORDERS ---\n");
    while(temp) {
        printf("%s\n", temp->details);
        temp = temp->next;
    }
}

void userMenu() {
    int choice;
    while(1) {
        printf("\n--- USER MENU ---\n");
        printf("1. Show Menu\n2. Add to Cart\n3. Show Cart\n4. Remove Item from Cart\n");
        printf("5. Place Order\n6. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: showMenu(); break;
            case 2: addToCart(); break;
            case 3: showCart(); break;
            case 4: removeCartItem(); break;
            case 5: placeOrder(); break;
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    }
}

void adminMenu() {
    int choice;
    while(1) {
        printf("\n===== ADMIN PANEL =====\n");
        printf("1. View Menu\n2. Add Menu Item\n3. Remove Menu Item\n");
        printf("4. View Pending Orders\n5. View Canceled Items\n6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: showMenu(); break;
            case 2: addMenuItem(); break;
            case 3: removeMenuItem(); break;
            case 4: showOrders(); break;
            case 5: {
                if(canceledTop == NULL) printf("No canceled items!\n");
                else {
                    printf("\n--- CANCELED ITEMS ---\n");
                    struct CanceledNode* temp = canceledTop;
                    while(temp) {
                        printf("%s\n", temp->name);
                        temp = temp->next;
                    }
                }
                break;
            }
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    }
}
int loginUser() {
    char id[20], pass[20];
    printf("Enter User ID: ");
    scanf("%s", id);
    printf("Enter Password: ");
    scanf("%s", pass);

    if(strcmp(user1.username, id) == 0 && strcmp(user1.password, pass) == 0) {
        printf("User login successful!\n");
        return 1;
    } else {
        printf("Invalid credentials!\n");
        return 0;
    }
}

int loginAdmin() {
    char uname[20], pass[20];
    printf("Enter Admin Username: ");
    scanf("%s", uname);
    printf("Enter Password: ");
    scanf("%s", pass);

    if(strcmp(admin1.username, uname) == 0 && strcmp(admin1.password, pass) == 0) {
        printf("Admin login successful!\n");
        return 1;
    } else {
        printf("Invalid credentials!\n");
        return 0;
    }
}

void mainMenu(){
    int choice;
    while(1) {
        printf("\n===== ONLINE FOOD ORDERING SYSTEM =====\n");
        printf("1. User Login\n2. Admin Login\n3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                if(loginUser()) userMenu();
                break;
            case 2:
                if(loginAdmin()) adminMenu();
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }

}
int main() {
    mainMenu();
    return 0;
}

