
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "customer.h"
#include "report.h"
#include "gui.h"

void display_main_menu() {
    printf("\n==================================================\n");
    printf("     XYZ RETAIL STORE BILLING SYSTEM\n");
    printf("==================================================\n");
    printf("1. Item Management\n");
    printf("2. Customer Management\n");
    printf("3. Billing & Checkout\n");
    printf("4. Reports\n");
    printf("5. Exit\n");
    printf("==================================================\n");
}

void display_item_menu() {
    printf("\n--- ITEM MANAGEMENT ---\n");
    printf("1. Search Item\n");
    printf("2. View All Items\n");
    printf("3. Add Item to Cart\n");
    printf("4. View Cart\n");
    printf("5. Remove Item from Cart\n");
    printf("6. Back to Main Menu\n");
}

void display_customer_menu() {
    printf("\n--- CUSTOMER MANAGEMENT ---\n");
    printf("1. Search Customer\n");
    printf("2. View All Customers\n");
    printf("3. Add New Customer\n");
    printf("4. Select Customer\n");
    printf("5. Back to Main Menu\n");
}

void display_billing_menu() {
    printf("\n--- BILLING & CHECKOUT ---\n");
    printf("1. View Current Cart\n");
    printf("2. Generate Receipt\n");
    printf("3. Clear Cart\n");
    printf("4. Back to Main Menu\n");
}

void display_reports_menu() {
    printf("\n--- REPORTS ---\n");
    printf("1. Daily Sales Report\n");
    printf("2. Customer Purchase History\n");
    printf("3. Item Sales Summary\n");
    printf("4. Back to Main Menu\n");
}

void handle_item_management() {
    int choice;
    char query[100];
    int item_id, quantity;
    
    while (1) {
        display_item_menu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter item name or ID to search: ");
                fgets(query, sizeof(query), stdin);
                query[strcspn(query, "\n")] = 0; // Remove newline
                search_items(query);
                break;
                
            case 2:
                display_all_items();
                break;
                
            case 3:
                printf("Enter item ID to add to cart: ");
                scanf("%d", &item_id);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                if (add_to_cart(item_id, quantity)) {
                    printf("Item added to cart successfully!\n");
                    gui_update_cart_display();
                    gui_show_message("Item added to cart!", "success");
                } else {
                    printf("Failed to add item to cart!\n");
                    gui_show_message("Failed to add item to cart!", "warning");
                }
                break;
                
            case 4:
                display_cart();
                break;
                
            case 5:
                printf("Enter item ID to remove from cart: ");
                scanf("%d", &item_id);
                if (remove_from_cart(item_id)) {
                    printf("Item removed from cart!\n");
                } else {
                    printf("Item not found in cart!\n");
                }
                break;
                
            case 6:
                return;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

void handle_customer_management() {
    int choice;
    char query[100];
    int customer_id;
    
    while (1) {
        display_customer_menu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter customer name, phone, or ID to search: ");
                fgets(query, sizeof(query), stdin);
                query[strcspn(query, "\n")] = 0;
                search_customers(query);
                break;
                
            case 2:
                display_all_customers();
                break;
                
            case 3:
                add_customer();
                break;
                
            case 4:
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                current_customer = find_customer_by_id(customer_id);
                if (current_customer) {
                    printf("Selected customer: %s\n", current_customer->name);
                    gui_set_current_customer(current_customer);
                } else {
                    printf("Customer not found!\n");
                    gui_show_message("Customer not found!", "warning");
                }
                break;
                
            case 5:
                return;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

void handle_billing() {
    int choice;
    
    while (1) {
        display_billing_menu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                display_cart();
                break;
                
            case 2:
                if (!current_customer) {
                    printf("Please select a customer first!\n");
                    break;
                }
                
                if (is_cart_empty()) {
                    printf("Cart is empty! Add items before generating receipt.\n");
                    break;
                }
                
                char* receipt_id = generate_receipt(current_customer);
                if (receipt_id) {
                    printf("Receipt generated successfully! Receipt ID: %s\n", receipt_id);
                    float total = get_cart_total();
                    gui_update_last_transaction(receipt_id, total);
                    clear_cart();
                    gui_update_cart_display();
                } else {
                    printf("Failed to generate receipt!\n");
                }
                break;
                
            case 3:
                clear_cart();
                break;
                
            case 4:
                return;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

void handle_reports() {
    int choice;
    char date[20];
    
    while (1) {
        display_reports_menu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter date (YYYY-MM-DD) or press Enter for today: ");
                fgets(date, sizeof(date), stdin);
                date[strcspn(date, "\n")] = 0;
                
                if (strlen(date) == 0) {
                    get_current_date(date);
                }
                daily_sales_report(date);
                break;
                
            case 2:
                if (!current_customer) {
                    printf("Please select a customer first!\n");
                    break;
                }
                customer_purchase_history(current_customer->id);
                break;
                
            case 3:
                item_sales_summary();
                break;
                
            case 4:
                return;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

int main() {
    int choice;
    
    printf("Initializing XYZ Retail Store Billing System...\n");
    
    // Initialize data
    load_items();
    load_customers();
    load_receipts();
    
    // Initialize GUI
    if (gui_init()) {
        printf("GUI status window initialized at top-right corner.\n");
        gui_set_current_screen("Main Menu");
        gui_show_message("System Ready", "success");
    } else {
        printf("Warning: GUI initialization failed. Running in console-only mode.\n");
    }
    
    printf("Welcome to XYZ Retail Store Billing System!\n");
    
    while (1) {
        display_main_menu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                gui_set_current_screen("Item Management");
                gui_handle_events(); // Process any GUI events
                handle_item_management();
                gui_set_current_screen("Main Menu");
                break;
                
            case 2:
                gui_set_current_screen("Customer Management");
                gui_handle_events();
                handle_customer_management();
                gui_set_current_screen("Main Menu");
                break;
                
            case 3:
                gui_set_current_screen("Billing & Checkout");
                gui_handle_events();
                handle_billing();
                gui_set_current_screen("Main Menu");
                break;
                
            case 4:
                gui_set_current_screen("Reports");
                gui_handle_events();
                handle_reports();
                gui_set_current_screen("Main Menu");
                break;
                
            case 5:
                printf("Thank you for using XYZ Billing System!\n");
                exit(0);
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    // Cleanup GUI before exit
    gui_cleanup();
    
    return 0;
}