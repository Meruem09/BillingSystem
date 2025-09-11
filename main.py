#!/usr/bin/env python3
"""
XYZ Retail Store Billing Application
Main entry point and menu system
"""

import os
import sys
from datetime import datetime

# Import modules
from item import ItemManager, Cart
from customer import CustomerManager
from report import ReportGenerator

class XYZBillingSystem:
    def __init__(self):
        self.item_manager = ItemManager()
        self.customer_manager = CustomerManager()
        self.report_generator = ReportGenerator()
        self.cart = Cart()
        self.current_customer = None
        
    def display_main_menu(self):
        print("\n" + "="*50)
        print("     XYZ RETAIL STORE BILLING SYSTEM")
        print("="*50)
        print("1. Item Management")
        print("2. Customer Management")
        print("3. Billing & Checkout")
        print("4. Reports")
        print("5. Exit")
        print("="*50)
        
    def display_item_menu(self):
        print("\n--- ITEM MANAGEMENT ---")
        print("1. Search Item")
        print("2. View All Items")
        print("3. Add Item to Cart")
        print("4. View Cart")
        print("5. Remove Item from Cart")
        print("6. Back to Main Menu")
        
    def display_customer_menu(self):
        print("\n--- CUSTOMER MANAGEMENT ---")
        print("1. Search Customer")
        print("2. View All Customers")
        print("3. Select Customer")
        print("4. Back to Main Menu")
        
    def display_billing_menu(self):
        print("\n--- BILLING & CHECKOUT ---")
        print("1. View Current Cart")
        print("2. Generate Receipt")
        print("3. Clear Cart")
        print("4. Back to Main Menu")
        
    def display_reports_menu(self):
        print("\n--- REPORTS ---")
        print("1. Daily Sales Report")
        print("2. Customer Purchase History")
        print("3. Item Sales Summary")
        print("4. Back to Main Menu")
        
    def handle_item_management(self):
        while True:
            self.display_item_menu()
            choice = input("\nEnter your choice: ").strip()
            
            if choice == '1':
                query = input("Enter item name or ID to search: ").strip()
                items = self.item_manager.search_items(query)
                if items:
                    print("\nSearch Results:")
                    for item in items:
                        print(f"ID: {item['id']}, Name: {item['name']}, Price: ${item['price']:.2f}, Stock: {item['stock']}")
                else:
                    print("No items found!")
                    
            elif choice == '2':
                items = self.item_manager.get_all_items()
                print("\nAll Items:")
                for item in items:
                    print(f"ID: {item['id']}, Name: {item['name']}, Price: ${item['price']:.2f}, Stock: {item['stock']}")
                    
            elif choice == '3':
                item_id = input("Enter item ID to add to cart: ").strip()
                try:
                    quantity = int(input("Enter quantity: ").strip())
                    if self.cart.add_item(self.item_manager, item_id, quantity):
                        print("Item added to cart successfully!")
                    else:
                        print("Failed to add item to cart!")
                except ValueError:
                    print("Invalid quantity entered!")
                    
            elif choice == '4':
                self.cart.display_cart()
                
            elif choice == '5':
                item_id = input("Enter item ID to remove from cart: ").strip()
                if self.cart.remove_item(item_id):
                    print("Item removed from cart!")
                else:
                    print("Item not found in cart!")
                    
            elif choice == '6':
                break
            else:
                print("Invalid choice! Please try again.")
                
    def handle_customer_management(self):
        while True:
            self.display_customer_menu()
            choice = input("\nEnter your choice: ").strip()
            
            if choice == '1':
                query = input("Enter customer name, phone, or ID to search: ").strip()
                customers = self.customer_manager.search_customers(query)
                if customers:
                    print("\nSearch Results:")
                    for customer in customers:
                        print(f"ID: {customer['id']}, Name: {customer['name']}, Phone: {customer['phone']}")
                else:
                    print("No customers found!")
                    
            elif choice == '2':
                customers = self.customer_manager.get_all_customers()
                print("\nAll Customers:")
                for customer in customers:
                    print(f"ID: {customer['id']}, Name: {customer['name']}, Phone: {customer['phone']}, Email: {customer['email']}")
                    
            elif choice == '3':
                customer_id = input("Enter customer ID: ").strip()
                customer = self.customer_manager.get_customer_by_id(customer_id)
                if customer:
                    self.current_customer = customer
                    print(f"Selected customer: {customer['name']}")
                else:
                    print("Customer not found!")
                    
            elif choice == '4':
                break
            else:
                print("Invalid choice! Please try again.")
                
    def handle_billing(self):
        while True:
            self.display_billing_menu()
            choice = input("\nEnter your choice: ").strip()
            
            if choice == '1':
                self.cart.display_cart()
                
            elif choice == '2':
                if not self.current_customer:
                    print("Please select a customer first!")
                    continue
                    
                if self.cart.is_empty():
                    print("Cart is empty! Add items before generating receipt.")
                    continue
                    
                receipt_id = self.report_generator.generate_receipt(
                    self.current_customer, 
                    self.cart.get_items()
                )
                if receipt_id:
                    print(f"Receipt generated successfully! Receipt ID: {receipt_id}")
                    self.cart.clear_cart()
                else:
                    print("Failed to generate receipt!")
                    
            elif choice == '3':
                self.cart.clear_cart()
                print("Cart cleared!")
                
            elif choice == '4':
                break
            else:
                print("Invalid choice! Please try again.")
                
    def handle_reports(self):
        while True:
            self.display_reports_menu()
            choice = input("\nEnter your choice: ").strip()
            
            if choice == '1':
                date = input("Enter date (YYYY-MM-DD) or press Enter for today: ").strip()
                if not date:
                    date = datetime.now().strftime("%Y-%m-%d")
                self.report_generator.daily_sales_report(date)
                
            elif choice == '2':
                if not self.current_customer:
                    print("Please select a customer first!")
                    continue
                self.report_generator.customer_purchase_history(self.current_customer['id'])
                
            elif choice == '3':
                self.report_generator.item_sales_summary()
                
            elif choice == '4':
                break
            else:
                print("Invalid choice! Please try again.")
                
    def run(self):
        print("Welcome to XYZ Retail Store Billing System!")
        
        while True:
            self.display_main_menu()
            choice = input("\nEnter your choice: ").strip()
            
            if choice == '1':
                self.handle_item_management()
            elif choice == '2':
                self.handle_customer_management()
            elif choice == '3':
                self.handle_billing()
            elif choice == '4':
                self.handle_reports()
            elif choice == '5':
                print("Thank you for using XYZ Billing System!")
                break
            else:
                print("Invalid choice! Please try again.")

if __name__ == "__main__":
    # Initialize system
    system = XYZBillingSystem()
    try:
        system.run()
    except KeyboardInterrupt:
        print("\n\nSystem terminated by user.")
    except Exception as e:
        print(f"An error occurred: {e}")