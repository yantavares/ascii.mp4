#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <time.h>
#include <math.h>

const char *ASCII_CHARS = "@#W$21abc?!;:+=-,. ";

char brightness_to_ascii(unsigned char brightness)
{
    int index = (brightness * (strlen(ASCII_CHARS) - 1)) / 255;
    return ASCII_CHARS[index];
}

void apply_blur(uint8_t *frame, int width, int height, int linesize)
{
    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            int idx = y * linesize + x;
            frame[idx] = (frame[idx] +
                          frame[idx - 1] +
                          frame[idx + 1] +
                          frame[idx - linesize] +
                          frame[idx + linesize]) /
                         5;
        }
    }
}

void clear_screen()
{
    printf("\033[H\033[J");
}

void get_terminal_size(int *cols, int *rows)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0)
    {
        *cols = w.ws_col;
        *rows = w.ws_row;
    }
    else
    {
        fprintf(stderr, "Unable to detect terminal size. Defaulting to 80x24.\n");
        *cols = 80;
        *rows = 24;
    }
}

void detect_motion_and_display(const uint8_t *prev_frame, const uint8_t *current_frame, int width, int height, int linesize, int threshold, int debug_mode)
{
    if (debug_mode)
        return;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int current_index = y * linesize + x;
            int diff = abs(current_frame[current_index] - prev_frame[current_index]);
            if (diff > threshold)
            {
                printf("\033[48;2;%d;0;0m%c", diff, brightness_to_ascii(current_frame[current_index]));
            }
            else
            {
                printf("\033[48;2;0;0;0m%c", brightness_to_ascii(current_frame[current_index]));
            }
        }
        printf("\033[0m\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Uso: %s <caminho_do_arquivo> [--debug]\n", argv[0]);
        return 1;
    }

    const char *video_path = argv[1];
    int debug_mode = (argc > 2 && strcmp(argv[2], "--debug") == 0);

    AVFormatContext *format_ctx = NULL;
    if (avformat_open_input(&format_ctx, video_path, NULL, NULL) != 0)
    {
        fprintf(stderr, "Erro ao abrir o arquivo de vídeo: %s\n", video_path);
        return -1;
    }

    if (avformat_find_stream_info(format_ctx, NULL) < 0)
    {
        fprintf(stderr, "Erro ao obter informações do fluxo.\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    int video_stream_index = -1;
    for (unsigned int i = 0; i < format_ctx->nb_streams; i++)
    {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream_index = i;
            break;
        }
    }

    if (video_stream_index == -1)
    {
        fprintf(stderr, "Nenhum fluxo de vídeo encontrado.\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    AVCodecParameters *codec_params = format_ctx->streams[video_stream_index]->codecpar;
    const AVCodec *codec = avcodec_find_decoder(codec_params->codec_id);
    if (!codec)
    {
        fprintf(stderr, "Codec não suportado.\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx)
    {
        fprintf(stderr, "Erro ao alocar contexto de codec.\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    if (avcodec_parameters_to_context(codec_ctx, codec_params) < 0)
    {
        fprintf(stderr, "Erro ao copiar parâmetros de codec para o contexto.\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&format_ctx);
        return -1;
    }

    if (avcodec_open2(codec_ctx, codec, NULL) < 0)
    {
        fprintf(stderr, "Erro ao abrir o codec.\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&format_ctx);
        return -1;
    }

    int terminal_width, terminal_height;
    get_terminal_size(&terminal_width, &terminal_height);

    double character_aspect_ratio = 1.0;
    int output_width = terminal_width;
    int output_height = (int)(terminal_height * character_aspect_ratio);

    struct SwsContext *sws_ctx = sws_getContext(
        codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt,
        output_width, output_height, AV_PIX_FMT_GRAY8,
        SWS_BILINEAR, NULL, NULL, NULL);

    AVFrame *frame = av_frame_alloc();
    AVFrame *frame_gray = av_frame_alloc();
    uint8_t *prev_frame = (uint8_t *)malloc(output_width * output_height);
    memset(prev_frame, 0, output_width * output_height);

    int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_GRAY8, output_width, output_height, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(buffer_size);
    av_image_fill_arrays(frame_gray->data, frame_gray->linesize, buffer, AV_PIX_FMT_GRAY8, output_width, output_height, 1);

    AVPacket packet;
    clock_t start_time = clock();

    while (av_read_frame(format_ctx, &packet) >= 0)
    {
        if (packet.stream_index == video_stream_index)
        {
            if (avcodec_send_packet(codec_ctx, &packet) == 0)
            {
                while (avcodec_receive_frame(codec_ctx, frame) == 0)
                {
                    sws_scale(sws_ctx, (const uint8_t *const *)frame->data, frame->linesize, 0, codec_ctx->height, frame_gray->data, frame_gray->linesize);

                    if (!debug_mode)
                    {
                        clear_screen();
                    }

                    apply_blur(frame_gray->data[0], output_width, output_height, frame_gray->linesize[0]);
                    detect_motion_and_display(prev_frame, frame_gray->data[0], output_width, output_height, frame_gray->linesize[0], 20, debug_mode);

                    memcpy(prev_frame, frame_gray->data[0], output_width * output_height);

                    if (!debug_mode)
                    {
                        usleep(1000000 / codec_ctx->framerate.num);
                    }
                }
            }
        }
        av_packet_unref(&packet);
    }

    clock_t end_time = clock();

    if (debug_mode)
    {
        double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        printf("Tempo total de execução: %.2f segundos\n", duration);
    }

    free(prev_frame);
    av_free(buffer);
    av_frame_free(&frame);
    av_frame_free(&frame_gray);
    sws_freeContext(sws_ctx);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx);

    if (!debug_mode)
    {
        printf("----------------\n");
        printf("Vídeo concluído.\n");
    }

    return 0;
}
