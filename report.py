"""
Report Generation Module
Handles receipt generation and sales reports
"""

import os
import json
from datetime import datetime

class ReportGenerator:
    def __init__(self, receipts_file="data/receipts.txt", receipt_details_file="data/receipt_details.txt"):
        self.receipts_file = receipts_file
        self.receipt_details_file = receipt_details_file
        self.receipts = []
        self.receipt_details = []
        self.load_receipts()
        
    def load_receipts(self):
        """Load receipts from file"""
        if not os.path.exists(os.path.dirname(self.receipts_file)):
            os.makedirs(os.path.dirname(self.receipts_file))
            
        # Load receipt headers
        if os.path.exists(self.receipts_file):
            try:
                with open(self.receipts_file, 'r') as f:
                    for line in f:
                        line = line.strip()
                        if line and not line.startswith('#'):
                            parts = line.split(',')
                            if len(parts) >= 4:
                                receipt = {
                                    'receipt_id': parts[0].strip(),
                                    'customer_id': parts[1].strip(),
                                    'date': parts[2].strip(),
                                    'total_amount': float(parts[3].strip())
                                }
                                self.receipts.append(receipt)
            except Exception as e:
                print(f"Error loading receipts: {e}")
        else:
            # Create empty receipts file
            with open(self.receipts_file, 'w') as f:
                f.write("# ReceiptID, CustID, Date, TotalAmount\n")
                
        # Load receipt details
        if os.path.exists(self.receipt_details_file):
            try:
                with open(self.receipt_details_file, 'r') as f:
                    for line in f:
                        line = line.strip()
                        if line and not line.startswith('#'):
                            parts = line.split(',')
                            if len(parts) >= 5:
                                detail = {
                                    'receipt_id': parts[0].strip(),
                                    'item_id': parts[1].strip(),
                                    'item_name': parts[2].strip(),
                                    'quantity': int(parts[3].strip()),
                                    'price': float(parts[4].strip()),
                                    'total': float(parts[5].strip()) if len(parts) > 5 else float(parts[4].strip()) * int(parts[3].strip())
                                }
                                self.receipt_details.append(detail)
            except Exception as e:
                print(f"Error loading receipt details: {e}")
        else:
            # Create empty receipt details file
            with open(self.receipt_details_file, 'w') as f:
                f.write("# ReceiptID, ItemID, ItemName, Quantity, Price, Total\n")
                
    def generate_receipt(self, customer, cart_items):
        """Generate receipt for customer purchase"""
        if not cart_items:
            return None
            
        # Generate receipt ID
        receipt_id = self.get_next_receipt_id()
        current_time = datetime.now()
        date_str = current_time.strftime("%Y-%m-%d")
        time_str = current_time.strftime("%H:%M:%S")
        
        # Calculate total
        total_amount = 0
        receipt_details = []
        
        for cart_item in cart_items:
            item = cart_item['item']
            quantity = cart_item['quantity']
            line_total = item['price'] * quantity
            total_amount += line_total
            
            detail = {
                'receipt_id': receipt_id,
                'item_id': item['id'],
                'item_name': item['name'],
                'quantity': quantity,
                'price': item['price'],
                'total': line_total
            }
            receipt_details.append(detail)
            
        # Create receipt record
        receipt = {
            'receipt_id': receipt_id,
            'customer_id': customer['id'],
            'date': date_str,
            'total_amount': total_amount
        }
        
        # Save receipt
        self.receipts.append(receipt)
        self.receipt_details.extend(receipt_details)
        self.save_receipts()
        
        # Display receipt
        self.display_receipt(customer, receipt_details, total_amount, current_time)
        
        return receipt_id
        
    def display_receipt(self, customer, receipt_details, total_amount, timestamp):
        """Display formatted receipt"""
        print("\n" + "="*60)
        print("                    XYZ RETAIL STORE")
        print("                   CUSTOMER RECEIPT")
        print("="*60)
        print(f"Date: {timestamp.strftime('%Y-%m-%d')}          Time: {timestamp.strftime('%H:%M:%S')}")
        print(f"Receipt ID: {receipt_details[0]['receipt_id']}")
        print("-"*60)
        print(f"Customer: {customer['name']}")
        print(f"Phone: {customer['phone']}")
        print(f"Email: {customer['email']}")
        print("-"*60)
        print(f"{'Item ID':<8} {'Item Name':<20} {'Qty':<5} {'Price':<10} {'Total':<10}")
        print("-"*60)
        
        for detail in receipt_details:
            print(f"{detail['item_id']:<8} {detail['item_name']:<20} {detail['quantity']:<5} ${detail['price']:<9.2f} ${detail['total']:<9.2f}")
            
        print("-"*60)
        print(f"{'TOTAL AMOUNT':<50} ${total_amount:.2f}")
        print("="*60)
        print("            Thank you for shopping with us!")
        print("="*60)
        
    def get_next_receipt_id(self):
        """Generate next receipt ID"""
        if not self.receipts:
            return "R001"
            
        max_id = 0
        for receipt in self.receipts:
            try:
                receipt_num = int(receipt['receipt_id'][1:])  # Remove 'R' prefix
                if receipt_num > max_id:
                    max_id = receipt_num
            except ValueError:
                continue
                
        return f"R{max_id + 1:03d}"
        
    def save_receipts(self):
        """Save receipts to files"""
        try:
            # Save receipt headers
            with open(self.receipts_file, 'w') as f:
                f.write("# ReceiptID, CustID, Date, TotalAmount\n")
                for receipt in self.receipts:
                    f.write(f"{receipt['receipt_id']}, {receipt['customer_id']}, {receipt['date']}, {receipt['total_amount']}\n")
                    
            # Save receipt details
            with open(self.receipt_details_file, 'w') as f:
                f.write("# ReceiptID, ItemID, ItemName, Quantity, Price, Total\n")
                for detail in self.receipt_details:
                    f.write(f"{detail['receipt_id']}, {detail['item_id']}, {detail['item_name']}, {detail['quantity']}, {detail['price']}, {detail['total']}\n")
                    
        except Exception as e:
            print(f"Error saving receipts: {e}")
            
    def daily_sales_report(self, date):
        """Generate daily sales report"""
        print(f"\n{'='*50}")
        print(f"           DAILY SALES REPORT - {date}")
        print(f"{'='*50}")
        
        daily_receipts = [r for r in self.receipts if r['date'] == date]
        
        if not daily_receipts:
            print("No sales found for this date.")
            return
            
        total_sales = 0
        total_transactions = len(daily_receipts)
        
        print(f"{'Receipt ID':<12} {'Customer ID':<12} {'Amount':<12}")
        print("-"*40)
        
        for receipt in daily_receipts:
            total_sales += receipt['total_amount']
            print(f"{receipt['receipt_id']:<12} {receipt['customer_id']:<12} ${receipt['total_amount']:<11.2f}")
            
        print("-"*40)
        print(f"Total Transactions: {total_transactions}")
        print(f"Total Sales: ${total_sales:.2f}")
        print(f"Average Transaction: ${total_sales/total_transactions:.2f}" if total_transactions > 0 else "Average Transaction: $0.00")
        print("="*50)
        
    def customer_purchase_history(self, customer_id):
        """Generate customer purchase history"""
        print(f"\n{'='*60}")
        print(f"        CUSTOMER PURCHASE HISTORY - ID: {customer_id}")
        print(f"{'='*60}")
        
        customer_receipts = [r for r in self.receipts if r['customer_id'] == customer_id]
        
        if not customer_receipts:
            print("No purchase history found for this customer.")
            return
            
        total_spent = 0
        
        for receipt in customer_receipts:
            total_spent += receipt['total_amount']
            print(f"\nReceipt ID: {receipt['receipt_id']} | Date: {receipt['date']} | Amount: ${receipt['total_amount']:.2f}")
            
            # Show items for this receipt
            receipt_items = [d for d in self.receipt_details if d['receipt_id'] == receipt['receipt_id']]
            for item in receipt_items:
                print(f"  - {item['item_name']} x{item['quantity']} @ ${item['price']:.2f} = ${item['total']:.2f}")
                
        print("-"*60)
        print(f"Total Purchases: {len(customer_receipts)}")
        print(f"Total Amount Spent: ${total_spent:.2f}")
        print("="*60)
        
    def item_sales_summary(self):
        """Generate item sales summary"""
        print(f"\n{'='*60}")
        print(f"                ITEM SALES SUMMARY")
        print(f"{'='*60}")
        
        if not self.receipt_details:
            print("No sales data available.")
            return
            
        # Aggregate item sales
        item_stats = {}
        for detail in self.receipt_details:
            item_id = detail['item_id']
            if item_id not in item_stats:
                item_stats[item_id] = {
                    'name': detail['item_name'],
                    'quantity_sold': 0,
                    'total_revenue': 0
                }
            item_stats[item_id]['quantity_sold'] += detail['quantity']
            item_stats[item_id]['total_revenue'] += detail['total']
            
        # Sort by revenue
        sorted_items = sorted(item_stats.items(), key=lambda x: x[1]['total_revenue'], reverse=True)
        
        print(f"{'Item ID':<8} {'Item Name':<20} {'Qty Sold':<10} {'Revenue':<12}")
        print("-"*60)
        
        total_revenue = 0
        total_quantity = 0
        
        for item_id, stats in sorted_items:
            total_revenue += stats['total_revenue']
            total_quantity += stats['quantity_sold']
            print(f"{item_id:<8} {stats['name']:<20} {stats['quantity_sold']:<10} ${stats['total_revenue']:<11.2f}")
            
        print("-"*60)
        print(f"Total Items Sold: {total_quantity}")
        print(f"Total Revenue: ${total_revenue:.2f}")
        print("="*60)