#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
int game_is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Posiciones y tamaños de los bolos
float pin_width = 50;
float pin_height = 50;
float pin_spacing = 20;
SDL_Rect pins[5];

// Posiciones, velocidad y tamaño de la bola
float ball_x = WINDOW_WIDTH / 2;
float ball_y = WINDOW_HEIGHT - 50;
float ball_radius = 25;
float ball_vel_x = 0;
float ball_vel_y = 0; // La velocidad vertical se establecerá al lanzar

bool ball_launched = false; // Indica si la bola ha sido lanzada

///////////////////////////////////////////////////////////////////////////////
// Function to initialize our SDL window
///////////////////////////////////////////////////////////////////////////////
int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow(
        "Bowling Game with SDL2",
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
// Function to setup the game objects
///////////////////////////////////////////////////////////////////////////////
void setup(void) {
    // Colocar los bolos (5 cuadrados) en la parte superior
    float start_x = (WINDOW_WIDTH - (5 * pin_width + 4 * pin_spacing)) / 2;
    for (int i = 0; i < 5; i++) {
        pins[i].x = start_x + i * (pin_width + pin_spacing);
        pins[i].y = 50;  // Altura fija en la parte superior
        pins[i].w = pin_width;
        pins[i].h = pin_height;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Function to draw a circle (ball)
///////////////////////////////////////////////////////////////////////////////
void draw_circle(int center_x, int center_y, int radius) {
    int x = radius;
    int y = 0;
    int radius_error = 1 - x;

    while (x >= y) {
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
// Function to detect collision between ball and pins
///////////////////////////////////////////////////////////////////////////////
bool detect_collision(SDL_Rect pin, float ball_x, float ball_y, float ball_radius) {
    float nearest_x = fmax(pin.x, fmin(ball_x, pin.x + pin.w));
    float nearest_y = fmax(pin.y, fmin(ball_y, pin.y + pin.h));
    float delta_x = ball_x - nearest_x;
    float delta_y = ball_y - nearest_y;
    return (delta_x * delta_x + delta_y * delta_y) < (ball_radius * ball_radius);
}

///////////////////////////////////////////////////////////////////////////////
// Function to update the game state
///////////////////////////////////////////////////////////////////////////////
void update(float delta_time) {
    // Mover la bola hacia arriba si ha sido lanzada
    if (ball_launched) {
        ball_y += ball_vel_y * delta_time;

        // Colisión con los bordes de la ventana
        if (ball_x - ball_radius < 0 || ball_x + ball_radius > WINDOW_WIDTH) {
            ball_vel_x = -ball_vel_x;  // Rebote horizontal
        }

        // Si la bola toca la parte superior de la pantalla, reiniciarla
        if (ball_y - ball_radius < 0) {
            ball_y = WINDOW_HEIGHT - 50;
            ball_vel_y = 0;
            ball_launched = false;  // Resetea el lanzamiento
        }

        // Detección de colisión con los bolos
        for (int i = 0; i < 5; i++) {
            if (detect_collision(pins[i], ball_x, ball_y, ball_radius)) {
                pins[i].x = -100; // Mover el bolo fuera de la pantalla al colisionar. NO FUNCIONA 
                pins[i].y = -100;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Function to render the game objects
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Dibujar los bolos
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < 5; i++) {
        SDL_RenderFillRect(renderer, &pins[i]);
    }

    // Dibujar la bola (círculo)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    draw_circle((int)ball_x, (int)ball_y, (int)ball_radius);

    SDL_RenderPresent(renderer);
}

///////////////////////////////////////////////////////////////////////////////
// Function to handle user input
///////////////////////////////////////////////////////////////////////////////
void process_input(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game_is_running = false;
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                if (!ball_launched) ball_x -= 10;  // Mover la bola a la izquierda
                break;
            case SDLK_RIGHT:
                if (!ball_launched) ball_x += 10;  // Mover la bola a la derecha
                break;
            case SDLK_SPACE:
                if (!ball_launched) {
                    ball_vel_y = -300;  // Velocidad hacia arriba
                    ball_launched = true;
                }
                break;
            }
        }
    }
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
    if (!initialize_window()) {
        return -1;
    }

    setup();

    int last_frame_time = SDL_GetTicks();
    game_is_running = true;

    while (game_is_running) {
        process_input();

        // Calcular delta time
        int time_to_wait = 16; // Aproximadamente 60 FPS
        int now = SDL_GetTicks();
        int delta_time = now - last_frame_time;
        if (delta_time < time_to_wait) {
            SDL_Delay(time_to_wait - delta_time);
            continue;
        }
        last_frame_time = now;

        update(delta_time / 1000.0f);  // Convertir a segundos
        render();
    }

    destroy_window();
    return 0;
}
