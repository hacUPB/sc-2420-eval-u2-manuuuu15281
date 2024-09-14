#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <math.h>

#include "./constants.h"

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
int game_is_running = false;
int last_frame_time = 0;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Posiciones, tamaños, velocidades y colores de las figuras
float rect_x = 50, rect_y = 50, rect_width = 100, rect_height = 50;
float rect_vel_x = 200, rect_vel_y = 150;

float line_x1 = 300, line_y1 = 100, line_x2 = 500, line_y2 = 300;
float line_vel_x = 180, line_vel_y = 120;

float circle_x = 400, circle_y = 400, circle_radius = 50;
float circle_vel_x = 160, circle_vel_y = 130;

int color_r = 255, color_g = 0, color_b = 0;

///////////////////////////////////////////////////////////////////////////////
// Function to initialize our SDL window
///////////////////////////////////////////////////////////////////////////////
int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow(
        "Bouncing Figures in SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Function to draw a circle using the midpoint algorithm
///////////////////////////////////////////////////////////////////////////////
void draw_circle(int center_x, int center_y, int radius) {
    int x = radius;
    int y = 0;
    int radius_error = 1 - x;

    while (x >= y) {
        // Dibujar los puntos del círculo (simetría)
        SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y + x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y + x);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x + x, center_y - y);

        y++;
        if (radius_error < 0) {
            radius_error += 2 * y + 1;
        }
        else {
            x--;
            radius_error += 2 * (y - x + 1);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Function to detect collision between two rectangles
///////////////////////////////////////////////////////////////////////////////
bool detect_collision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

///////////////////////////////////////////////////////////////////////////////
// Function to update positions and detect collisions
///////////////////////////////////////////////////////////////////////////////
void update(void) {
    // Calcular delta time en segundos
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks();

    ///////////////////////////////////////////////////////////////////////////
    // Movimiento y rebote contra las paredes
    ///////////////////////////////////////////////////////////////////////////
    // Actualizar rectángulo
    rect_x += rect_vel_x * delta_time;
    rect_y += rect_vel_y * delta_time;
    if (rect_x <= 0 || rect_x + rect_width >= WINDOW_WIDTH) rect_vel_x = -rect_vel_x;
    if (rect_y <= 0 || rect_y + rect_height >= WINDOW_HEIGHT) rect_vel_y = -rect_vel_y;

    // Actualizar círculo
    circle_x += circle_vel_x * delta_time;
    circle_y += circle_vel_y * delta_time;
    if (circle_x - circle_radius <= 0 || circle_x + circle_radius >= WINDOW_WIDTH) circle_vel_x = -circle_vel_x;
    if (circle_y - circle_radius <= 0 || circle_y + circle_radius >= WINDOW_HEIGHT) circle_vel_y = -circle_vel_y;

    // Actualizar línea (se puede representar por un rectángulo para colisiones)
    float line_width = line_x2 - line_x1;
    float line_height = line_y2 - line_y1;
    line_x1 += line_vel_x * delta_time;
    line_y1 += line_vel_y * delta_time;
    line_x2 += line_vel_x * delta_time;
    line_y2 += line_vel_y * delta_time;
    if (line_x1 <= 0 || line_x2 >= WINDOW_WIDTH) line_vel_x = -line_vel_x;
    if (line_y1 <= 0 || line_y2 >= WINDOW_HEIGHT) line_vel_y = -line_vel_y;

    ///////////////////////////////////////////////////////////////////////////
    // Detección de colisiones entre figuras
    ///////////////////////////////////////////////////////////////////////////
    // Rectángulo y círculo
    if (detect_collision(rect_x, rect_y, rect_width, rect_height, circle_x - circle_radius, circle_y - circle_radius, circle_radius * 2, circle_radius * 2)) {
        rect_vel_x = -rect_vel_x;
        rect_vel_y = -rect_vel_y;
        circle_vel_x = -circle_vel_x;
        circle_vel_y = -circle_vel_y;
    }

    // Rectángulo y línea
    if (detect_collision(rect_x, rect_y, rect_width, rect_height, line_x1, line_y1, line_width, line_height)) {
        rect_vel_x = -rect_vel_x;
        rect_vel_y = -rect_vel_y;
        line_vel_x = -line_vel_x;
        line_vel_y = -line_vel_y;
    }

    // Círculo y línea
    if (detect_collision(circle_x - circle_radius, circle_y - circle_radius, circle_radius * 2, circle_radius * 2, line_x1, line_y1, line_width, line_height)) {
        circle_vel_x = -circle_vel_x;
        circle_vel_y = -circle_vel_y;
        line_vel_x = -line_vel_x;
        line_vel_y = -line_vel_y;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw the figures in the SDL window
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    // Limpiar la pantalla
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Dibujar un rectángulo
    SDL_SetRenderDrawColor(renderer, color_r, color_g, color_b, 255);  // Color cambiante
    SDL_Rect rectangle = { (int)rect_x, (int)rect_y, (int)rect_width, (int)rect_height };
    SDL_RenderFillRect(renderer, &rectangle);

    // Dibujar una línea
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Color fijo
    SDL_RenderDrawLine(renderer, (int)line_x1, (int)line_y1, (int)line_x2, (int)line_y2);

    // Dibujar un círculo
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);  // Color fijo
    draw_circle((int)circle_x, (int)circle_y, (int)circle_radius);

    // Actualizar el render
    SDL_RenderPresent(renderer);
}

///////////////////////////////////////////////////////////////////////////////
// Function to destroy SDL window and renderer
///////////////////////////////////////////////////////////////////////////////
void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* args[]) {
    game_is_running = initialize_window();

    last_frame_time = SDL_GetTicks();

    while (game_is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game_is_running = false;
            }
        }

        update();
        render();
    }

    destroy_window();

    return 0;
}
