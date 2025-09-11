#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

// Global variables
Item items[MAX_ITEMS];
int item_count = 0;
Cart cart = {.count = 0};

void load_items() {
    FILE *file = fopen("data/items.dat", "r");
    if (file == NULL) {
        printf("Creating sample items data...\n");
        
        // Create sample items
        Item sample_items[] = {
            {101, "Pen", 10.0, 100},
            {102, "Notebook", 50.0, 200},
            {103, "Pencil", 5.0, 150},
            {104, "Eraser", 3.0, 80},
            {105, "Ruler", 15.0, 60},
            {106, "Calculator", 250.0, 25},
            {107, "Stapler", 120.0, 40},
            {108, "Paper Pack", 80.0, 75},
            {109, "Marker", 25.0, 90},
            {110, "Folder", 20.0, 120}
        };
        
        item_count = sizeof(sample_items) / sizeof(Item);
        memcpy(items, sample_items, sizeof(sample_items));
        save_items();
        return;
    }
    
    item_count = 0;
    while (fscanf(file, "%d,%49[^,],%f,%d\n", 
                  &items[item_count].id, 
                  items[item_count].name, 
                  &items[item_count].price, 
                  &items[item_count].stock) == 4) {
        item_count++;
        if (item_count >= MAX_ITEMS) break;
    }
    
    fclose(file);
    printf("Loaded %d items from database.\n", item_count);
}

void save_items() {
    FILE *file = fopen("data/items.dat", "w");
    if (file == NULL) {
        printf("Error: Cannot save items to file.\n");
        return;
    }
    
    for (int i = 0; i < item_count; i++) {
        fprintf(file, "%d,%s,%.2f,%d\n", 
                items[i].id, 
                items[i].name, 
                items[i].price, 
                items[i].stock);
    }
    
    fclose(file);
}

void display_all_items() {
    printf("\n==================================================\n");
    printf("                   ALL ITEMS\n");
    printf("==================================================\n");
    printf("%-5s %-20s %-10s %-8s\n", "ID", "Name", "Price", "Stock");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < item_count; i++) {
        printf("%-5d %-20s $%-9.2f %-8d\n", 
               items[i].id, 
               items[i].name, 
               items[i].price, 
               items[i].stock);
    }
    printf("==================================================\n");
}

void search_items(const char* query) {
    printf("\n==================================================\n");
    printf("                 SEARCH RESULTS\n");
    printf("==================================================\n");
    printf("%-5s %-20s %-10s %-8s\n", "ID", "Name", "Price", "Stock");
    printf("--------------------------------------------------\n");
    
    int found = 0;
    char id_str[20];
    
    for (int i = 0; i < item_count; i++) {
        sprintf(id_str, "%d", items[i].id);
        if (strstr(items[i].name, query) != NULL || 
            strstr(id_str, query) != NULL) {
            printf("%-5d %-20s $%-9.2f %-8d\n", 
                   items[i].id, 
                   items[i].name, 
                   items[i].price, 
                   items[i].stock);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No items found matching '%s'\n", query);
    }
    printf("==================================================\n");
}

Item* find_item_by_id(int id) {
    for (int i = 0; i < item_count; i++) {
        if (items[i].id == id) {
            return &items[i];
        }
    }
    return NULL;
}

int add_to_cart(int item_id, int quantity) {
    Item* item = find_item_by_id(item_id);
    if (item == NULL) {
        printf("Item not found!\n");
        return 0;
    }
    
    if (item->stock < quantity) {
        printf("Insufficient stock! Available: %d\n", item->stock);
        return 0;
    }
    
    // Check if item already in cart
    for (int i = 0; i < cart.count; i++) {
        if (cart.items[i].item.id == item_id) {
            if (item->stock < (cart.items[i].quantity + quantity)) {
                printf("Total quantity exceeds stock! Available: %d\n", item->stock);
                return 0;
            }
            cart.items[i].quantity += quantity;
            return 1;
        }
    }
    
    // Add new item to cart
    if (cart.count >= MAX_CART_ITEMS) {
        printf("Cart is full!\n");
        return 0;
    }
    
    cart.items[cart.count].item = *item;
    cart.items[cart.count].quantity = quantity;
    cart.count++;
    return 1;
}

void display_cart() {
    if (cart.count == 0) {
        printf("\nCart is empty!\n");
        return;
    }
    
    printf("\n==================================================\n");
    printf("                  CURRENT CART\n");
    printf("==================================================\n");
    printf("%-5s %-20s %-10s %-5s %-10s\n", "ID", "Name", "Price", "Qty", "Total");
    printf("--------------------------------------------------\n");
    
    float total_amount = 0;
    for (int i = 0; i < cart.count; i++) {
        float line_total = cart.items[i].item.price * cart.items[i].quantity;
        total_amount += line_total;
        printf("%-5d %-20s $%-9.2f %-5d $%-9.2f\n", 
               cart.items[i].item.id, 
               cart.items[i].item.name, 
               cart.items[i].item.price, 
               cart.items[i].quantity, 
               line_total);
    }
    
    printf("--------------------------------------------------\n");
    printf("%-40s $%.2f\n", "TOTAL AMOUNT", total_amount);
    printf("==================================================\n");
}

int remove_from_cart(int item_id) {
    for (int i = 0; i < cart.count; i++) {
        if (cart.items[i].item.id == item_id) {
            // Shift remaining items
            for (int j = i; j < cart.count - 1; j++) {
                cart.items[j] = cart.items[j + 1];
            }
            cart.count--;
            return 1;
        }
    }
    return 0;
}

float get_cart_total() {
    float total = 0;
    for (int i = 0; i < cart.count; i++) {
        total += cart.items[i].item.price * cart.items[i].quantity;
    }
    return total;
}

void clear_cart() {
    cart.count = 0;
    printf("Cart cleared!\n");
}

int is_cart_empty() {
    return cart.count == 0;
}

void update_item_stock(int item_id, int quantity_sold) {
    for (int i = 0; i < item_count; i++) {
        if (items[i].id == item_id) {
            items[i].stock -= quantity_sold;
            save_items();
            break;
        }
    }
}