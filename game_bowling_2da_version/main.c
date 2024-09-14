#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct {
    float x, y;
    float width, height;
    float vel_x, vel_y;
    bool is_moving;
} GameObject;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool game_is_running = false;
GameObject ball;
GameObject pins[5]; // 5 bolos
bool ball_launched = false;

///////////////////////////////////////////////////////////////////////////////
// Inicializar ventana
///////////////////////////////////////////////////////////////////////////////
bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow("Bolos", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Configuración inicial del juego
///////////////////////////////////////////////////////////////////////////////
void setup(void) {
    // Configurar la bola
    ball.x = WINDOW_WIDTH / 2 - 15;
    ball.y = WINDOW_HEIGHT - 50;
    ball.width = 30;
    ball.height = 30;
    ball.vel_x = 200;  // Movimiento horizontal automático
    ball.vel_y = 0;

    // Configurar los bolos (estarán quietos al inicio)
    int pin_spacing = 100;
    for (int i = 0; i < 5; i++) {
        pins[i].x = 150 + i * pin_spacing;
        pins[i].y = 50;
        pins[i].width = 30;
        pins[i].height = 30;
        pins[i].vel_x = 0;
        pins[i].vel_y = 0;
        pins[i].is_moving = false;  // Los bolos empiezan quietos
    }
}

///////////////////////////////////////////////////////////////////////////////
// Procesar input (clic de ratón para lanzar la bola)
///////////////////////////////////////////////////////////////////////////////
void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
        game_is_running = false;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (!ball_launched) {
            ball.vel_y = -300;  // Lanzar la bola hacia arriba
            ball_launched = true;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Detectar colisión entre dos objetos
///////////////////////////////////////////////////////////////////////////////
bool check_collision(GameObject a, GameObject b) {
    return (a.x < b.x + b.width &&
        a.x + a.width > b.x &&
        a.y < b.y + b.height &&
        a.y + a.height > b.y);
}

///////////////////////////////////////////////////////////////////////////////
// Aplicar un rebote entre los bolos tras la colisión
///////////////////////////////////////////////////////////////////////////////
void resolve_collision(GameObject* a, GameObject* b) {
    // Calculamos la diferencia de velocidades
    float normal_x = b->x - a->x;
    float normal_y = b->y - a->y;

    // Normalizar el vector normal
    float magnitude = sqrt(normal_x * normal_x + normal_y * normal_y);
    if (magnitude == 0) return; // Evitar divisiones por cero

    normal_x /= magnitude;
    normal_y /= magnitude;

    // Separar los bolos para que no se superpongan
    float overlap = (a->width + b->width) / 2 - magnitude / 2;
    a->x -= normal_x * overlap;
    a->y -= normal_y * overlap;
    b->x += normal_x * overlap;
    b->y += normal_y * overlap;

    // Intercambiar las velocidades en la dirección de la colisión
    float relative_vel_x = b->vel_x - a->vel_x;
    float relative_vel_y = b->vel_y - a->vel_y;

    // Producto punto entre la velocidad relativa y el vector normal
    float dot_product = (relative_vel_x * normal_x + relative_vel_y * normal_y);

    // Asumimos coeficiente de restitución perfecto (rebote elástico)
    float restitution = 1.0;

    // Cambiar las velocidades a lo largo del vector normal
    a->vel_x -= restitution * dot_product * normal_x;
    a->vel_y -= restitution * dot_product * normal_y;
    b->vel_x += restitution * dot_product * normal_x;
    b->vel_y += restitution * dot_product * normal_y;

    // Marcar los bolos como en movimiento
    a->is_moving = true;
    b->is_moving = true;
}

///////////////////////////////////////////////////////////////////////////////
// Actualizar las posiciones de los objetos
///////////////////////////////////////////////////////////////////////////////
void update(float delta_time) {
    // Movimiento de la bola de lado a lado
    if (!ball_launched) {
        ball.x += ball.vel_x * delta_time;
        if (ball.x <= 0 || ball.x + ball.width >= WINDOW_WIDTH) {
            ball.vel_x = -ball.vel_x; // Rebotar en las paredes
        }
    }

    // Actualizar la posición de la bola si ha sido lanzada
    ball.y += ball.vel_y * delta_time;

    // Detectar colisiones con los bolos
    for (int i = 0; i < 5; i++) {
        if (check_collision(ball, pins[i])) {
            // Transferir la velocidad de la bola al bolo en la misma dirección
            if (!pins[i].is_moving) {
                pins[i].vel_x = ball.vel_x * 0.5;  // El bolo adquiere la mitad de la velocidad de la bola en X
                pins[i].vel_y = ball.vel_y * 0.5;  // El bolo adquiere la mitad de la velocidad de la bola en Y
                pins[i].is_moving = true;  // Ahora el bolo está en movimiento
            }

            // Rebote simple: invertir la velocidad en Y al golpear un bolo
            ball.vel_y = -ball.vel_y;
        }
    }

    // Detectar colisiones entre los bolos
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (check_collision(pins[i], pins[j])) {
                resolve_collision(&pins[i], &pins[j]);
            }
        }
    }

    // Actualizar la posición de los bolos que están en movimiento
    for (int i = 0; i < 5; i++) {
        if (pins[i].is_moving) {
            pins[i].x += pins[i].vel_x * delta_time;
            pins[i].y += pins[i].vel_y * delta_time;

            // Hacer que los bolos reboten en las paredes
            if (pins[i].x <= 0 || pins[i].x + pins[i].width >= WINDOW_WIDTH) {
                pins[i].vel_x = -pins[i].vel_x;
            }
            if (pins[i].y <= 0 || pins[i].y + pins[i].height >= WINDOW_HEIGHT) {
                pins[i].vel_y = -pins[i].vel_y;
            }
        }
    }

    // Si la bola sale de la pantalla por la parte superior, detener el juego
    if (ball.y < 0) {
        ball_launched = false;  // Restablecer la bola
        ball.x = WINDOW_WIDTH / 2 - 15;
        ball.y = WINDOW_HEIGHT - 50;
        ball.vel_y = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Renderizar los objetos
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fondo negro
    SDL_RenderClear(renderer);

    // Dibujar la bola
    SDL_Rect ball_rect = { (int)ball.x, (int)ball.y, (int)ball.width, (int)ball.height };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Bola blanca
    SDL_RenderFillRect(renderer, &ball_rect);

    // Dibujar los bolos
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Bolos rojos
    for (int i = 0; i < 5; i++) {
        SDL_Rect pin_rect = { (int)pins[i].x, (int)pins[i].y, (int)pins[i].width, (int)pins[i].height };
        SDL_RenderFillRect(renderer, &pin_rect);
    }

    SDL_RenderPresent(renderer);
}

///////////////////////////////////////////////////////////////////////////////
// Limpiar y destruir la ventana
///////////////////////////////////////////////////////////////////////////////
void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

///////////////////////////////////////////////////////////////////////////////
// Función principal
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    game_is_running = initialize_window();

    setup();

    int last_frame_time = 0;

    while (game_is_running) {
        process_input();

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
