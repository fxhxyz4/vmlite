#define MINIAUDIO_IMPLEMENTATION
#include "../../external/miniaudio.h"
#include "../utils/print.h"
#include "audio_capture.h"
#include "amplifier.h"
#include <iostream>
#include <vector>
#include <mutex>

using namespace std;

static ma_device g_audioDevice;
static float g_gain = 1.0f;
static bool g_isInitialized = false;

void dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    if (pOutput == NULL || pInput == NULL) return;

    size_t sampleCount = frameCount * pDevice->playback.channels;
    const float* pInputSamples = (const float*)pInput;
    float* pOutputSamples = (float*)pOutput;

    for (size_t i = 0; i < sampleCount; ++i) {
        pOutputSamples[i] = pInputSamples[i];
    }

    amplifyBuffer(pOutputSamples, sampleCount, g_gain);
}

bool startAudioEngine(float gain) {
    g_gain = gain;

    ma_backend backends[] = {
        ma_backend_alsa,
        ma_backend_jack
    };

    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_duplex);

    deviceConfig.capture.format     = ma_format_f32;
    deviceConfig.capture.channels   = 2;
    deviceConfig.playback.format    = ma_format_f32;
    deviceConfig.playback.channels  = 2;

    deviceConfig.sampleRate         = 48000;
    deviceConfig.periodSizeInFrames = 512;
    deviceConfig.dataCallback       = dataCallback;

    if (ma_device_init_ex(backends, sizeof(backends)/sizeof(backends[0]), NULL, &deviceConfig, &g_audioDevice) != MA_SUCCESS) {
        if (ma_device_init(NULL, &deviceConfig, &g_audioDevice) != MA_SUCCESS) {
            print("[ERROR] Failed to initialize audio device.");
            return false;
        }
    }

    if (ma_device_start(&g_audioDevice) != MA_SUCCESS) {
        print("[ERROR] Failed to start audio device.");
        ma_device_uninit(&g_audioDevice);
        return false;
    }

    g_isInitialized = true;
    return true;
}

void stopAudioEngine() {
    if (!g_isInitialized) return;
    g_isInitialized = false;

    ma_device_stop(&g_audioDevice);
    ma_device_uninit(&g_audioDevice);
}