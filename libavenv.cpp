#include "libavenv.h"
#include <string>
#include <sstream>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/log.h>
}

//
LibAVEnv::LibAVEnv()
    : inputFmtCtx(nullptr), videoInstreamIndex(-1), audioInstreamIndex(-1),
      inputAudioCodecCtx(nullptr), inputVideoCodecCtx(nullptr),
      inputAudioCodec(nullptr), inputVideoCodec(nullptr),
      onePacket(nullptr), oneFrame(nullptr), yoloFrame(nullptr),
      swsCtx(nullptr)
{
}

LibAVEnv::~LibAVEnv()
{
    Cleanup();
}

RunError LibAVEnv::InitLibAv()
{
    return InitPacket();
}

RunError LibAVEnv::InitInputCodec(const std::string &inputUrl)
{
    std::cout << "InitInputCodec, inputUrl=" << inputUrl.c_str() << std::endl;
    int ret = 0;
    char error_buffer[128];
    std::stringstream ss;
    ret = avformat_open_input(&inputFmtCtx, inputUrl.c_str(), NULL, NULL);
    if (ret < 0)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "avformat_open_input inputFmtCtx:" << error_buffer;
        std::cout << ss.str() << std::endl;
        return RunError(ret, ss.str());
    }
    ret = avformat_find_stream_info(inputFmtCtx, NULL);
    if (ret < 0)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "avformat_find_stream_info inputFmtCtx:" << error_buffer;
        std::cout << ss.str() << std::endl;
        return RunError(ret, ss.str());
    }
    for (int i = 0; i < inputFmtCtx->nb_streams; i++)
    {
        if (inputFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoInstreamIndex = i;
        }
        if (inputFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            audioInstreamIndex = i;
        }
    }
    if (videoInstreamIndex < 0)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "videoInstreamIndex < 0:" << error_buffer;
        std::cout << ss.str() << std::endl;
        return RunError(ret, ss.str());
    }
    if (audioInstreamIndex < 0)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "audioInstreamIndex < 0:" << error_buffer;
        std::cout << ss.str() << std::endl;
        return RunError(ret, ss.str());
    }
    std::cout << "3" << std ::endl;
    AVCodecParameters *audioCodecpar = inputFmtCtx->streams[audioInstreamIndex]->codecpar;
    inputAudioCodec = avcodec_find_decoder(audioCodecpar->codec_id);
    if (!inputAudioCodec)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "avcodec_find_decoder audioCodecpar:" << error_buffer;
        std::cout << ss.str() << std::endl;
        return RunError(ret, ss.str());
    }
    AVCodecParameters *videoCodecpar = inputFmtCtx->streams[videoInstreamIndex]->codecpar;
    inputVideoCodec = avcodec_find_decoder(videoCodecpar->codec_id);
    if (!inputVideoCodec)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "avcodec_find_decoder videoCodecpar:" << error_buffer;
        std::cout << ss.str() << std::endl;
        return RunError(ret, ss.str());
    }
    inputAudioCodecCtx = avcodec_alloc_context3(inputAudioCodec);
    if (!inputAudioCodecCtx)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "avcodec_alloc_context3 inputAudioCodec:" << error_buffer;
        std::cout << ss.str() << std::endl;
        return RunError(ret, ss.str());
    }
    inputVideoCodecCtx = avcodec_alloc_context3(inputVideoCodec);
    if (!inputVideoCodecCtx)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "avcodec_alloc_context3 inputVideoCodec:" << error_buffer;
        std::cout << ss.str() << std::endl;
        return RunError(ret, ss.str());
    }
    ret = avcodec_parameters_to_context(inputAudioCodecCtx, audioCodecpar);
    if (ret < 0)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "avcodec_parameters_to_context inputAudioCodecCtx audioCodecpar:" << error_buffer;
        std::cout << ss.str() << std::endl;
        return RunError(ret, ss.str());
    }
    ret = avcodec_parameters_to_context(inputVideoCodecCtx, videoCodecpar);
    if (ret < 0)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "avcodec_parameters_to_context inputVideoCodecCtx videoCodecpar:" << error_buffer;
        std::cout << ss.str() << std::endl;
        return RunError(ret, ss.str());
    }
    ret = avcodec_open2(inputAudioCodecCtx, inputAudioCodec, NULL);
    if (ret < 0)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "avcodec_open2 inputAudioCodecCtx inputAudioCodec:" << error_buffer;
        std::cout << ss.str() << std::endl;
        return RunError(ret, ss.str());
    }
    ret = avcodec_open2(inputVideoCodecCtx, inputVideoCodec, NULL);
    if (ret < 0)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "avcodec_open2 inputVideoCodecCtx inputVideoCodec:" << error_buffer;
        std::cout << ss.str() << std::endl;
        return RunError(ret, ss.str());
    }

    return RunError();
}

RunError LibAVEnv::InitPacket()
{
    int ret = 0;
    char error_buffer[128];
    std::stringstream ss;
    oneFrame = av_frame_alloc();

    if (!oneFrame)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "av_frame_alloc oneFrame:" << error_buffer;
        return RunError(ret, ss.str());
    }
    yoloFrame = av_frame_alloc();
    if (!yoloFrame)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "av_frame_alloc yoloFrame:" << error_buffer;
        return RunError(ret, ss.str());
    }
    onePacket = av_packet_alloc();
    if (!onePacket)
    {
        av_strerror(ret, error_buffer, sizeof(error_buffer));
        ss << __FILE__ << ":" << __LINE__ << "; " << "av_frame_alloc onePacket:" << error_buffer;
        return RunError(ret, ss.str());
    }
    return RunError();
}

RunError LibAVEnv::InitSwSCtx()
{
    int w = 640, h = 640;
    int frame_size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, w, h, 1);
    swsCtx = sws_getContext(
        inputVideoCodecCtx->width,
        inputVideoCodecCtx->height,
        inputVideoCodecCtx->pix_fmt,
        w, h, AV_PIX_FMT_RGB24,
        SWS_BILINEAR, NULL, NULL, NULL);

    uint8_t *rgb_buffer = (uint8_t *)av_malloc(frame_size);
    av_image_fill_arrays(yoloFrame->data, yoloFrame->linesize,
                         rgb_buffer, AV_PIX_FMT_RGB24, w, h, 1);
    return RunError();
}

void LibAVEnv::StartStream(FrameHandler handler)
{
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;

    while (av_read_frame(inputFmtCtx, onePacket) >= 0)
    {
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;

        if (onePacket->stream_index == audioInstreamIndex)
        {
            continue;
        }
        if (onePacket->stream_index == videoInstreamIndex)
        {
            if (avcodec_send_packet(inputVideoCodecCtx, onePacket) < 0)
            {
                fprintf(stderr, "Error avcodec_send_packet(inputVideoCodecCtx, onePacket)\n");
                av_packet_unref(onePacket);
                continue;
            }

            while (avcodec_receive_frame(inputVideoCodecCtx, oneFrame) >= 0)
            {
                std::cout << "avcodec_receive_frame.pts: " << std::endl;
                handler(oneFrame);
            }
        }
        av_packet_unref(onePacket);
    }
}

RunError LibAVEnv::Stop()
{
    Cleanup();
    return RunError();
}

void LibAVEnv::Cleanup()
{
    av_frame_free(&oneFrame);
    av_frame_free(&yoloFrame);
    av_packet_free(&onePacket);
    avformat_close_input(&inputFmtCtx);
    avcodec_free_context(&inputAudioCodecCtx);
    avcodec_free_context(&inputVideoCodecCtx);
}
