"""
Item Management Module
Handles item database operations and cart management
"""

import os
import json
from datetime import datetime

class ItemManager:
    def __init__(self, data_file="data/items.txt"):
        self.data_file = data_file
        self.items = []
        self.load_items()
        
    def load_items(self):
        """Load items from file"""
        if not os.path.exists(os.path.dirname(self.data_file)):
            os.makedirs(os.path.dirname(self.data_file))
            
        if os.path.exists(self.data_file):
            try:
                with open(self.data_file, 'r') as f:
                    for line in f:
                        line = line.strip()
                        if line and not line.startswith('#'):
                            parts = line.split(',')
                            if len(parts) >= 4:
                                item = {
                                    'id': parts[0].strip(),
                                    'name': parts[1].strip(),
                                    'price': float(parts[2].strip()),
                                    'stock': int(parts[3].strip())
                                }
                                self.items.append(item)
            except Exception as e:
                print(f"Error loading items: {e}")
        else:
            # Create sample data file
            self.create_sample_items()
            
    def create_sample_items(self):
        """Create sample items data file"""
        sample_items = [
            "# ItemID, ItemName, Price, Stock",
            "101, Pen, 10.0, 100",
            "102, Notebook, 50.0, 200",
            "103, Pencil, 5.0, 150",
            "104, Eraser, 3.0, 80",
            "105, Ruler, 15.0, 60",
            "106, Calculator, 250.0, 25",
            "107, Stapler, 120.0, 40",
            "108, Paper Pack, 80.0, 75",
            "109, Marker, 25.0, 90",
            "110, Folder, 20.0, 120"
        ]
        
        try:
            with open(self.data_file, 'w') as f:
                f.write('\n'.join(sample_items))
            # Reload after creating
            self.load_items()
            print(f"Sample items data created in {self.data_file}")
        except Exception as e:
            print(f"Error creating sample items: {e}")
            
    def search_items(self, query):
        """Search items by name or ID"""
        query = query.lower()
        results = []
        for item in self.items:
            if (query in item['name'].lower() or 
                query in item['id'].lower()):
                results.append(item)
        return results
        
    def get_item_by_id(self, item_id):
        """Get item by ID"""
        for item in self.items:
            if item['id'] == item_id:
                return item
        return None
        
    def get_all_items(self):
        """Get all items"""
        return self.items.copy()
        
    def update_stock(self, item_id, quantity_sold):
        """Update item stock after sale"""
        for item in self.items:
            if item['id'] == item_id:
                item['stock'] -= quantity_sold
                self.save_items()
                return True
        return False
        
    def save_items(self):
        """Save current items to file"""
        try:
            with open(self.data_file, 'w') as f:
                f.write("# ItemID, ItemName, Price, Stock\n")
                for item in self.items:
                    f.write(f"{item['id']}, {item['name']}, {item['price']}, {item['stock']}\n")
        except Exception as e:
            print(f"Error saving items: {e}")

class Cart:
    def __init__(self):
        self.items = []  # List of {'item': item_dict, 'quantity': int}
        
    def add_item(self, item_manager, item_id, quantity):
        """Add item to cart"""
        item = item_manager.get_item_by_id(item_id)
        if not item:
            print("Item not found!")
            return False
            
        if item['stock'] < quantity:
            print(f"Insufficient stock! Available: {item['stock']}")
            return False
            
        # Check if item already in cart
        for cart_item in self.items:
            if cart_item['item']['id'] == item_id:
                if item['stock'] < (cart_item['quantity'] + quantity):
                    print(f"Total quantity exceeds stock! Available: {item['stock']}")
                    return False
                cart_item['quantity'] += quantity
                return True
                
        # Add new item to cart
        self.items.append({
            'item': item,
            'quantity': quantity
        })
        return True
        
    def remove_item(self, item_id):
        """Remove item from cart"""
        for i, cart_item in enumerate(self.items):
            if cart_item['item']['id'] == item_id:
                del self.items[i]
                return True
        return False
        
    def update_quantity(self, item_id, new_quantity):
        """Update quantity of item in cart"""
        if new_quantity <= 0:
            return self.remove_item(item_id)
            
        for cart_item in self.items:
            if cart_item['item']['id'] == item_id:
                if cart_item['item']['stock'] < new_quantity:
                    print(f"Insufficient stock! Available: {cart_item['item']['stock']}")
                    return False
                cart_item['quantity'] = new_quantity
                return True
        return False
        
    def get_total(self):
        """Calculate total amount"""
        total = 0
        for cart_item in self.items:
            total += cart_item['item']['price'] * cart_item['quantity']
        return total
        
    def get_items(self):
        """Get all items in cart"""
        return self.items.copy()
        
    def is_empty(self):
        """Check if cart is empty"""
        return len(self.items) == 0
        
    def clear_cart(self):
        """Clear all items from cart"""
        self.items = []
        
    def display_cart(self):
        """Display cart contents"""
        if self.is_empty():
            print("\nCart is empty!")
            return
            
        print("\n" + "="*60)
        print("                    CURRENT CART")
        print("="*60)
        print(f"{'ID':<5} {'Item Name':<20} {'Price':<10} {'Qty':<5} {'Total':<10}")
        print("-"*60)
        
        total_amount = 0
        for cart_item in self.items:
            item = cart_item['item']
            quantity = cart_item['quantity']
            line_total = item['price'] * quantity
            total_amount += line_total
            
            print(f"{item['id']:<5} {item['name']:<20} ${item['price']:<9.2f} {quantity:<5} ${line_total:<9.2f}")
            
        print("-"*60)
        print(f"{'TOTAL AMOUNT':<50} ${total_amount:.2f}")
        print("="*60)