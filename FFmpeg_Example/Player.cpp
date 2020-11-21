//
//  Player.cpp
//  FFmpeg_Example
//
//  Created by wangbo on 2020/11/17.
//

#include "Player.hpp"

Player::Player()
{
    
}

Player::~Player()
{
    //onCleanUp();
}

//init ffmpeg
void Player::init(const char *filePath)
{
    //1. av register
    av_register_all();
    
    //2. init format context
    formatCtx = avformat_alloc_context();
    
    //3. open file
    
    int result = avformat_open_input(&formatCtx, filePath, NULL, NULL);
    if (result != 0) {
        printf("Could't open input file %s && %d", filePath, result);
        return;
    }
    
    printf("open file success.\r\n");
    
    //4. find stream info
    if (avformat_find_stream_info(formatCtx, NULL) < 0) {
        printf("Could't find stream info");
        return;
    }
    AVDictionaryEntry *m = NULL;
    m=av_dict_get(formatCtx->metadata,"author",m,0);
    
    //printf("metedata %s \r\n", m->key);
    
    // 5. find decoder
    videoIndex = -1;
    for (int i = 0; i < formatCtx->nb_streams; i++) {
        if (formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            break;
        }
    }
    
    if (videoIndex == -1) {
        printf("Could't find video info.");
        return;
    }
    
    codecCtx = formatCtx->streams[videoIndex]->codec;
    AVCodec *codec = avcodec_find_decoder(codecCtx->codec_id);
    if (codec == NULL) {
        printf("cannot find codec.");
        return;
    }
    
    printf("find codec %s success.\r\n", codec->name);
    // 6. open codec
    if (avcodec_open2(codecCtx, codec, NULL) < 0) {
        printf("cannot open codec.");
        return;
    }
    
    printf("open codec success.");
    
    //7. read frame
    av_dump_format(formatCtx, 0, filePath, 0);
    
    //8. SWS
    frameYUV = av_frame_alloc();
    unsigned char *outputBuffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codecCtx->width, codecCtx->height, 1));
    av_image_fill_arrays(frameYUV->data, frameYUV->linesize, outputBuffer, AV_PIX_FMT_YUV420P, codecCtx->width, codecCtx->height, 1);
 
    image_sws_context = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt, codecCtx->width, codecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL,NULL,NULL);
    
    frame = av_frame_alloc();
    packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    
    //init SDL
    activity = new SDLActivity(codecCtx->width, codecCtx->height);
    //activity->readFrame = a;
    activity->listener = this;
}

void Player::start()
{
    activity->onExecute();
}

std::tuple<uint8_t** ,int*>  Player::onRenderStart()
{
    return readFrame();
}


std::tuple<uint8_t** ,int*> Player::readFrame()
{
    //1. open output file
    int get_pic = -1;
    int ret = -1;
    std::tuple<uint8_t** ,int*> result;
    while (true) {
        if (av_read_frame(formatCtx, packet) < 0) {
            //running = false;
            break;
        }
        if (packet->stream_index == videoIndex) {
//            ret = avcodec_decode_video2(codecCtx, frame, &get_pic, packet);
            ret = avcodec_send_packet(codecCtx, packet);
            if (ret < 0) {
                printf("decode failed.\r\n");
                return result;
            }
            while (ret >= 0) {
                ret = avcodec_receive_frame(codecCtx, frame);
                if (ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    //printf("decode frame ended. %s\r\n", av_err2str(ret));
                    break;
                }
                sws_scale(image_sws_context, frame->data, frame->linesize, 0, codecCtx->height, frameYUV->data, frameYUV->linesize);
                result = std::make_tuple(frameYUV->data, frameYUV->linesize);
                return result;
            }
//            if (get_pic >= 0) {
//                sws_scale(image_sws_context, frame->data, frame->linesize, 0, codecCtx->height, frameYUV->data, frameYUV->linesize);
//                result = std::make_tuple(frameYUV->data, frameYUV->linesize);
//                printf("%d\r\n", frame->pts);
//                //printf("%d", frame->pict_type);
//            } else
//            {
//                printf("get pic failed %d.\r\n", get_pic);
//            }
            //break;
        }
        av_free_packet(packet);
    }
    
    return result;
}


void Player::onCleanUp()
{
    sws_freeContext(image_sws_context);
    //fclose(yuv_file);
    av_frame_free(&frame);
    av_frame_free(&frameYUV);
    avcodec_close(codecCtx);
    avformat_close_input(&formatCtx);
}
