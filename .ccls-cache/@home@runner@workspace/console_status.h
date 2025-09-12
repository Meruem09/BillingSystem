#ifndef CONSOLE_STATUS_H
#define CONSOLE_STATUS_H

#include "customer.h"
#include "item.h"

// Console positioning and display functions
void init_console_status();
void clear_status_area();
void display_status_panel();
void update_current_screen(const char* screen_name);
void update_selected_customer(Customer* customer);
void update_cart_status();
void update_last_transaction(const char* receipt_id, float amount);
void show_status_message(const char* message);

// ANSI escape codes for cursor positioning
#define MOVE_CURSOR(row, col) printf("\033[%d;%dH", (row), (col))
#define CLEAR_SCREEN() printf("\033[2J")
#define CLEAR_LINE() printf("\033[K")
#define SAVE_CURSOR() printf("\033[s")
#define RESTORE_CURSOR() printf("\033[u")
#define HIDE_CURSOR() printf("\033[?25l")
#define SHOW_CURSOR() printf("\033[?25h")

// Console dimensions and status panel area
#define CONSOLE_WIDTH 120
#define STATUS_PANEL_WIDTH 35
#define STATUS_PANEL_HEIGHT 20
#define STATUS_START_COL (CONSOLE_WIDTH - STATUS_PANEL_WIDTH + 1)
#define STATUS_START_ROW 1

#endif