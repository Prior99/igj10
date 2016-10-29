#include "game.hpp"

#include "main_state.hpp"
#include "game_config.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <iostream>

Game::~Game() {
    m_res_manager.shutdown();
    TTF_Quit();
    SDL_CloseAudio();
    SDL_DestroyRenderer(m_render);
    SDL_DestroyWindow(m_window);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int Game::init() {
	this->freeze = false;
    this->player = m_ex.entities.create();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    m_window = SDL_CreateWindow("Innojam #10", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    m_render =
        SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (m_render == nullptr) {
        SDL_DestroyWindow(m_window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError()
                  << std::endl;
        return 1;
    }

    if (TTF_Init() != 0) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    m_res_manager.load_texture("gradient", "res/gradient.png", m_render);
    m_res_manager.load_texture("playerimg", "res/0.png", m_render);
    m_res_manager.load_texture("player-small", "res/player-small.png", m_render);
    m_res_manager.load_texture("player", "res/player.png", m_render);
    m_res_manager.load_texture("house", "res/house-1.png", m_render);
    m_res_manager.load_texture("background", "res/background.png", m_render);
    m_res_manager.load_font("font20", "res/DejaVuSans.ttf", 20);

    SDL_RenderSetLogicalSize(m_render, WIDTH, HEIGHT);

    entityx::Entity entity = m_ex.entities.create();
    entity.assign<Position>();

    m_states.push({"main", std::make_unique<MainState>(this)});
    m_states.top().second->init();

    return 0;
}

void Game::toggleFreeze() {
    this->freeze = !this->freeze;
    if (!this->freeze) {
        this->setInsanity(1.0f);
    }
}


entityx::Entity Game::getPlayer() {
    return this->player;
}

void Game::mainloop() {
    int current_time = SDL_GetTicks();
    double dt = (current_time - m_last_frame_time) / 1000.0;
    m_last_frame_time = current_time;

    m_states.top().second->update(dt);
}

bool Game::isFrozen() {
	return this->freeze;
}

SDL_Renderer *Game::renderer() {
    return m_render;
}

SDL_Window *Game::window() {
    return m_window;
}

ResourceManager &Game::res_manager() {
    return m_res_manager;
}

std::stack<std::pair<std::string, std::unique_ptr<State>>> &Game::states() {
    return m_states;
}

bool Game::is_running() {
    return m_running;
}

float Game::getInsanity() {
    return insanity;
}

void Game::setInsanity(float f) {
    this->insanity = f;
}

void Game::addInsanity(float f) {
    this->insanity += f;
}

void Game::shutdown() {
    m_running = false;
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#endif
}

void Game::popstate() {
    m_states.pop();
}

const std::string &Game::statename() const {
    return m_states.top().first;
}
