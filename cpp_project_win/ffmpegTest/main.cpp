extern "C" {
#include "libavcodec\avcodec.h"
#include "libavformat\avformat.h"
#include "libswscale\swscale.h"
#include "libavutil\imgutils.h"
#include "SDL.h"
}

#include "output_log.h"

#undef main
#define SFM_REFRESH_EVENT (SDL_USEREVENT + 1)//sdl的用户自定义事件
#define SFM_BREAK_EVENT (SDL_USEREVENT + 2)

static bool g_sfp_refresh_thread_exit = 0;
static bool g_sfp_refresh_thread_pause = 0;
static int g_FPS = 1;//FPS
static Uint32 g_FPS_timer = SDL_GetTicks();//记录两帧之间的刷新间隔

//子线程，事件发生器
int sfp_refresh_thread(void *data) {
	//标记初始化
	g_sfp_refresh_thread_exit = 0;
	g_sfp_refresh_thread_pause = 0;
	Uint32 fpsDuration = 1000 / g_FPS;//帧间间隔时间，使用整型会使得时间间隔略低，即刷新率略高
	while (!g_sfp_refresh_thread_exit) {
		if (!g_sfp_refresh_thread_pause) {
			SDL_Event sdlEvent = { 0 };
			sdlEvent.type = SFM_REFRESH_EVENT;
			SDL_PushEvent(&sdlEvent);
		}
		//计算每帧之间的时间间隔，与帧率对应时间比较，判断是否要延迟，以及延迟多久
		Uint32 oneDuration = SDL_GetTicks() - g_FPS_timer;
		if (oneDuration < fpsDuration) {
			SDL_Delay(fpsDuration - oneDuration);
		}
		g_FPS_timer = SDL_GetTicks();
	}
	//标记复位
	g_sfp_refresh_thread_exit = 0;
	g_sfp_refresh_thread_pause = 0;
	SDL_Event sdlEvent = { 0 };
	sdlEvent.type = SFM_BREAK_EVENT;	
	return SDL_PushEvent(&sdlEvent);
}

int vPlayer_sdl2(char* filePath) {
	int ret = 0;

	//ffmpeg param
	AVFormatContext* pFormatCtx = nullptr;
	AVCodecContext* pCodecCtx = nullptr;
	const AVCodec* pCodec = nullptr;
	AVPacket* packet = nullptr;
	AVFrame* pFrame = nullptr, * pFrameYUV = nullptr;
	SwsContext* pSwsCtx = nullptr;
	unsigned char* outBuff = nullptr;
	int outBuffSize = 0;
	int videoIdx = 0;//视频通道所在的下标

	//sdl param
	SDL_Window* pSDLWindow = nullptr;
	SDL_Renderer* pSDLRender = nullptr;
	SDL_Texture* pSDLTexture = nullptr;
	SDL_Rect sdlRect = { 0 };
	SDL_Thread* pSDLThread = nullptr;
	SDL_Event sdlEvent = { 0 };

	//init ffmpeg，分配空间
	pFormatCtx = avformat_alloc_context();
	packet = av_packet_alloc();
	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();

	//init network
	avformat_network_init();

	//open input stream
	if (0 != avformat_open_input(&pFormatCtx, filePath, nullptr, nullptr)) {
		output_log(LOG_ERROR, "avformat_open_input error");
		ret = -1;
		goto end;
	}

	//find stream info
	if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
		output_log(LOG_ERROR, "avformat_find_stream_info error");
		ret = -1;
		goto end;
	}

	//get video codec
	for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
		AVStream* pStream = pFormatCtx->streams[i];
		if (pStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			pCodec = avcodec_find_decoder(pStream->codecpar->codec_id);
			pCodecCtx = avcodec_alloc_context3(pCodec);//avcodec_find_decoder_by_name
			avcodec_parameters_to_context(pCodecCtx, pStream->codecpar);
			videoIdx = i;
			//帧率，解码、刷新画面的频率
			g_FPS = pStream->avg_frame_rate.num / pStream->avg_frame_rate.den;
		}
	}
	if (!pCodecCtx) {
		output_log(LOG_ERROR, "pCodecCtx empty");
		ret = -1;
		goto end;
	}

	//open codec
	if (avcodec_open2(pCodecCtx, pCodec, nullptr)) {
		output_log(LOG_ERROR, "pCodecCtx empty");
		ret = -1;
		goto end;
	}
	av_dump_format(pFormatCtx, videoIdx, filePath, 0);//打印视频格式信息

	//转换视频帧数据格式
	pSwsCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
		pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
		AV_PIX_FMT_YUV420P, SWS_BICUBIC/*缩放算法-双三次线性插值*/, nullptr, nullptr, nullptr);

	outBuffSize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P,
		pCodecCtx->width, pCodecCtx->height, 1);
	outBuff = (unsigned char*)av_malloc(outBuffSize);
	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize,
		outBuff, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, 1);

	//init sdl
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
		output_log(LOG_ERROR, "SDL_Init error");
		ret = -1;
		goto end;
	}

	//create window
	pSDLWindow = SDL_CreateWindow("video window",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		pCodecCtx->width, pCodecCtx->height, SDL_WINDOW_OPENGL);
	//create render & texture
	pSDLRender = SDL_CreateRenderer(pSDLWindow, -1, 0);
	pSDLTexture = SDL_CreateTexture(pSDLRender, 
		SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
		pCodecCtx->width, pCodecCtx->height);
	sdlRect.x = 0;
	sdlRect.y = 0;
	sdlRect.w = pCodecCtx->width;
	sdlRect.h = pCodecCtx->height;

	//create thread，创建刷新线程
	pSDLThread = SDL_CreateThread(sfp_refresh_thread, nullptr, nullptr);

	//show，事件循环，展示视频、音频
	while (true) {
		int temp_ret = 0;
		SDL_WaitEvent(&sdlEvent);//阻塞等待事件发生，鼠标键盘事件、渲染事件
		if (SFM_BREAK_EVENT == sdlEvent.type) {
			break;
		}
		else if (SFM_REFRESH_EVENT == sdlEvent.type) {
			while (true) {
				if (av_read_frame(pFormatCtx, packet) < 0) {
					g_sfp_refresh_thread_exit = 1;
					break;
				}
				//读到视频帧就进行下一步
				if (packet->stream_index == videoIdx) {
					break;
				}
			}
			//把视频帧发给解码器
			if (0 != avcodec_send_packet(pCodecCtx, packet)) {
				g_sfp_refresh_thread_exit = 1;
			}

			//读取解码后数据，packet->frame
			do {
				temp_ret = avcodec_receive_frame(pCodecCtx, pFrame);
				if (AVERROR_EOF == temp_ret) {
					//文件读完了
					g_sfp_refresh_thread_exit = 1;
					break;
				}
				//使用前面设置好的视频帧格式进行转换
				if (0 == temp_ret) {
					sws_scale(pSwsCtx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
					SDL_UpdateTexture(pSDLTexture, &sdlRect, pFrameYUV->data[0], pFrameYUV->linesize[0]);
					SDL_RenderClear(pSDLRender);
					SDL_RenderCopy(pSDLRender, pSDLTexture, nullptr, &sdlRect);/*关注整个texture*/
					SDL_RenderPresent(pSDLRender);//draw call
				}
			}while(AVERROR(EAGAIN) != temp_ret);
		}
		else if (SDL_AUDIODEVICEADDED == sdlEvent.type) {
			output_log(LOG_INFO, "audio device added");
		}
		else if (SDL_KEYDOWN == sdlEvent.type) {
			if (SDLK_SPACE == sdlEvent.key.keysym.sym) {
				g_sfp_refresh_thread_pause = !g_sfp_refresh_thread_pause;
			}
			if (SDLK_ESCAPE == sdlEvent.key.keysym.sym) {
				g_sfp_refresh_thread_exit = 1;
			}
		}
		else if (SDL_QUIT == sdlEvent.type) {
			g_sfp_refresh_thread_exit = 1;
		}
		else {
			output_log(LOG_INFO, "event:0x%x", sdlEvent.type);
		}
	}								

	//release
end:
	sws_freeContext(pSwsCtx);

	avcodec_free_context(&pCodecCtx);

	avformat_close_input(&pFormatCtx);
	avformat_free_context(pFormatCtx);

	av_packet_free(&packet);
	av_frame_free(&pFrame);
	av_frame_free(&pFrameYUV);

	SDL_DestroyTexture(pSDLTexture);
	SDL_DestroyRenderer(pSDLRender);
	SDL_DestroyWindow(pSDLWindow);
	SDL_Quit();
	return ret;
}

int main(int argc, char* argv[]) {
	if (argc > 1)
	{
		vPlayer_sdl2(argv[1]);
	}

	return 0;
}


