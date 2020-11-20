//
//  SDLActivity.hpp
//  FFmpeg_Example
//
//  Created by wangbo on 2020/11/18.
//

#ifndef SDLActivity_hpp
#define SDLActivity_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <tuple>

class RenderListener{
public:
    virtual std::tuple<uint8_t** ,int*> onRenderStart() = 0;
};

class SDLActivity
{
public:
    SDLActivity(int w, int h);
    
    ~SDLActivity();

    int onExecute();
    
    RenderListener *listener;
private:
    enum APP_STATE{
        OK,FAIL
    };
    
    SDL_Window *window;
    SDL_Renderer *render;
    SDL_Texture *texture;
    SDL_Rect rect;
    SDL_Rect rect2;
    
    int width;
    
    int height;
    
    bool running;
    
    int initSDL();
    
    void OnEvent(SDL_Event* event);
    
    void onCleanUp();
    
    void onUpdate(uint8_t *data[], int* linesize);
    
    void onRender();
};


#endif /* SDLActivity_hpp */
