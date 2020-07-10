// pcmplayer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "winmm.lib")

#define DATASIZE 1024*10 //分次截取数据大小
FILE*			pcmfile;  //音频文件
HWAVEOUT        hwo;

void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwInstance, DWORD dw1, DWORD dw2)//回调函数
{
	switch (uMsg)
	{
	case WOM_DONE://上次缓存播放完成,触发该事件
		{
			LPWAVEHDR pWaveHeader = (LPWAVEHDR)dw1;
			pWaveHeader->dwBufferLength = fread(pWaveHeader->lpData, 1, DATASIZE, pcmfile);
			waveOutPrepareHeader(hwo, pWaveHeader, sizeof(WAVEHDR));
			waveOutWrite(hwo, pWaveHeader, sizeof(WAVEHDR));
			break;
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	WAVEHDR         wh1;
	WAVEHDR         wh2;
	WAVEFORMATEX    wfx;

	fopen_s(&pcmfile, "xiaoqingge.pcm", "rb");//打开文件

	wfx.wFormatTag = WAVE_FORMAT_PCM;//设置波形声音的格式
	wfx.nChannels = 2;//设置音频文件的通道数量
	//wfx.nSamplesPerSec = 48000;//设置每个声道播放和记录时的样本频率,看具体文件，一般44100
	wfx.nSamplesPerSec = 44100;//设置每个声道播放和记录时的样本频率,看具体文件，一般44100
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
	wfx.cbSize = 0;//额外信息的大小

	//打开一个给定的波形音频输出装置来进行声音播放，方式为回调函数方式。如果是对话框程序，可以将第五个参数改为(DWORD)this，操作跟本Demo程序相似
	int ret =  waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD)WaveCallback, 2L, CALLBACK_FUNCTION);
	if( ret != MMSYSERR_NOERROR)
	{
		return -1;
	}

	wh1.dwLoops = 0L;//播放区一
	wh1.lpData = new char[DATASIZE];
	wh1.dwBufferLength = DATASIZE; 
	fread(wh1.lpData, 1, DATASIZE, pcmfile);
	wh1.dwFlags = 0L;
	waveOutPrepareHeader(hwo, &wh1, sizeof(WAVEHDR));//准备一个波形数据块用于播放
	waveOutWrite(hwo, &wh1, sizeof(WAVEHDR));//在音频媒体中播放第二个参数指定的数据，也相当于开启一个播放区的意思

	wh2.dwLoops = 0L;//播放区二，基本同上
	wh2.lpData = new char[DATASIZE];
	wh2.dwBufferLength = DATASIZE;
	fread(wh2.lpData, 1, DATASIZE, pcmfile);
	wh2.dwFlags = 0L;
	waveOutPrepareHeader(hwo, &wh2, sizeof(WAVEHDR));
	waveOutWrite(hwo, &wh2, sizeof(WAVEHDR));

	while (wh1.dwBufferLength != 0 || wh2.dwBufferLength != 0)//如果文件还在没播放完则等
	{
		Sleep(10);
	}
	waveOutUnprepareHeader(hwo, &wh1, sizeof(WAVEHDR));//清理数据
	waveOutUnprepareHeader(hwo, &wh2, sizeof(WAVEHDR));
	waveOutClose(hwo);

	delete []wh1.lpData;
	delete []wh2.lpData;
	fclose(pcmfile);//关闭文件
	return 0;
}

