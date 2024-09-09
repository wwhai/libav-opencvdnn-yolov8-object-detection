#ifndef LIBAVENV_H
#define LIBAVENV_H

#include "runerror.h"
#include <string>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/log.h>
}
typedef void (*FrameHandler)(AVFrame *frame);

class LibAVEnv
{
private:
    AVFormatContext *inputFmtCtx;
    int videoInstreamIndex;
    int audioInstreamIndex;

    AVCodecContext *inputAudioCodecCtx;
    AVCodecContext *inputVideoCodecCtx;

    const AVCodec *inputAudioCodec;
    const AVCodec *inputVideoCodec;

    AVPacket *onePacket;
    AVFrame *oneFrame;
    AVFrame *yoloFrame;

    SwsContext *swsCtx;

private:
    RunError InitPacket();

public:
    LibAVEnv();
    ~LibAVEnv();

    RunError InitLibAv();
    RunError InitInputCodec(const std::string &inputUrl);
    RunError InitSwSCtx();
    void StartStream(FrameHandler handler);
    RunError Stop();

private:
    void Cleanup();
};

#endif // LIBAVENV_H
