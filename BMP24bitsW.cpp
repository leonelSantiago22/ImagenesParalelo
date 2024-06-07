/*
Cargar y procesar imágenes BMP de 24bits con C++
http://www.widget-101.com/
Autor: Christiam Mena
*/

#include <stdio.h>
#include <windows.h>
#include "BMP.h"

// Imagen mediana
#define NUM_THREADS 8 // Cantidad de hilos deseados

struct stParam
{
    unsigned char br[16];
    BYTE* pImgData;
    DWORD pImgSize;
};
typedef struct stParam STPARAM;

DWORD WINAPI hilobr(LPVOID);

extern "C" void brightness_xmmw(unsigned char u[16], BYTE * gimg, int size);

int main(int argc, char** argv)
{
    LARGE_INTEGER tinicio, tfin, frec;
    double mcseg;
    DWORD threadsId[NUM_THREADS];
    HANDLE hHilosbr[NUM_THREADS];
    STPARAM stParam[NUM_THREADS];
    unsigned char br[16] = { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 };

    BMP bmp("nature.bmp");
    //bmp.printHeader();
    //bmp.blueChannel();

    QueryPerformanceFrequency(&frec);
    QueryPerformanceCounter(&tinicio);

    //bmp.brightness(25);
    //brightness_xmmw(br, bmp.getImageData(), bmp.getImageSize());

    // Sección de inicialización de parámetros para los hilos
    int imgSizePerThread = bmp.getImageSize() / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        memcpy(stParam[i].br, br, sizeof(unsigned char) * 16);
        stParam[i].pImgData = bmp.getImageData() + i * imgSizePerThread;
        stParam[i].pImgSize = imgSizePerThread;
    }



    for (int i = 0; i < 2; i++) {
        hHilosbr[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)hilobr,
            (LPVOID)&stParam[i],
            0, (LPDWORD)&threadsId[i]);
    }


    WaitForMultipleObjects(NUM_THREADS, hHilosbr, TRUE, INFINITE);

    QueryPerformanceCounter(&tfin);
    bmp.save("brillo_xmm_2h.bmp");

    mcseg = (double)(tfin.QuadPart - tinicio.QuadPart) / frec.QuadPart;
    printf("%lf ms\n", mcseg * 1000);

    //getchar();  // Con esta línea evitaremos que se cierre la consola
    return 0;
}

DWORD WINAPI hilobr(LPVOID lpparam) {
    STPARAM* param = (STPARAM*)lpparam;
    brightness_xmmw(param->br, param->pImgData, param->pImgSize);

    return 0;
}
