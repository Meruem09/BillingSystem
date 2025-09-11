#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "report.h"
#include "item.h"

// Global variables
Receipt receipts[MAX_RECEIPTS];
ReceiptDetail receipt_details[MAX_RECEIPT_DETAILS];
int receipt_count = 0;
int receipt_detail_count = 0;

void load_receipts() {
    // Load receipt headers
    FILE *file = fopen("data/receipts.dat", "r");
    if (file != NULL) {
        receipt_count = 0;
        while (fscanf(file, "%9[^,],%d,%14[^,],%f\n", 
                      receipts[receipt_count].receipt_id, 
                      &receipts[receipt_count].customer_id, 
                      receipts[receipt_count].date, 
                      &receipts[receipt_count].total_amount) == 4) {
            receipt_count++;
            if (receipt_count >= MAX_RECEIPTS) break;
        }
        fclose(file);
    }
    
    // Load receipt details
    file = fopen("data/receipt_details.dat", "r");
    if (file != NULL) {
        receipt_detail_count = 0;
        while (fscanf(file, "%9[^,],%d,%49[^,],%d,%f,%f\n", 
                      receipt_details[receipt_detail_count].receipt_id, 
                      &receipt_details[receipt_detail_count].item_id, 
                      receipt_details[receipt_detail_count].item_name, 
                      &receipt_details[receipt_detail_count].quantity, 
                      &receipt_details[receipt_detail_count].price, 
                      &receipt_details[receipt_detail_count].total) == 6) {
            receipt_detail_count++;
            if (receipt_detail_count >= MAX_RECEIPT_DETAILS) break;
        }
        fclose(file);
    }
}

void save_receipts() {
    // Save receipt headers
    FILE *file = fopen("data/receipts.dat", "w");
    if (file != NULL) {
        for (int i = 0; i < receipt_count; i++) {
            fprintf(file, "%s,%d,%s,%.2f\n", 
                    receipts[i].receipt_id, 
                    receipts[i].customer_id, 
                    receipts[i].date, 
                    receipts[i].total_amount);
        }
        fclose(file);
    }
    
    // Save receipt details
    file = fopen("data/receipt_details.dat", "w");
    if (file != NULL) {
        for (int i = 0; i < receipt_detail_count; i++) {
            fprintf(file, "%s,%d,%s,%d,%.2f,%.2f\n", 
                    receipt_details[i].receipt_id, 
                    receipt_details[i].item_id, 
                    receipt_details[i].item_name, 
                    receipt_details[i].quantity, 
                    receipt_details[i].price, 
                    receipt_details[i].total);
        }
        fclose(file);
    }
}

char* generate_receipt(Customer* customer) {
    if (is_cart_empty()) {
        printf("Cart is empty! Cannot generate receipt.\n");
        return NULL;
    }
    
    static char receipt_id[10];
    strcpy(receipt_id, get_next_receipt_id());
    
    char date_str[MAX_DATE_LEN];
    get_current_date(date_str);
    
    float total_amount = get_cart_total();
    
    // Create receipt record
    Receipt new_receipt;
    strcpy(new_receipt.receipt_id, receipt_id);
    new_receipt.customer_id = customer->id;
    strcpy(new_receipt.date, date_str);
    new_receipt.total_amount = total_amount;
    
    receipts[receipt_count] = new_receipt;
    receipt_count++;
    
    // Create receipt details
    for (int i = 0; i < cart.count; i++) {
        ReceiptDetail detail;
        strcpy(detail.receipt_id, receipt_id);
        detail.item_id = cart.items[i].item.id;
        strcpy(detail.item_name, cart.items[i].item.name);
        detail.quantity = cart.items[i].quantity;
        detail.price = cart.items[i].item.price;
        detail.total = cart.items[i].item.price * cart.items[i].quantity;
        
        receipt_details[receipt_detail_count] = detail;
        receipt_detail_count++;
        
        // Update stock
        update_item_stock(cart.items[i].item.id, cart.items[i].quantity);
    }
    
    save_receipts();
    display_receipt(customer, receipt_id, total_amount);
    
    return receipt_id;
}

void display_receipt(Customer* customer, const char* receipt_id, float total) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    printf("\n==================================================\n");
    printf("                XYZ RETAIL STORE\n");
    printf("               CUSTOMER RECEIPT\n");
    printf("==================================================\n");
    printf("Date: %04d-%02d-%02d        Time: %02d:%02d:%02d\n", 
           tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
           tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("Receipt ID: %s\n", receipt_id);
    printf("--------------------------------------------------\n");
    printf("Customer: %s\n", customer->name);
    printf("Phone: %s\n", customer->phone);
    printf("Email: %s\n", customer->email);
    printf("--------------------------------------------------\n");
    printf("%-8s %-20s %-5s %-10s %-10s\n", "Item ID", "Item Name", "Qty", "Price", "Total");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < cart.count; i++) {
        float line_total = cart.items[i].item.price * cart.items[i].quantity;
        printf("%-8d %-20s %-5d $%-9.2f $%-9.2f\n", 
               cart.items[i].item.id, 
               cart.items[i].item.name, 
               cart.items[i].quantity, 
               cart.items[i].item.price, 
               line_total);
    }
    
    printf("--------------------------------------------------\n");
    printf("%-40s $%.2f\n", "TOTAL AMOUNT", total);
    printf("==================================================\n");
    printf("           Thank you for shopping with us!\n");
    printf("==================================================\n");
}

char* get_next_receipt_id() {
    static char next_id[10];
    int max_num = 0;
    
    for (int i = 0; i < receipt_count; i++) {
        int num;
        if (sscanf(receipts[i].receipt_id, "R%d", &num) == 1) {
            if (num > max_num) {
                max_num = num;
            }
        }
    }
    
    sprintf(next_id, "R%03d", max_num + 1);
    return next_id;
}

void get_current_date(char* date_str) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date_str, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void get_current_time(char* time_str) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(time_str, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void daily_sales_report(const char* date) {
    printf("\n==================================================\n");
    printf("           DAILY SALES REPORT - %s\n", date);
    printf("==================================================\n");
    
    float total_sales = 0;
    int total_transactions = 0;
    
    printf("%-12s %-12s %-12s\n", "Receipt ID", "Customer ID", "Amount");
    printf("------------------------------------------\n");
    
    for (int i = 0; i < receipt_count; i++) {
        if (strcmp(receipts[i].date, date) == 0) {
            total_sales += receipts[i].total_amount;
            total_transactions++;
            printf("%-12s %-12d $%-11.2f\n", 
                   receipts[i].receipt_id, 
                   receipts[i].customer_id, 
                   receipts[i].total_amount);
        }
    }
    
    if (total_transactions == 0) {
        printf("No sales found for this date.\n");
    } else {
        printf("------------------------------------------\n");
        printf("Total Transactions: %d\n", total_transactions);
        printf("Total Sales: $%.2f\n", total_sales);
        printf("Average Transaction: $%.2f\n", total_sales / total_transactions);
    }
    printf("==================================================\n");
}

void customer_purchase_history(int customer_id) {
    printf("\n==================================================\n");
    printf("    CUSTOMER PURCHASE HISTORY - ID: %d\n", customer_id);
    printf("==================================================\n");
    
    float total_spent = 0;
    int purchase_count = 0;
    
    for (int i = 0; i < receipt_count; i++) {
        if (receipts[i].customer_id == customer_id) {
            total_spent += receipts[i].total_amount;
            purchase_count++;
            
            printf("\nReceipt ID: %s | Date: %s | Amount: $%.2f\n", 
                   receipts[i].receipt_id, 
                   receipts[i].date, 
                   receipts[i].total_amount);
            
            // Show items for this receipt
            for (int j = 0; j < receipt_detail_count; j++) {
                if (strcmp(receipt_details[j].receipt_id, receipts[i].receipt_id) == 0) {
                    printf("  - %s x%d @ $%.2f = $%.2f\n", 
                           receipt_details[j].item_name, 
                           receipt_details[j].quantity, 
                           receipt_details[j].price, 
                           receipt_details[j].total);
                }
            }
        }
    }
    
    if (purchase_count == 0) {
        printf("No purchase history found for this customer.\n");
    } else {
        printf("--------------------------------------------------\n");
        printf("Total Purchases: %d\n", purchase_count);
        printf("Total Amount Spent: $%.2f\n", total_spent);
    }
    printf("==================================================\n");
}

void item_sales_summary() {
    printf("\n==================================================\n");
    printf("                ITEM SALES SUMMARY\n");
    printf("==================================================\n");
    
    if (receipt_detail_count == 0) {
        printf("No sales data available.\n");
        return;
    }
    
    typedef struct {
        int item_id;
        char item_name[MAX_NAME_LEN];
        int quantity_sold;
        float total_revenue;
    } ItemStats;
    
    ItemStats stats[MAX_ITEMS];
    int stat_count = 0;
    
    // Aggregate item sales
    for (int i = 0; i < receipt_detail_count; i++) {
        int found = 0;
        for (int j = 0; j < stat_count; j++) {
            if (stats[j].item_id == receipt_details[i].item_id) {
                stats[j].quantity_sold += receipt_details[i].quantity;
                stats[j].total_revenue += receipt_details[i].total;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            stats[stat_count].item_id = receipt_details[i].item_id;
            strcpy(stats[stat_count].item_name, receipt_details[i].item_name);
            stats[stat_count].quantity_sold = receipt_details[i].quantity;
            stats[stat_count].total_revenue = receipt_details[i].total;
            stat_count++;
        }
    }
    
    // Simple bubble sort by revenue (descending)
    for (int i = 0; i < stat_count - 1; i++) {
        for (int j = 0; j < stat_count - i - 1; j++) {
            if (stats[j].total_revenue < stats[j + 1].total_revenue) {
                ItemStats temp = stats[j];
                stats[j] = stats[j + 1];
                stats[j + 1] = temp;
            }
        }
    }
    
    printf("%-8s %-20s %-10s %-12s\n", "Item ID", "Item Name", "Qty Sold", "Revenue");
    printf("--------------------------------------------------\n");
    
    float total_revenue = 0;
    int total_quantity = 0;
    
    for (int i = 0; i < stat_count; i++) {
        total_revenue += stats[i].total_revenue;
        total_quantity += stats[i].quantity_sold;
        printf("%-8d %-20s %-10d $%-11.2f\n", 
               stats[i].item_id, 
               stats[i].item_name, 
               stats[i].quantity_sold, 
               stats[i].total_revenue);
    }
    
    printf("--------------------------------------------------\n");
    printf("Total Items Sold: %d\n", total_quantity);
    printf("Total Revenue: $%.2f\n", total_revenue);
    printf("==================================================\n");
}