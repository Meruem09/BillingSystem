#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer.h"

// Global variables
Customer customers[MAX_CUSTOMERS];
int customer_count = 0;
Customer* current_customer = NULL;

void load_customers() {
    FILE *file = fopen("data/customers.dat", "r");
    if (file == NULL) {
        printf("Creating sample customers data...\n");
        
        // Create sample customers
        Customer sample_customers[] = {
            {1, "Rahul", "9876543210", "rahul@example.com", "Patan"},
            {2, "Priya", "9876543211", "priya@example.com", "Ahmedabad"},
            {3, "Amit", "9876543212", "amit@example.com", "Gandhinagar"},
            {4, "Sita", "9876543213", "sita@example.com", "Rajkot"},
            {5, "Ravi", "9876543214", "ravi@example.com", "Surat"},
            {6, "Neha", "9876543215", "neha@example.com", "Vadodara"},
            {7, "Kiran", "9876543216", "kiran@example.com", "Bhavnagar"},
            {8, "Maya", "9876543217", "maya@example.com", "Junagadh"},
            {9, "Dev", "9876543218", "dev@example.com", "Anand"},
            {10, "Asha", "9876543219", "asha@example.com", "Mehsana"}
        };
        
        customer_count = sizeof(sample_customers) / sizeof(Customer);
        memcpy(customers, sample_customers, sizeof(sample_customers));
        save_customers();
        return;
    }
    
    customer_count = 0;
    while (fscanf(file, "%d,%49[^,],%14[^,],%49[^,],%99[^\n]\n", 
                  &customers[customer_count].id, 
                  customers[customer_count].name, 
                  customers[customer_count].phone, 
                  customers[customer_count].email, 
                  customers[customer_count].address) == 5) {
        customer_count++;
        if (customer_count >= MAX_CUSTOMERS) break;
    }
    
    fclose(file);
    printf("Loaded %d customers from database.\n", customer_count);
}

void save_customers() {
    FILE *file = fopen("data/customers.dat", "w");
    if (file == NULL) {
        printf("Error: Cannot save customers to file.\n");
        return;
    }
    
    for (int i = 0; i < customer_count; i++) {
        fprintf(file, "%d,%s,%s,%s,%s\n", 
                customers[i].id, 
                customers[i].name, 
                customers[i].phone, 
                customers[i].email, 
                customers[i].address);
    }
    
    fclose(file);
}

void display_all_customers() {
    printf("\n==================================================\n");
    printf("                 ALL CUSTOMERS\n");
    printf("==================================================\n");
    printf("%-5s %-20s %-15s %-25s\n", "ID", "Name", "Phone", "Email");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < customer_count; i++) {
        printf("%-5d %-20s %-15s %-25s\n", 
               customers[i].id, 
               customers[i].name, 
               customers[i].phone, 
               customers[i].email);
    }
    printf("==================================================\n");
}

void search_customers(const char* query) {
    printf("\n==================================================\n");
    printf("               CUSTOMER SEARCH RESULTS\n");
    printf("==================================================\n");
    printf("%-5s %-20s %-15s %-25s\n", "ID", "Name", "Phone", "Email");
    printf("--------------------------------------------------\n");
    
    int found = 0;
    char id_str[20];
    
    for (int i = 0; i < customer_count; i++) {
        sprintf(id_str, "%d", customers[i].id);
        if (strstr(customers[i].name, query) != NULL || 
            strstr(customers[i].phone, query) != NULL ||
            strstr(customers[i].email, query) != NULL ||
            strstr(id_str, query) != NULL) {
            printf("%-5d %-20s %-15s %-25s\n", 
                   customers[i].id, 
                   customers[i].name, 
                   customers[i].phone, 
                   customers[i].email);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No customers found matching '%s'\n", query);
    }
    printf("==================================================\n");
}

Customer* find_customer_by_id(int id) {
    for (int i = 0; i < customer_count; i++) {
        if (customers[i].id == id) {
            return &customers[i];
        }
    }
    return NULL;
}

void add_customer() {
    if (customer_count >= MAX_CUSTOMERS) {
        printf("Customer database is full!\n");
        return;
    }
    
    Customer new_customer;
    
    // Generate new ID
    int max_id = 0;
    for (int i = 0; i < customer_count; i++) {
        if (customers[i].id > max_id) {
            max_id = customers[i].id;
        }
    }
    new_customer.id = max_id + 1;
    
    printf("Enter customer details:\n");
    printf("Name: ");
    fgets(new_customer.name, MAX_NAME_LEN, stdin);
    new_customer.name[strcspn(new_customer.name, "\n")] = 0; // Remove newline
    
    printf("Phone: ");
    fgets(new_customer.phone, MAX_PHONE_LEN, stdin);
    new_customer.phone[strcspn(new_customer.phone, "\n")] = 0;
    
    printf("Email: ");
    fgets(new_customer.email, MAX_EMAIL_LEN, stdin);
    new_customer.email[strcspn(new_customer.email, "\n")] = 0;
    
    printf("Address: ");
    fgets(new_customer.address, MAX_ADDRESS_LEN, stdin);
    new_customer.address[strcspn(new_customer.address, "\n")] = 0;
    
    customers[customer_count] = new_customer;
    customer_count++;
    
    save_customers();
    
    printf("Customer added successfully! ID: %d\n", new_customer.id);
}