#include <SDL2/SDL.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <random>
#include <ctime>

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

const int FRAMEBUFFER_WIDTH = 200;
const int FRAMEBUFFER_HEIGHT = 200;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const float RENDER_SCALE = 0.95;
const int RENDER_WIDTH = FRAMEBUFFER_WIDTH / RENDER_SCALE;
const int RENDER_HEIGHT = FRAMEBUFFER_HEIGHT / RENDER_SCALE;
const int FRAMEBUFFER_SIZE = RENDER_WIDTH * RENDER_HEIGHT;

Color framebuffer[FRAMEBUFFER_SIZE];
Color aliveColor = { 255, 255, 255 };
Color deadColor = { 0, 0, 0 };

void setPixel(int x, int y, Color color) {
    if (x >= 0 && x < RENDER_WIDTH && y >= 0 && y < RENDER_HEIGHT) {
        framebuffer[y * RENDER_WIDTH + x] = color;
    }
}

void renderBuffer(SDL_Renderer* renderer) {
    // Crear una textura
    SDL_Texture* texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_STATIC,
            RENDER_WIDTH,
            RENDER_HEIGHT
    );

    // Actualizar la textura con los datos de los píxeles en el framebuffer
    SDL_UpdateTexture(
            texture,
            NULL,
            framebuffer,
            RENDER_WIDTH * sizeof(Color)
    );

    // Limpiar el renderer
    SDL_RenderClear(renderer);

    // Establecer la escala para la representación
    SDL_RenderSetScale(renderer, RENDER_SCALE, RENDER_SCALE);

    // Copiar la textura al renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Destruir la textura
    SDL_DestroyTexture(texture);

    // Actualizar el renderer
    SDL_RenderPresent(renderer);
}

int countAliveNeighbors(int x, int y) {
    int aliveNeighbors = 0;

    // Revisar los 8 vecinos del píxel en (x, y)
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            // Excluir el píxel central en sí mismo
            if (i == 0 && j == 0) {
                continue;
            }

            int neighborX = x + i;
            int neighborY = y + j;

            // Revisar si el vecino está dentro de los límites del framebuffer
            if (neighborX >= 0 && neighborX < RENDER_WIDTH && neighborY >= 0 && neighborY < RENDER_HEIGHT) {
                // Revisar si el vecino está vivo (blanco)
                if (framebuffer[neighborY * RENDER_WIDTH + neighborX].r == aliveColor.r &&
                    framebuffer[neighborY * RENDER_WIDTH + neighborX].g == aliveColor.g &&
                    framebuffer[neighborY * RENDER_WIDTH + neighborX].b == aliveColor.b) {
                    aliveNeighbors++;
                }
            }
        }
    }

    return aliveNeighbors;
}

void updateGameOfLife() {
    Color newFramebuffer[FRAMEBUFFER_SIZE];

    // Iterar sobre cada píxel en el framebuffer
    for (int y = 0; y < RENDER_HEIGHT; y++) {
        for (int x = 0; x < RENDER_WIDTH; x++) {
            // Obtener el estado actual del píxel
            Color currentColor = framebuffer[y * RENDER_WIDTH + x];
            bool isAlive = (currentColor.r == aliveColor.r &&
                            currentColor.g == aliveColor.g &&
                            currentColor.b == aliveColor.b);

            // Contar el número de vecinos vivos
            int aliveNeighbors = countAliveNeighbors(x, y);

            // Aplicar las reglas del Juego de la Vida
            if (isAlive) {
                if (aliveNeighbors < 2 || aliveNeighbors > 3) {
                    // Cualquier célula viva con menos de dos o más de tres vecinos vivos muere
                    newFramebuffer[y * RENDER_WIDTH + x] = deadColor;
                } else {
                    // Cualquier célula viva con dos o tres vecinos vivos sobrevive
                    newFramebuffer[y * RENDER_WIDTH + x] = aliveColor;
                }
            } else {
                if (aliveNeighbors == 3) {
                    // Cualquier célula muerta con exactamente tres vecinos vivos se vuelve viva
                    newFramebuffer[y * RENDER_WIDTH + x] = aliveColor;
                } else {
                    // Las células muertas con cualquier otro número de vecinos vivos permanecen muertas
                    newFramebuffer[y * RENDER_WIDTH + x] = deadColor;
                }
            }
        }
    }

    std::memcpy(framebuffer, newFramebuffer, sizeof(framebuffer));
}

void initializeGameOfLife() {
    // Limpiar el framebuffer (opcional, ya que actualizamos todos los píxeles durante la inicialización)
    std::memset(framebuffer, 0, sizeof(framebuffer));

    // Definir el patrón inicial - Spaceship (Glider)
    std::vector<std::pair<int, int>> gliderPattern = {
            { 1, 0 }, { 2, 1 }, { 0, 2 }, { 1, 2 }, { 2, 2 }
    };

    // Colocar el patrón de la nave espacial en posiciones aleatorias en el framebuffer
    for (int i = 0; i < 100; i++) {  // Incrementar el número de naves espaciales a 100
        int x = std::rand() % RENDER_WIDTH;
        int y = std::rand() % RENDER_HEIGHT;

        for (const auto& point : gliderPattern) {
            int gliderX = point.first + x;
            int gliderY = point.second + y;
            setPixel(gliderX, gliderY, aliveColor);
        }
    }

    // Definir el patrón inicial - Gun (Gosper glider gun)
    std::vector<std::pair<int, int>> gunPattern = {
            { 0, 4 }, { 1, 4 }, { 0, 5 }, { 1, 5 }, { 10, 4 }, { 10, 5 }, { 10, 6 },
            { 11, 3 }, { 11, 7 }, { 12, 2 }, { 12, 8 }, { 13, 2 }, { 13, 8 }, { 14, 5 },
            { 15, 3 }, { 15, 7 }, { 16, 4 }, { 16, 5 }, { 16, 6 }, { 17, 5 }, { 20, 2 },
            { 20, 3 }, { 20, 4 }, { 21, 2 }, { 21, 3 }, { 21, 4 }, { 22, 1 }, { 22, 5 },
            { 24, 0 }, { 24, 1 }, { 24, 5 }, { 24, 6 }, { 34, 2 }, { 34, 3 }, { 35, 2 },
            { 35, 3 }
    };

    // Colocar el patrón de la pistola en posiciones aleatorias en el framebuffer
    for (int i = 0; i < 20; i++) {  // Incrementar el número de pistolas a 20
        int x = std::rand() % RENDER_WIDTH;
        int y = std::rand() % RENDER_HEIGHT;

        for (const auto& point : gunPattern) {
            int gunX = point.first + x;
            int gunY = point.second + y;
            setPixel(gunX, gunY, aliveColor);
        }
    }

    // Definir el patrón inicial - Glider
    std::vector<std::pair<int, int>> smallGliderPattern = {
            { 0, 0 }, { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 2 }
    };

    // Colocar el patrón pequeño de la nave espacial en posiciones aleatorias en el framebuffer
    for (int i = 0; i < 50; i++) {  // Incrementar el número de naves pequeñas a 50
        int x = std::rand() % RENDER_WIDTH;
        int y = std::rand() % RENDER_HEIGHT;

        for (const auto& point : smallGliderPattern) {
            int gliderX = point.first + x;
            int gliderY = point.second + y;
            setPixel(gliderX, gliderY, aliveColor);
        }
    }
}

int main(int argc, char* args[]) {
    // Inicializar SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Crear una ventana
    SDL_Window* window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    // Obtener el renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Semilla para la generación de números aleatorios (para experimentar con diferentes patrones iniciales)
    std::srand(std::time(nullptr));

    // Inicializar el juego de la vida
    initializeGameOfLife();

    // Bucle principal
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        // Manejar eventos
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Actualizar el juego de la vida
        updateGameOfLife();

        // Renderizar el framebuffer en la ventana
        renderBuffer(renderer);

        // Delay para visualizar mejor la animación (ajusta la velocidad según tus preferencias)
        SDL_Delay(100);
    }

    // Liberar recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
