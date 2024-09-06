[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/C5iNLRra)
# Documentación del Proyecto
## Unidad 2

Estudiante: Manuela Buriticá Bedoya 
Id: 000505665
---
### Ejercicio 1: Reconociendo las Partes del Game Loop
**Función Setup()**
```c
void setup(void) {
    // Initialize the ball object moving down at a constant velocity
    ball.x = 10;
    ball.y = 20;
    ball.width = 20;
    ball.height = 20;
    ball.vel_x = 180;
    ball.vel_y = 140;
}
```
En esta función se inicializan los objetos del juego. En este caso, la bola comienza en la posición fija x=10, y=20 y tiene una velocidad constante en ambos ejes.

**Función Update()**
```c
void update(void) {
    // Get delta_time factor converted to seconds to be used to update objects
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

    // Store the milliseconds of the current frame to be used in the next one
    last_frame_time = SDL_GetTicks();

    // Move ball as a function of delta time
    ball.x += ball.vel_x * delta_time;
    ball.y += ball.vel_y * delta_time;

    // Check for ball collision with the window borders
    if (ball.x < 0) {
        ball.x = 0;
        ball.vel_x = -ball.vel_x;
    }
    if (ball.x + ball.height > WINDOW_WIDTH) {
        ball.x = WINDOW_WIDTH - ball.width;
        ball.vel_x = -ball.vel_x;
    }
    if (ball.y < 0) {
        ball.y = 0;
        ball.vel_y = -ball.vel_y;
    }
    if (ball.y + ball.height > WINDOW_HEIGHT) {
        ball.y = WINDOW_HEIGHT - ball.height;
        ball.vel_y = -ball.vel_y;
    }
}
```
Esta función actualiza la lógica del juego en cada cuadro de la siguiente manera:

- Calcula delta_time para determinar cuánto tiempo ha pasado desde el último cuadro.
- Mueve la bola según su velocidad y el tiempo transcurrido.
- Verifica si la bola choca con los bordes de la ventana, invirtiendo la dirección si es necesario.

**Función Render()**
```c
void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw a rectangle for the ball object
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    SDL_RenderPresent(renderer);
}
```
Esta  función dibuja los objetos del juego de la sgte manera: 

- Limpia la pantalla.
- Dibuja un rectángulo que representa la bola.
- Presenta lo que ha sido dibujado en la pantalla.
- 
**Implementación del printf**

![image](https://github.com/user-attachments/assets/e8b2f863-c70c-45f7-b425-8ead6abd2fd6)

![image](https://github.com/user-attachments/assets/424b0f83-c5b8-4c6f-bb8a-58a26985dfd7)

![image](https://github.com/user-attachments/assets/6f529d5b-c2e1-4e35-bd4d-195882f2df22)

![image](https://github.com/user-attachments/assets/edbedefd-44a4-4c40-a3a5-255ada7b2e17)

### Ejercicio 2: Calculando el Tiempo de Ejecución
Para este ejercicio me ayudé del video y de la IA para lograr calcular el tiempo entre la llamada de las funciones Update() y Render(). Implementé un cronometraje utilizando la función SDL_GetTicks(). El código modificado de las funciones Update() y Render() quedó de la siguiente manera: 

```c
void update(void) {
    // Inicio del cronómetro para update
    update_start_time = SDL_GetTicks();

    printf("Llama a la funcion Update .\n");

    // Get delta_time factor converted to seconds to be used to update objects
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

    // Store the milliseconds of the current frame to be used in the next one
    last_frame_time = SDL_GetTicks();

    // Move ball as a function of delta time
    ball.x += ball.vel_x * delta_time;
    ball.y += ball.vel_y * delta_time;

    // Check for ball collision with the window borders
    if (ball.x < 0) {
        ball.x = 0;
        ball.vel_x = -ball.vel_x;
    }
    if (ball.x + ball.height > WINDOW_WIDTH) {
        ball.x = WINDOW_WIDTH - ball.width;
        ball.vel_x = -ball.vel_x;
    }
    if (ball.y < 0) {
        ball.y = 0;
        ball.vel_y = -ball.vel_y;
    }
    if (ball.y + ball.height > WINDOW_HEIGHT) {
        ball.y = WINDOW_HEIGHT - ball.height;
        ball.vel_y = -ball.vel_y;
    }

    // Fin del cronómetro para update
    update_end_time = SDL_GetTicks();
    printf("Update took %d ms\n", update_end_time - update_start_time);
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw game objects in the SDL window
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    // Inicio del cronómetro para render
    render_start_time = SDL_GetTicks();

    printf("Llama a la funcion Render .\n");

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw a rectangle for the ball object
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    SDL_RenderPresent(renderer);

    // Fin del cronómetro para render
    render_end_time = SDL_GetTicks();
    printf("Render took %d ms\n", render_end_time - render_start_time);
}
```
Así se ve la consola: 
![image](https://github.com/user-attachments/assets/319b8951-6273-49d2-a75b-d64489028e52)

Mantener una frecuencia de actualización constante asegura que el juego se vea fluido, sin saltos bruscos en los fotogramas. Si los tiempos de actualización o renderizado varían demasiado, el jugador notará fluctuaciones que pueden distraer y frustrar. Además mantener tiempos de update() y render() bajos y constantes es crucial para una experiencia de juego fluida. Cuando estos tiempos varían demasiado, la fluidez del juego se ve afectada, lo que lleva a una peor jugabilidad. Asegurarse de optimizar el renderizado es vital para mantener una frecuencia de actualización constante y proporcionar la mejor experiencia posible al jugador.


### Ejercicio 3: Creación de Figuras Geométricas
Para este ejercicio se me complicó un poco hacer el circulo pues SDL no contiene una función especifica para hacer esta figura.Para esto usé el algoritmo de punto medio, esta idea me la dió la IA. El codigo quedó de la sgte manera: 
```c
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <math.h>  // Para funciones matemáticas como sqrt

#include "./constants.h"

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
int game_is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

///////////////////////////////////////////////////////////////////////////////
// Function to initialize our SDL window
///////////////////////////////////////////////////////////////////////////////
int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow(
        "Geometric Figures in SDL",
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
        } else {
            x--;
            radius_error += 2 * (y - x + 1);
        }
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
    SDL_Rect rectangle = {50, 50, 200, 100};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Rojo
    SDL_RenderFillRect(renderer, &rectangle);

    // Dibujar una línea
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Verde
    SDL_RenderDrawLine(renderer, 300, 100, 500, 300);

    // Dibujar un círculo
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Azul
    draw_circle(400, 400, 100);

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

    while (game_is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game_is_running = false;
            }
        }

        render();
    }

    destroy_window();

    return 0;
}
```
y en la terminal se ve de esta manera: 
![image](https://github.com/user-attachments/assets/e7ced8f3-a569-4a58-8f89-7eb2adcdbdb7)

### Ejercicio 4: Movimiento de Figuras en Pantalla
