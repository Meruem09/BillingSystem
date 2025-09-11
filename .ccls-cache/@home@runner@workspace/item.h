#ifndef ITEM_H
#define ITEM_H

#define MAX_NAME_LEN 50
#define MAX_ITEMS 100
#define MAX_CART_ITEMS 50

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    float price;
    int stock;
} Item;

typedef struct {
    Item item;
    int quantity;
} CartItem;

typedef struct {
    CartItem items[MAX_CART_ITEMS];
    int count;
} Cart;

// Function prototypes
void load_items();
void save_items();
void display_all_items();
void search_items(const char* query);
Item* find_item_by_id(int id);
int add_to_cart(int item_id, int quantity);
void display_cart();
int remove_from_cart(int item_id);
float get_cart_total();
void clear_cart();
int is_cart_empty();
void update_item_stock(int item_id, int quantity_sold);

// Global variables
extern Item items[MAX_ITEMS];
extern int item_count;
extern Cart cart;

#endif