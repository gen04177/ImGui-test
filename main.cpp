#include <SDL2/SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include <cmath>

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
#define IM_PI 3.14159265358979323846f // Definition of IM_PI

bool button1Pressed = false;
bool button2Pressed = false;

void RenderVisualExperiments()
{
    ImGui::Begin("Visual Experiments", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

    static float rotation = 0.0f;
    static float scale = 1.0f;
    rotation += 0.5f;
    scale = 1.0f + 0.2f * sinf(rotation * 0.1f);

    ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Visual Experiments");
    ImGui::Separator();

    static float values[90] = {0.0f};
    static int offset = 0;
    static float time = 0.0f;
    time += 0.1f;
    values[offset] = sinf(time);
    offset = (offset + 1) % IM_ARRAYSIZE(values);
    ImGui::PlotLines("Dynamic Graph", values, IM_ARRAYSIZE(values), offset, nullptr, -1.0f, 1.0f, ImVec2(0, 80));

    ImVec2 center = ImGui::GetCursorScreenPos();
    float radius = 100.0f;
    ImVec2 points[6];
    for (int i = 0; i < 6; ++i)
    {
        float angle = i * (2.0f * IM_PI / 6.0f) + rotation;
        points[i] = ImVec2(center.x + radius * cosf(angle), center.y + radius * sinf(angle));
    }
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddConvexPolyFilled(points, 6, IM_COL32(255, 100, 100, 255));

    if (button1Pressed)
    {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "You pressed Button Cross!");
    }
    if (button2Pressed)
    {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "You pressed Button Circle!");
    }

    ImGui::End();
}

void printHelp(void)
{
    printf("Example program for ImGui.\n\n");

    printf("\nby @gen04177\n");

}

int main(int, char**)
{

    printHelp();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("ImGui Visual Experiments", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
    if (window == nullptr)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == nullptr)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_GameController* controller = SDL_GameControllerOpen(0);
    if (controller == nullptr)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }

        button1Pressed = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
        button2Pressed = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B);

        ImGui_ImplSDL2_NewFrame();
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui::NewFrame();

        RenderVisualExperiments();

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDL2_Shutdown();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui::DestroyContext();

    SDL_GameControllerClose(controller);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}