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
    Mix_AllocateChannels(100);

    if (TTF_Init() != 0) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    // Stomper
    m_res_manager.load_texture("stomper-top", "res/stomper.png", m_render);
    m_res_manager.load_texture("stomper-middle", "res/stomper-worm.png", m_render);
    m_res_manager.load_texture("stomper-bottom", "res/stomper-head.png", m_render);
    // House 1
    m_res_manager.load_texture("house-01-bottom", "res/house-1/bottom.png", m_render);
    m_res_manager.load_texture("house-01-middle", "res/house-1/middle.png", m_render);
    m_res_manager.load_texture("house-01-roof", "res/house-1/roof.png", m_render);
    m_res_manager.load_texture("house-02", "res/hall.png", m_render);
    // Other
    m_res_manager.load_texture("gradient", "res/gradient.png", m_render);
    m_res_manager.load_texture("player", "res/player.png", m_render);
    m_res_manager.load_texture("sidewalk", "res/sidewalk.png", m_render);
    m_res_manager.load_texture("street", "res/street.png", m_render);
    m_res_manager.load_texture("saw", "res/Saw.png", m_render);
    m_res_manager.load_texture("background", "res/background.png", m_render);
    m_res_manager.load_texture("splatter", "res/Splatter.png", m_render);
    m_res_manager.load_texture("splash", "res/Splash.png", m_render);
    m_res_manager.load_texture("splatter-house", "res/SplatterHouse.png", m_render);
    // Fonts
    m_res_manager.load_font("font-big", "res/Munro.ttf", 20);
    m_res_manager.load_font("font-small", "res/Munro.ttf", 10);
    // Steps
    m_res_manager.load_sound("step-01", "res/sounds/Step1.wav");
    m_res_manager.load_sound("step-02", "res/sounds/Step2.wav");
    m_res_manager.load_sound("step-03", "res/sounds/Step3.wav");
    m_res_manager.load_sound("step-04", "res/sounds/Step4.wav");
    m_res_manager.load_sound("step-05", "res/sounds/Step5.wav");
    m_res_manager.load_sound("step-06", "res/sounds/Step6.wav");
    m_res_manager.load_sound("step-07", "res/sounds/Step7.wav");
    // Stomper
    m_res_manager.load_sound("stomper-down", "res/sounds/StomperDownNew.wav");
    m_res_manager.load_sound("stomper-lock", "res/sounds/StomperLockNew.wav");
    m_res_manager.load_sound("stomper-smash", "res/sounds/StomperSmashNew.wav");
    m_res_manager.load_sound("stomper-up", "res/sounds/StomperUpNew.wav");
    m_res_manager.load_sound("stomper-warning", "res/sounds/StomperWarningNew.wav");
    // Saw
    m_res_manager.load_sound("saw", "res/sounds/Saw.wav");
    m_res_manager.load_sound("saw-cutting", "res/sounds/SawCutting.wav");
    // Other
    m_res_manager.load_sound("splatter", "res/sounds/SplatterNew.wav");
    m_res_manager.load_sound("landing", "res/sounds/CrappyLanding.wav");
    m_res_manager.load_sound("heartbeat", "res/sounds/HeartBeatSoftLoop.wav");
    m_res_manager.load_music("chill-song", "res/sounds/ChillSongBeta.wav");
    Mix_VolumeMusic(30);
    Mix_PlayMusic(m_res_manager.music("chill-song"), -1);

    SDL_RenderSetLogicalSize(m_render, WIDTH, HEIGHT);

    entityx::Entity entity = m_ex.entities.create();
    entity.assign<Position>();

    m_states.push({"main", std::make_unique<MainState>(this)});
    m_states.top().second->init();

    return 0;
}

void Game::toggleFreeze() {
    this->freeze = !this->freeze;
    Mix_PauseMusic();
    if (!this->freeze) {
        this->setInsanity(1.0f);
        Mix_ResumeMusic();
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

void Game::mute() {
    Mix_PauseMusic();
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
