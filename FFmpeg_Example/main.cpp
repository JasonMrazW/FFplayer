//
//  main.cpp
//  FFmpeg_Example
//
//  Created by wangbo on 2020/11/17.
//

#include <iostream>

#include "Player.hpp"
#include "FlvPlayer.hpp"

int main(int argc, const char * argv[]) {
    
   const char *filePath = "./video/ret.flv";
    //char *filePath = "./video/bigbuckbunny_480x272.h265";
    FlvPlayer flvPlayer;
    flvPlayer.init(filePath);
//    //SDL Player
    Player player;
//    player.init(filePath);
//    player.start();
    return 0;
}
