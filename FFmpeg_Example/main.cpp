//
//  main.cpp
//  FFmpeg_Example
//
//  Created by wangbo on 2020/11/17.
//

#include <iostream>

extern "C" {
#include <libavformat/avformat.h>
}
#include <SDL2/SDL.h>
#include "Player.hpp"

int main(int argc, const char * argv[]) {
    Player player;
    player.init();
    player.start();
    return 0;
}
