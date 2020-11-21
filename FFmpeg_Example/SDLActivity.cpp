//
//  SDLActivity.cpp
//  FFmpeg_Example
//
//  Created by wangbo on 2020/11/18.
//

#include "SDLActivity.hpp"

SDLActivity::SDLActivity(int w, int h):width(w),height(h) {
    initSDL();
}

SDLActivity::~SDLActivity()
{
    onCleanUp();
}

int SDLActivity::onExecute()
{
    int ret = initSDL();
    if (ret == FAIL) {
        printf("init SDL failed.");
        return ret;
    }
    
    //SDL event
    SDL_Event event;
    
    running = true;
    
    while (running) {
        if (SDL_PollEvent(&event)) {
            OnEvent(&event);
        }
        std::tuple<uint8_t** ,int*> tuple = listener->onRenderStart();
        if (std::get<0>(tuple) == NULL) {
            break;
        }
        onUpdate(std::get<0>(tuple), std::get<1>(tuple));
        onRender();
    }
    
    return OK;
}


int SDLActivity::initSDL()
{
    //2. init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("could not init SDL. %s", SDL_GetError());
        return FAIL;
    }
    printf("init SDL success.\r\n");
    
    //3. create window
    int screen_width = width;
    int screen_height = height;
    window = SDL_CreateWindow("bo player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width*2, screen_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("create window failed.");
        return FAIL;
    }
    
    //4. create render
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, screen_width, screen_height);
    
    rect = SDL_Rect();
    rect.x = 0;
    rect.y = 0;
    rect.h = screen_height;
    rect.w = screen_width;
    
    rect2 = SDL_Rect();
    rect2.x = screen_width;
    rect2.y = 0;
    rect2.h = screen_height;
    rect2.w = screen_width;
    
    return OK;
}

void SDLActivity::OnEvent(SDL_Event *event)
{
    switch (event->type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
            break;
        default:
            break;
    }
}

//update logic
void SDLActivity::onUpdate(uint8_t *data[], int* linesize)
{
    SDL_UpdateYUVTexture(texture, &rect, data[0], linesize[0],
                         data[1], linesize[1],
                         data[2], linesize[2]);
}

void SDLActivity::onRender()
{
    SDL_RenderClear(render);
    SDL_RenderCopy(render, texture, NULL, &rect);
    SDL_RenderCopy(render, texture, NULL, &rect2);
    SDL_RenderPresent(render);
    
    // fps control
    SDL_Delay(30);
}

void SDLActivity::onCleanUp()
{
    if (render != NULL) {
        SDL_DestroyRenderer(render);
    }
    
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
    }
}

