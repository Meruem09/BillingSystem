#ifndef CUSTOMER_H
#define CUSTOMER_H

#define MAX_NAME_LEN 50
#define MAX_PHONE_LEN 15
#define MAX_EMAIL_LEN 50
#define MAX_ADDRESS_LEN 100
#define MAX_CUSTOMERS 100

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
    char address[MAX_ADDRESS_LEN];
} Customer;

// Function prototypes
void load_customers();
void save_customers();
void display_all_customers();
void search_customers(const char* query);
Customer* find_customer_by_id(int id);
void add_customer();

// Global variables
extern Customer customers[MAX_CUSTOMERS];
extern int customer_count;
extern Customer* current_customer;

#endif