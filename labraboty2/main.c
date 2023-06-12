#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include"windows.h"
#pragma warning(disable:4996)


extern int min_n;
extern int max_n;
extern int mas_len;

		
void WINAPI min_max(LPVOID params) {

	float minValue = FLT_MAX;
	float maxValue = FLT_MIN;

	for (int i = 0; i < mas_len; i++) {
		if (minValue > ((float*)params)[i]) {
			minValue = ((float*)params)[i];
			min_n = i;
		}
		Sleep(7);
		if (maxValue < ((float*)params)[i]) {
			maxValue = ((float*)params)[i];
			max_n = i;
		}
		Sleep(7);
	}
	printf("\nMin Element:\n");
	printf("%f", minValue);
	printf("\nMax Element:\n");
	printf("%f", maxValue);
}

extern float av_val;
extern int mas_len;


void WINAPI average(LPVOID params){
	av_val = 0;

	for (int i = 0; i < mas_len; i++) {
		av_val += ((float*)params)[i];
		Sleep(12);
	}
	av_val /= mas_len;
	printf("\n§³§â§Ö§Õ§ß§Ú§Û: \n");
	printf("%f", av_val);
}

int min_n = 0;
int max_n = 0;
float av_val;
int mas_len = 0;

int main() {
	printf("§Ù§Ñ§á§Ú§ã§Ñ§ä§î §â§Ñ§Ù§Þ§Ö§â §Þ§Ñ§ã§ã§Ú§Ó§Ñ\n");
	scanf("%d", &mas_len);

	HANDLE* threadArray = (HANDLE*)calloc(mas_len, sizeof(HANDLE));
	float* array = (float*)calloc(mas_len, sizeof(float));

	for (int i = 0; i < mas_len; i++) {
		float value = 0;

		printf("§Ù§Ñ§á§Ú§ã§Ñ§ä§î §Ù§ß§Ñ§é§Ö§ß§Ú§Ö §ï§Ý§Ö§Þ§Ö§ß§ä§Ñ §Þ§Ñ§ã§ã§Ú§Ó§Ñ [%d]\n", i);
		scanf("%f", &value);
		array[i] = value;


	}

	printf("***********************\n");

	HANDLE MinMaxThread = CreateThread(NULL, 0, min_max, array, 0, NULL);
		
	HANDLE AverageThread = CreateThread(NULL, 0, average, array, 0, NULL);
		

	DWORD WaitDorSingleObject(
		HANDLE hHandle,
		DWORD dwMilliseconds
	);
	WaitForSingleObject(MinMaxThread, INFINITE);
	WaitForSingleObject(AverageThread, INFINITE);
	CloseHandle(MinMaxThread);
	CloseHandle(AverageThread);

	array[min_n] = av_val;
	array[max_n] = av_val;

	printf("\n§²§Ö§Ù§å§Ý§î§ä§Ñ§ä:\n");
	for (size_t i = 0; i < mas_len; i++)
	{
		printf("%f ", array[i]);
	}

	system("pause");
	return 0;
}