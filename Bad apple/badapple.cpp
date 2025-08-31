#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;
static bool g_bPlaying = true;
DWORD WINAPI AudioThread(LPVOID lpParam) {
    MCIERROR err = mciSendString("open \"badapple.wav\" type MPEGVideo alias mysong", NULL, 0, NULL);
    if (err != 0) {
        char errorMsg[256];
        mciGetErrorString(err, errorMsg, sizeof(errorMsg));
        printf("无法打开音频文件: %s\n", errorMsg);
        return 1;
    }
    
    mciSendString("play mysong repeat", NULL, 0, NULL);
    
    while (g_bPlaying) {
        Sleep(100);
    }
    
    mciSendString("close mysong", NULL, 0, NULL);
    return 0;
}
int main() {
    FILE* fp;
    char add[60], ai[5];
    int i = 1, delay = 33;
    clock_t stime = 0, ftime = 0;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOutput, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOutput, &cursorInfo);
    COORD bufferSize = {150, 5000};
    SetConsoleScreenBufferSize(hOutput, bufferSize);
    SMALL_RECT windowSize = {0, 0, 149, 40};
    SetConsoleWindowInfo(hOutput, TRUE, &windowSize);
    HANDLE hAudioThread = CreateThread(NULL, 0, AudioThread, NULL, 0, NULL);
    if (hAudioThread == NULL) {
        printf("无法创建音频线程! 错误代码: %d\n", GetLastError());
        return 1;
    }
    
    stime = clock();
    
    while (i <= 6573) {
        if (i % 60 == 0) {
            delay = 43;
        } else {
            delay = 33;
        }
        sprintf(add, "images\\a (%d).txt", i);
        
        ftime = clock();
        if (ftime - stime >= delay) {
            fp = fopen(add, "r");
            if (fp != NULL) {
                char frameBuffer[32768] = {0};
                char line[256];
                int offset = 0;
                
                while (fgets(line, sizeof(line), fp) && offset < sizeof(frameBuffer) - 256) {
                    int len = strlen(line);
                    if (offset + len > sizeof(frameBuffer) - 1) break;
                    strcpy(frameBuffer + offset, line);
                    offset += len;
                }
                SetConsoleCursorPosition(hOutput, {0, 0});
                DWORD bytesWritten;
                WriteConsole(hOutput, frameBuffer, offset, &bytesWritten, NULL);
                
                fclose(fp);
                i++;
                stime += delay;
            } else {
                printf("无法打开文件: %s\n", add);
            }
        }
    }
    g_bPlaying = false;
    WaitForSingleObject(hAudioThread, 2000);
    CloseHandle(hAudioThread);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hOutput, &cursorInfo);
    return 0;
}
