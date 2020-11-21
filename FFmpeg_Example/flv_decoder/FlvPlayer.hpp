//
//  FlvPlayer.hpp
//  FFmpeg_Example
//
//  Created by wangbo on 2020/11/21.
//

#ifndef FlvPlayer_hpp
#define FlvPlayer_hpp

#include <stdio.h>
extern "C"{
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
}

#include <iostream>

class FlvPlayer{
public:
    FlvPlayer();
    ~FlvPlayer();
    
    void init(const char *url);
};
#endif /* FlvPlayer_hpp */
