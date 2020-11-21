//
//  FlvPlayer.cpp
//  FFmpeg_Example
//
//  Created by wangbo on 2020/11/21.
//

#include "FlvPlayer.hpp"

using namespace std;

FlvPlayer::FlvPlayer()
{
    
}

FlvPlayer::~FlvPlayer()
{
    
}

void FlvPlayer::init(const char *url)
{
    //1. init format
    AVFormatContext *pFormatContext = avformat_alloc_context();
    
    
    int ret = avformat_open_input(&pFormatContext, url, NULL, NULL);
    
    if (ret < 0) {
        printf("could't open input file: %s", url);
        return;
    }
    
    printf("open file success.");
    
    av_dump_format(pFormatContext, 0, url, 0);
    
    //2. init stream info
    if(avformat_find_stream_info(pFormatContext, NULL) < 0)
    {
        printf("could't find stream info.");
        return;
    }
    
    printf("find %d stream info.",pFormatContext->nb_streams);
    int video_index = -1;
    for (int i = 0;i < pFormatContext->nb_streams;i++) {
        if (pFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_index = i;
            break;
        }
    }
    
    if (video_index < 0) {
        printf("find video stream failed.");
        return;
    }
    
    AVStream *videoStream = pFormatContext->streams[video_index];
    AVCodec *videoCodec = avcodec_find_decoder(videoStream->codecpar->codec_id);
    
    if (videoCodec == NULL) {
        cout << "could't find video codec by id：" << videoStream->codecpar->codec_id;
        return;
    }
    
    
    AVCodecContext *codecContext = avcodec_alloc_context3(videoCodec);
    avcodec_parameters_to_context(codecContext, videoStream->codecpar);
    
    if (avcodec_open2(codecContext, videoCodec, NULL) < 0) {
        cout << "open codec failed." << endl;
        return;
    }
    cout << "bit_rate:" << codecContext->bit_rate << endl;
    
    AVFrame *avFrame = av_frame_alloc();
    AVPacket *avPacket = av_packet_alloc();
    
    while (av_read_frame(pFormatContext, avPacket) >= 0) {
        cout << "av_read_frame" << endl;
        if (avPacket->stream_index != video_index) {
            continue;
        }
        ret = avcodec_send_packet(codecContext, avPacket);
        av_packet_unref(avPacket);
        if (ret < 0) {
            cout << "send packet failed." << endl;
            return;
        }
        
        while(ret >= 0) {
            ret = avcodec_receive_frame(codecContext, avFrame);
            if (ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                cout << "decode frame end. "<< av_err2str(ret) <<endl;
                continue;
            }
            cout << "pts：" << avFrame->key_frame << endl;
        }
    }
}
