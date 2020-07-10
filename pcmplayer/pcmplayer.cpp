// pcmplayer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "winmm.lib")

#define DATASIZE 1024*10 //�ִν�ȡ���ݴ�С
FILE*			pcmfile;  //��Ƶ�ļ�
HWAVEOUT        hwo;

void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwInstance, DWORD dw1, DWORD dw2)//�ص�����
{
	switch (uMsg)
	{
	case WOM_DONE://�ϴλ��沥�����,�������¼�
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

	fopen_s(&pcmfile, "xiaoqingge.pcm", "rb");//���ļ�

	wfx.wFormatTag = WAVE_FORMAT_PCM;//���ò��������ĸ�ʽ
	wfx.nChannels = 2;//������Ƶ�ļ���ͨ������
	//wfx.nSamplesPerSec = 48000;//����ÿ���������źͼ�¼ʱ������Ƶ��,�������ļ���һ��44100
	wfx.nSamplesPerSec = 44100;//����ÿ���������źͼ�¼ʱ������Ƶ��,�������ļ���һ��44100
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
	wfx.cbSize = 0;//������Ϣ�Ĵ�С

	//��һ�������Ĳ�����Ƶ���װ���������������ţ���ʽΪ�ص�������ʽ������ǶԻ�����򣬿��Խ������������Ϊ(DWORD)this����������Demo��������
	int ret =  waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD)WaveCallback, 2L, CALLBACK_FUNCTION);
	if( ret != MMSYSERR_NOERROR)
	{
		return -1;
	}

	wh1.dwLoops = 0L;//������һ
	wh1.lpData = new char[DATASIZE];
	wh1.dwBufferLength = DATASIZE; 
	fread(wh1.lpData, 1, DATASIZE, pcmfile);
	wh1.dwFlags = 0L;
	waveOutPrepareHeader(hwo, &wh1, sizeof(WAVEHDR));//׼��һ���������ݿ����ڲ���
	waveOutWrite(hwo, &wh1, sizeof(WAVEHDR));//����Ƶý���в��ŵڶ�������ָ�������ݣ�Ҳ�൱�ڿ���һ������������˼

	wh2.dwLoops = 0L;//��������������ͬ��
	wh2.lpData = new char[DATASIZE];
	wh2.dwBufferLength = DATASIZE;
	fread(wh2.lpData, 1, DATASIZE, pcmfile);
	wh2.dwFlags = 0L;
	waveOutPrepareHeader(hwo, &wh2, sizeof(WAVEHDR));
	waveOutWrite(hwo, &wh2, sizeof(WAVEHDR));

	while (wh1.dwBufferLength != 0 || wh2.dwBufferLength != 0)//����ļ�����û���������
	{
		Sleep(10);
	}
	waveOutUnprepareHeader(hwo, &wh1, sizeof(WAVEHDR));//��������
	waveOutUnprepareHeader(hwo, &wh2, sizeof(WAVEHDR));
	waveOutClose(hwo);

	delete []wh1.lpData;
	delete []wh2.lpData;
	fclose(pcmfile);//�ر��ļ�
	return 0;
}

