//
//  Player.hpp
//  FFmpeg_Example
//
//  Created by wangbo on 2020/11/17.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <string>
#include <tuple>
#include "SDLActivity.hpp"

using std::string;

extern "C"{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}


class Player: RenderListener{
public:
    Player();
    ~Player();
    
    void init();
    
    void start();
    
    virtual std::tuple<uint8_t **, int *> onRenderStart() override;
private:
    AVFormatContext *formatCtx;
    AVCodecContext *codecCtx;
    AVPacket *packet;
    int videoIndex = -1;
    AVFrame *frameYUV;
    AVFrame *frame;
    struct SwsContext *image_sws_context;
    
    //SDL
    SDLActivity *activity;
    
    
    std::tuple<uint8_t** ,int*> readFrame();
    
    void onCleanUp();
};
#endif /* Player_hpp */


