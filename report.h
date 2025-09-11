#ifndef REPORT_H
#define REPORT_H

#include "customer.h"
#include "item.h"

#define MAX_DATE_LEN 15
#define MAX_RECEIPTS 1000
#define MAX_RECEIPT_DETAILS 5000

typedef struct {
    char receipt_id[10];
    int customer_id;
    char date[MAX_DATE_LEN];
    float total_amount;
} Receipt;

typedef struct {
    char receipt_id[10];
    int item_id;
    char item_name[MAX_NAME_LEN];
    int quantity;
    float price;
    float total;
} ReceiptDetail;

// Function prototypes
void load_receipts();
void save_receipts();
char* generate_receipt(Customer* customer);
void display_receipt(Customer* customer, const char* receipt_id, float total);
void daily_sales_report(const char* date);
void customer_purchase_history(int customer_id);
void item_sales_summary();
char* get_next_receipt_id();
void get_current_date(char* date_str);
void get_current_time(char* time_str);

// Global variables
extern Receipt receipts[MAX_RECEIPTS];
extern ReceiptDetail receipt_details[MAX_RECEIPT_DETAILS];
extern int receipt_count;
extern int receipt_detail_count;

#endif