# FFplayer
基于FFmpeg的学习Demo

# 基础环境
SDL：2.0

FFmpeg：4.3.0

Xcode：12.2

# Demo
### 1. 基于SDL 2.0的简单播放器

#### 使用方式：
```
Player player;
player.init();
player.start();
```
#### 实现原理

ffmpeg：使用libavcodec和libavformat，进行解封装和解码

SDL：展示解码后AVFrame对应的数据

### 2. 待补充...
