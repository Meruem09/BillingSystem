#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "console_status.h"

#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"

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
    clear_status_area();

    int row = STATUS_START_ROW;

    // Header
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("%s┌─── STATUS PANEL ───────────────┐%s", CYAN, RESET);

    // Current time
    MOVE_CURSOR(row++, STATUS_START_COL);
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
    printf("%s│%s Time: %s%-22s%s │", CYAN, RESET, YELLOW, time_str, RESET);

    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("%s├─────────────────────────────────┤%s", CYAN, RESET);

    // Current screen
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("%s│%s Screen: %s%-22s%s │", CYAN, RESET, GREEN, current_screen, RESET);

    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("%s├─────────────────────────────────┤%s", CYAN, RESET);

    // Customer info
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("%s│ CUSTOMER:%s                       │", CYAN, RESET);

    if (selected_customer) {
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("%s│%s ID: %s%-26d%s │", CYAN, RESET, YELLOW, selected_customer->id, RESET);

        MOVE_CURSOR(row++, STATUS_START_COL);
        char name_truncated[23];
        strncpy(name_truncated, selected_customer->name, 22);
        name_truncated[22] = '\0';
        printf("%s│%s Name: %s%-24s%s │", CYAN, RESET, GREEN, name_truncated, RESET);

        MOVE_CURSOR(row++, STATUS_START_COL);
        char phone_truncated[23];
        strncpy(phone_truncated, selected_customer->phone, 22);
        phone_truncated[22] = '\0';
        printf("%s│%s Phone: %s%-23s%s │", CYAN, RESET, MAGENTA, phone_truncated, RESET);
    } else {
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("%s│%s %sNo customer selected%s            │", CYAN, RESET, RED, RESET);
        row += 2;
    }

    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("%s├─────────────────────────────────┤%s", CYAN, RESET);

    // Cart status
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("%s│ CART:%s                           │", CYAN, RESET);

    if (is_cart_empty()) {
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("%s│%s %sEmpty%s                           │", CYAN, RESET, RED, RESET);
    } else {
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("%s│%s Items: %s%-24d%s │", CYAN, RESET, YELLOW, cart.count, RESET);

        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("%s│%s Total: %s$%-22.2f%s │", CYAN, RESET, GREEN, get_cart_total(), RESET);
    }

    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("%s├─────────────────────────────────┤%s", CYAN, RESET);

    // Last transaction
    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("%s│ LAST TRANSACTION:%s               │", CYAN, RESET);

    if (strcmp(last_receipt_id, "None") != 0) {
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("%s│%s ID: %s%-26s%s │", CYAN, RESET, YELLOW, last_receipt_id, RESET);

        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("%s│%s Amount: %s$%-20.2f%s │", CYAN, RESET, GREEN, last_transaction_amount, RESET);
    } else {
        MOVE_CURSOR(row++, STATUS_START_COL);
        printf("%s│%s %sNo transactions yet%s             │", CYAN, RESET, RED, RESET);
        row++;
    }

    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("%s├─────────────────────────────────┤%s", CYAN, RESET);

    // Status message
    MOVE_CURSOR(row++, STATUS_START_COL);
    char msg_truncated[30];
    strncpy(msg_truncated, last_message, 29);
    msg_truncated[29] = '\0';
    printf("%s│ %s%-31s%s │", CYAN, GREEN, msg_truncated, RESET);

    MOVE_CURSOR(row++, STATUS_START_COL);
    printf("%s└─────────────────────────────────┘%s", CYAN, RESET);

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