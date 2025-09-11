"""
Customer Management Module
Handles customer database operations
"""

import os
from datetime import datetime

class CustomerManager:
    def __init__(self, data_file="data/customers.txt"):
        self.data_file = data_file
        self.customers = []
        self.load_customers()
        
    def load_customers(self):
        """Load customers from file"""
        if not os.path.exists(os.path.dirname(self.data_file)):
            os.makedirs(os.path.dirname(self.data_file))
            
        if os.path.exists(self.data_file):
            try:
                with open(self.data_file, 'r') as f:
                    for line in f:
                        line = line.strip()
                        if line and not line.startswith('#'):
                            parts = line.split(',')
                            if len(parts) >= 5:
                                customer = {
                                    'id': parts[0].strip(),
                                    'name': parts[1].strip(),
                                    'phone': parts[2].strip(),
                                    'email': parts[3].strip(),
                                    'address': parts[4].strip()
                                }
                                self.customers.append(customer)
            except Exception as e:
                print(f"Error loading customers: {e}")
        else:
            # Create sample data file
            self.create_sample_customers()
            
    def create_sample_customers(self):
        """Create sample customers data file"""
        sample_customers = [
            "# CustID, Name, Phone, Email, Address",
            "1, Rahul, 9876543210, rahul@example.com, Patan",
            "2, Priya, 9876543211, priya@example.com, Ahmedabad",
            "3, Amit, 9876543212, amit@example.com, Gandhinagar",
            "4, Sita, 9876543213, sita@example.com, Rajkot",
            "5, Ravi, 9876543214, ravi@example.com, Surat",
            "6, Neha, 9876543215, neha@example.com, Vadodara",
            "7, Kiran, 9876543216, kiran@example.com, Bhavnagar",
            "8, Maya, 9876543217, maya@example.com, Junagadh",
            "9, Dev, 9876543218, dev@example.com, Anand",
            "10, Asha, 9876543219, asha@example.com, Mehsana"
        ]
        
        try:
            with open(self.data_file, 'w') as f:
                f.write('\n'.join(sample_customers))
            # Reload after creating
            self.load_customers()
            print(f"Sample customers data created in {self.data_file}")
        except Exception as e:
            print(f"Error creating sample customers: {e}")
            
    def search_customers(self, query):
        """Search customers by name, phone, or ID"""
        query = query.lower()
        results = []
        for customer in self.customers:
            if (query in customer['name'].lower() or 
                query in customer['phone'].lower() or 
                query in customer['id'].lower() or
                query in customer['email'].lower()):
                results.append(customer)
        return results
        
    def get_customer_by_id(self, customer_id):
        """Get customer by ID"""
        for customer in self.customers:
            if customer['id'] == customer_id:
                return customer
        return None
        
    def get_all_customers(self):
        """Get all customers"""
        return self.customers.copy()
        
    def add_customer(self, name, phone, email, address):
        """Add new customer"""
        # Generate new ID
        max_id = 0
        for customer in self.customers:
            try:
                cust_id = int(customer['id'])
                if cust_id > max_id:
                    max_id = cust_id
            except ValueError:
                continue
                
        new_customer = {
            'id': str(max_id + 1),
            'name': name,
            'phone': phone,
            'email': email,
            'address': address
        }
        
        self.customers.append(new_customer)
        self.save_customers()
        return new_customer
        
    def save_customers(self):
        """Save current customers to file"""
        try:
            with open(self.data_file, 'w') as f:
                f.write("# CustID, Name, Phone, Email, Address\n")
                for customer in self.customers:
                    f.write(f"{customer['id']}, {customer['name']}, {customer['phone']}, {customer['email']}, {customer['address']}\n")
        except Exception as e:
            print(f"Error saving customers: {e}")