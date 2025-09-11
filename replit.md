# Overview

This is a retail point-of-sale (POS) system designed to manage customer transactions, inventory, and receipts. The system tracks customer information, item catalog with pricing and stock levels, and transaction records through receipts and their associated line items. It appears to be a foundational system for a small retail business, likely a stationery store based on the product types (pens, notebooks, calculators, etc.).

# User Preferences

Preferred communication style: Simple, everyday language.

# System Architecture

## Data Storage
- **File-based data storage**: The system uses plain text files with CSV-like format for data persistence
- **Normalized data structure**: Data is organized into separate entities (customers, items, receipts, receipt_details) following relational database principles
- **Simple delimited format**: Uses comma-separated values with headers for easy parsing and human readability

## Data Models
- **Customer Management**: Stores customer profiles with contact information and addresses
- **Inventory Management**: Tracks items with pricing, stock levels, and basic product information
- **Transaction Processing**: Implements a two-table approach for receipts - header table for transaction metadata and detail table for line items
- **Referential Integrity**: Uses ID-based relationships between customers, items, and receipts

## Business Logic Structure
- **Receipt System**: Implements a master-detail relationship where each receipt can contain multiple items
- **Stock Tracking**: Maintains current stock levels for inventory management
- **Customer Association**: Links all transactions to specific customers for relationship management
- **Price Calculation**: Stores both unit prices and calculated totals for transaction line items

## Design Patterns
- **Entity-Relationship Model**: Clear separation of concerns with distinct entities for different business objects
- **Master-Detail Pattern**: Receipt header contains summary information while receipt details contain line-item specifics
- **Flat File Database**: Simple, portable data storage without complex database dependencies

# External Dependencies

Currently, the system has minimal external dependencies:

- **No database server required**: Uses flat file storage eliminating need for database installation or configuration
- **No web frameworks**: Data structure suggests this could be the foundation for various frontend implementations
- **No third-party APIs**: Self-contained system with local data storage
- **Platform independent**: Text-based storage makes it portable across different operating systems

The architecture suggests this is designed as a lightweight, easily deployable system that could be extended with various frontend technologies, database backends, or integrated with external payment processing, accounting, or inventory management systems in the future.