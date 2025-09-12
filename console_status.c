#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "console_status.h"

// Global status variables
static char current_screen[50] = "Main Menu";
static Customer* selected_customer = NULL;
static char last_message[100] = "System Ready";
static char last_receipt_id[20] = "None";
static float last_transaction_amount = 0.0;

void init_console_status() {
    // Initialize console for status display
    printf("\033[2J\033[1;1H"); // Clear screen and move to top-left
    printf("XYZ Retail Store Billing System - Console Mode\n");
    printf("Status panel will appear in the top-right corner.\n\n");
    display_status_panel();
}

void clear_status_area() {
    // Clear the status panel area
    for (int row = STATUS_START_ROW; row < STATUS_START_ROW + STATUS_PANEL_HEIGHT; row++) {
        MOVE_CURSOR(row, STATUS_START_COL);
        CLEAR_LINE();
    }
}

void display_status_panel() {
    SAVE_CURSOR();
    
    // Clear status area first
    clear_status_area();
    
    int row = STATUS_START_ROW;
    
    // Header
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("┌─── STATUS PANEL ───────────────┐");
    
    // Current time
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("│");
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
    printf(" Time: %-22s │", time_str);
    
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("├─────────────────────────────────┤");
    
    // Current screen
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("│ Screen: %-22s │", current_screen);
    
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("├─────────────────────────────────┤");
    
    // Customer info
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("│ CUSTOMER:                       │");
    
    if (selected_customer) {
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("│ ID: %-26d │", selected_customer->id);
        
        MOVE_CURSOR(row++, STATUS_START_COL);
        char name_truncated[23];
        strncpy(name_truncated, selected_customer->name, 22);
        name_truncated[22] = '\0';
        printf("│ Name: %-24s │", name_truncated);
        
        MOVE_CURSOR(row++, STATUS_START_COL);
        char phone_truncated[23];
        strncpy(phone_truncated, selected_customer->phone, 22);
        phone_truncated[22] = '\0';
        printf("│ Phone: %-23s │", phone_truncated);
    } else {
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("│ No customer selected            │");
        row += 2; // Skip empty lines
    }
    
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("├─────────────────────────────────┤");
    
    // Cart status
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("│ CART:                           │");
    
    if (is_cart_empty()) {
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("│ Empty                           │");
    } else {
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("│ Items: %-24d │", cart.count);
        
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("│ Total: $%-22.2f │", get_cart_total());
    }
    
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("├─────────────────────────────────┤");
    
    // Last transaction
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("│ LAST TRANSACTION:               │");
    
    if (strcmp(last_receipt_id, "None") != 0) {
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("│ ID: %-26s │", last_receipt_id);
        
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("│ Amount: $%-20.2f │", last_transaction_amount);
    } else {
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("│ No transactions yet             │");
        row++; // Skip empty line
    }
    
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("├─────────────────────────────────┤");
    
    // Status message
    MOVE_CURSOR(row++, STATUS_START_COL);
    char msg_truncated[30];
    strncpy(msg_truncated, last_message, 29);
    msg_truncated[29] = '\0';
    printf("│ %-31s │", msg_truncated);
    
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("└─────────────────────────────────┘");
    
    RESTORE_CURSOR();
    fflush(stdout);
}

void update_current_screen(const char* screen_name) {
    if (screen_name) {
        strncpy(current_screen, screen_name, sizeof(current_screen) - 1);
        current_screen[sizeof(current_screen) - 1] = '\0';
        display_status_panel();
    }
}

void update_selected_customer(Customer* customer) {
    selected_customer = customer;
    if (customer) {
        snprintf(last_message, sizeof(last_message), "Customer selected: %s", customer->name);
    } else {
        strcpy(last_message, "No customer selected");
    }
    display_status_panel();
}

void update_cart_status() {
    if (is_cart_empty()) {
        strcpy(last_message, "Cart is empty");
    } else {
        snprintf(last_message, sizeof(last_message), "Cart: %d items, $%.2f", cart.count, get_cart_total());
    }
    display_status_panel();
}

void update_last_transaction(const char* receipt_id, float amount) {
    if (receipt_id) {
        strncpy(last_receipt_id, receipt_id, sizeof(last_receipt_id) - 1);
        last_receipt_id[sizeof(last_receipt_id) - 1] = '\0';
        last_transaction_amount = amount;
        snprintf(last_message, sizeof(last_message), "Receipt %s: $%.2f", receipt_id, amount);
        display_status_panel();
    }
}

void show_status_message(const char* message) {
    if (message) {
        strncpy(last_message, message, sizeof(last_message) - 1);
        last_message[sizeof(last_message) - 1] = '\0';
        display_status_panel();
    }
}