#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gui.h"

// Global GUI system
GUISystem gui_system = {0};

// Static variables for tracking state
static char current_screen[50] = "Main Menu";
static Customer* current_customer_gui = NULL;
static char last_message[200] = "System Ready";
static char last_message_type[20] = "info";
static char last_receipt_id[20] = "None";
static float last_transaction_amount = 0.0;
static time_t last_update_time;

int gui_init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init failed: %s\n", SDL_GetError());
        return 0;
    }
    
    // Initialize TTF
    if (TTF_Init() < 0) {
        printf("TTF init failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 0;
    }
    
    // Get screen dimensions to position window in top-right
    SDL_DisplayMode display_mode;
    if (SDL_GetCurrentDisplayMode(0, &display_mode) != 0) {
        printf("Failed to get display mode: %s\n", SDL_GetError());
        gui_system.window_x = 800;  // Fallback position
        gui_system.window_y = 50;
    } else {
        gui_system.window_x = display_mode.w - GUI_WINDOW_WIDTH - 50;
        gui_system.window_y = 50;
    }
    
    // Create window positioned in top-right corner
    gui_system.window = SDL_CreateWindow(
        "XYZ Billing System - Status Display",
        gui_system.window_x,
        gui_system.window_y,
        GUI_WINDOW_WIDTH,
        GUI_WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP
    );
    
    if (!gui_system.window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 0;
    }
    
    // Create renderer
    gui_system.renderer = SDL_CreateRenderer(gui_system.window, -1, SDL_RENDERER_ACCELERATED);
    if (!gui_system.renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(gui_system.window);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }
    
    // Load font (use system default if available)
    gui_system.font = TTF_OpenFont("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", GUI_FONT_SIZE);
    if (!gui_system.font) {
        // Try alternative font paths
        gui_system.font = TTF_OpenFont("/System/Library/Fonts/Arial.ttf", GUI_FONT_SIZE);
        if (!gui_system.font) {
            gui_system.font = TTF_OpenFont("/usr/share/fonts/TTF/arial.ttf", GUI_FONT_SIZE);
            if (!gui_system.font) {
                printf("Font loading failed: %s\n", TTF_GetError());
                SDL_DestroyRenderer(gui_system.renderer);
                SDL_DestroyWindow(gui_system.window);
                TTF_Quit();
                SDL_Quit();
                return 0;
            }
        }
    }
    
    gui_system.initialized = 1;
    last_update_time = time(NULL);
    
    // Initial display update
    gui_update_display();
    
    printf("GUI initialized successfully - Status window positioned at top-right corner\n");
    return 1;
}

void gui_cleanup() {
    if (gui_system.initialized) {
        if (gui_system.font) {
            TTF_CloseFont(gui_system.font);
        }
        if (gui_system.renderer) {
            SDL_DestroyRenderer(gui_system.renderer);
        }
        if (gui_system.window) {
            SDL_DestroyWindow(gui_system.window);
        }
        TTF_Quit();
        SDL_Quit();
        gui_system.initialized = 0;
    }
}

void gui_render_text(const char* text, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    if (!gui_system.initialized || !text) return;
    
    SDL_Color color = {r, g, b, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(gui_system.font, text, color);
    if (!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gui_system.renderer, surface);
    if (texture) {
        SDL_Rect dest_rect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(gui_system.renderer, texture, NULL, &dest_rect);
        SDL_DestroyTexture(texture);
    }
    
    SDL_FreeSurface(surface);
}

void gui_clear_screen() {
    if (!gui_system.initialized) return;
    
    // Set background color
    SDL_SetRenderDrawColor(gui_system.renderer, GUI_COLOR_BG_R, GUI_COLOR_BG_G, GUI_COLOR_BG_B, 255);
    SDL_RenderClear(gui_system.renderer);
}

void gui_present() {
    if (!gui_system.initialized) return;
    SDL_RenderPresent(gui_system.renderer);
}

void gui_update_display() {
    if (!gui_system.initialized) return;
    
    gui_clear_screen();
    
    int y_pos = 20;
    char buffer[200];
    time_t current_time = time(NULL);
    struct tm* time_info = localtime(&current_time);
    
    // Header
    gui_render_text("XYZ BILLING SYSTEM", 20, y_pos, GUI_COLOR_HEADER_R, GUI_COLOR_HEADER_G, GUI_COLOR_HEADER_B);
    y_pos += GUI_LINE_HEIGHT + 10;
    
    // Current time
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);
    sprintf(buffer, "Time: %s", buffer);
    gui_render_text(buffer, 20, y_pos, GUI_COLOR_TEXT_R, GUI_COLOR_TEXT_G, GUI_COLOR_TEXT_B);
    y_pos += GUI_LINE_HEIGHT + 10;
    
    // Current screen
    sprintf(buffer, "Current Screen: %s", current_screen);
    gui_render_text(buffer, 20, y_pos, GUI_COLOR_TEXT_R, GUI_COLOR_TEXT_G, GUI_COLOR_TEXT_B);
    y_pos += GUI_LINE_HEIGHT + 10;
    
    // Current customer
    if (current_customer_gui) {
        gui_render_text("SELECTED CUSTOMER:", 20, y_pos, GUI_COLOR_HEADER_R, GUI_COLOR_HEADER_G, GUI_COLOR_HEADER_B);
        y_pos += GUI_LINE_HEIGHT;
        
        sprintf(buffer, "ID: %d", current_customer_gui->id);
        gui_render_text(buffer, 30, y_pos, GUI_COLOR_TEXT_R, GUI_COLOR_TEXT_G, GUI_COLOR_TEXT_B);
        y_pos += GUI_LINE_HEIGHT;
        
        sprintf(buffer, "Name: %s", current_customer_gui->name);
        gui_render_text(buffer, 30, y_pos, GUI_COLOR_TEXT_R, GUI_COLOR_TEXT_G, GUI_COLOR_TEXT_B);
        y_pos += GUI_LINE_HEIGHT;
        
        sprintf(buffer, "Phone: %s", current_customer_gui->phone);
        gui_render_text(buffer, 30, y_pos, GUI_COLOR_TEXT_R, GUI_COLOR_TEXT_G, GUI_COLOR_TEXT_B);
        y_pos += GUI_LINE_HEIGHT + 10;
    } else {
        gui_render_text("No customer selected", 20, y_pos, GUI_COLOR_WARNING_R, GUI_COLOR_WARNING_G, GUI_COLOR_WARNING_B);
        y_pos += GUI_LINE_HEIGHT + 10;
    }
    
    // Cart status
    gui_render_text("CART STATUS:", 20, y_pos, GUI_COLOR_HEADER_R, GUI_COLOR_HEADER_G, GUI_COLOR_HEADER_B);
    y_pos += GUI_LINE_HEIGHT;
    
    if (is_cart_empty()) {
        gui_render_text("Cart is empty", 30, y_pos, GUI_COLOR_TEXT_R, GUI_COLOR_TEXT_G, GUI_COLOR_TEXT_B);
        y_pos += GUI_LINE_HEIGHT;
    } else {
        sprintf(buffer, "Items in cart: %d", cart.count);
        gui_render_text(buffer, 30, y_pos, GUI_COLOR_TEXT_R, GUI_COLOR_TEXT_G, GUI_COLOR_TEXT_B);
        y_pos += GUI_LINE_HEIGHT;
        
        sprintf(buffer, "Cart total: $%.2f", get_cart_total());
        gui_render_text(buffer, 30, y_pos, GUI_COLOR_SUCCESS_R, GUI_COLOR_SUCCESS_G, GUI_COLOR_SUCCESS_B);
        y_pos += GUI_LINE_HEIGHT;
        
        // Show cart items
        for (int i = 0; i < cart.count && i < 5; i++) {  // Show max 5 items
            sprintf(buffer, "- %s x%d", cart.items[i].item.name, cart.items[i].quantity);
            gui_render_text(buffer, 40, y_pos, GUI_COLOR_TEXT_R, GUI_COLOR_TEXT_G, GUI_COLOR_TEXT_B);
            y_pos += GUI_LINE_HEIGHT;
        }
        if (cart.count > 5) {
            sprintf(buffer, "... and %d more items", cart.count - 5);
            gui_render_text(buffer, 40, y_pos, GUI_COLOR_TEXT_R, GUI_COLOR_TEXT_G, GUI_COLOR_TEXT_B);
            y_pos += GUI_LINE_HEIGHT;
        }
    }
    y_pos += 10;
    
    // Last transaction
    gui_render_text("LAST TRANSACTION:", 20, y_pos, GUI_COLOR_HEADER_R, GUI_COLOR_HEADER_G, GUI_COLOR_HEADER_B);
    y_pos += GUI_LINE_HEIGHT;
    
    if (strcmp(last_receipt_id, "None") != 0) {
        sprintf(buffer, "Receipt ID: %s", last_receipt_id);
        gui_render_text(buffer, 30, y_pos, GUI_COLOR_TEXT_R, GUI_COLOR_TEXT_G, GUI_COLOR_TEXT_B);
        y_pos += GUI_LINE_HEIGHT;
        
        sprintf(buffer, "Amount: $%.2f", last_transaction_amount);
        gui_render_text(buffer, 30, y_pos, GUI_COLOR_SUCCESS_R, GUI_COLOR_SUCCESS_G, GUI_COLOR_SUCCESS_B);
        y_pos += GUI_LINE_HEIGHT;
    } else {
        gui_render_text("No transactions yet", 30, y_pos, GUI_COLOR_TEXT_R, GUI_COLOR_TEXT_G, GUI_COLOR_TEXT_B);
        y_pos += GUI_LINE_HEIGHT;
    }
    y_pos += 10;
    
    // Status message
    gui_render_text("STATUS:", 20, y_pos, GUI_COLOR_HEADER_R, GUI_COLOR_HEADER_G, GUI_COLOR_HEADER_B);
    y_pos += GUI_LINE_HEIGHT;
    
    Uint8 msg_r, msg_g, msg_b;
    if (strcmp(last_message_type, "success") == 0) {
        msg_r = GUI_COLOR_SUCCESS_R; msg_g = GUI_COLOR_SUCCESS_G; msg_b = GUI_COLOR_SUCCESS_B;
    } else if (strcmp(last_message_type, "warning") == 0) {
        msg_r = GUI_COLOR_WARNING_R; msg_g = GUI_COLOR_WARNING_G; msg_b = GUI_COLOR_WARNING_B;
    } else {
        msg_r = GUI_COLOR_TEXT_R; msg_g = GUI_COLOR_TEXT_G; msg_b = GUI_COLOR_TEXT_B;
    }
    
    gui_render_text(last_message, 30, y_pos, msg_r, msg_g, msg_b);
    
    gui_present();
}

void gui_set_current_screen(const char* screen_name) {
    if (!gui_system.initialized || !screen_name) return;
    
    strncpy(current_screen, screen_name, sizeof(current_screen) - 1);
    current_screen[sizeof(current_screen) - 1] = '\0';
    gui_update_display();
}

void gui_set_current_customer(Customer* customer) {
    if (!gui_system.initialized) return;
    
    current_customer_gui = customer;
    if (customer) {
        sprintf(last_message, "Customer selected: %s", customer->name);
        strcpy(last_message_type, "success");
    } else {
        strcpy(last_message, "No customer selected");
        strcpy(last_message_type, "warning");
    }
    gui_update_display();
}

void gui_update_cart_display() {
    if (!gui_system.initialized) return;
    
    if (is_cart_empty()) {
        strcpy(last_message, "Cart is empty");
        strcpy(last_message_type, "info");
    } else {
        sprintf(last_message, "Cart: %d items, Total: $%.2f", cart.count, get_cart_total());
        strcpy(last_message_type, "info");
    }
    gui_update_display();
}

void gui_show_message(const char* message, const char* type) {
    if (!gui_system.initialized || !message || !type) return;
    
    strncpy(last_message, message, sizeof(last_message) - 1);
    last_message[sizeof(last_message) - 1] = '\0';
    
    strncpy(last_message_type, type, sizeof(last_message_type) - 1);
    last_message_type[sizeof(last_message_type) - 1] = '\0';
    
    gui_update_display();
}

void gui_update_last_transaction(const char* receipt_id, float amount) {
    if (!gui_system.initialized || !receipt_id) return;
    
    strncpy(last_receipt_id, receipt_id, sizeof(last_receipt_id) - 1);
    last_receipt_id[sizeof(last_receipt_id) - 1] = '\0';
    
    last_transaction_amount = amount;
    
    sprintf(last_message, "Receipt %s generated: $%.2f", receipt_id, amount);
    strcpy(last_message_type, "success");
    
    gui_update_display();
}

void gui_handle_events() {
    if (!gui_system.initialized) return;
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            // Don't quit the whole application, just minimize or ignore
            // The GUI is just for display, not control
            continue;
        }
    }
}