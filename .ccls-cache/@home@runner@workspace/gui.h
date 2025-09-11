#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "customer.h"
#include "item.h"

#define GUI_WINDOW_WIDTH 400
#define GUI_WINDOW_HEIGHT 600
#define GUI_FONT_SIZE 14
#define GUI_LINE_HEIGHT 20
#define GUI_MAX_LINES 25

// GUI Colors
#define GUI_COLOR_BG_R 25
#define GUI_COLOR_BG_G 25
#define GUI_COLOR_BG_B 25
#define GUI_COLOR_TEXT_R 255
#define GUI_COLOR_TEXT_G 255
#define GUI_COLOR_TEXT_B 255
#define GUI_COLOR_HEADER_R 0
#define GUI_COLOR_HEADER_G 150
#define GUI_COLOR_HEADER_B 255
#define GUI_COLOR_SUCCESS_R 0
#define GUI_COLOR_SUCCESS_G 200
#define GUI_COLOR_SUCCESS_B 0
#define GUI_COLOR_WARNING_R 255
#define GUI_COLOR_WARNING_G 200
#define GUI_COLOR_WARNING_B 0

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    int initialized;
    int window_x;
    int window_y;
} GUISystem;

// Function prototypes
int gui_init();
void gui_cleanup();
void gui_update_display();
void gui_set_current_screen(const char* screen_name);
void gui_set_current_customer(Customer* customer);
void gui_update_cart_display();
void gui_show_message(const char* message, const char* type);
void gui_update_last_transaction(const char* receipt_id, float amount);
void gui_render_text(const char* text, int x, int y, Uint8 r, Uint8 g, Uint8 b);
void gui_clear_screen();
void gui_present();
void gui_handle_events();

// Global GUI system
extern GUISystem gui_system;

#endif