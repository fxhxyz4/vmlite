#define MINIAUDIO_IMPLEMENTATION
#include "../../external/miniaudio.h"
#include "audio_capture.h"
#include "amplifier.h"
#include "../utils/print.h"
#include <iostream>
#include <vector>
#include <mutex>

using namespace std;

static ma_device g_captureDevice;
static ma_device g_playbackDevice;
static float g_gain = 1.0f;
static bool g_isInitialized = false;

static vector<float> g_audioBuffer;
static mutex g_bufferMutex;

constexpr size_t MAX_BUFFER_SAMPLES = 48000 * 2 * 0.05;

void captureCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    if (pInput == NULL) return;

    size_t sampleCount = frameCount * pDevice->capture.channels;
    const float* pInputSamples = (const float*)pInput;

    lock_guard<mutex> lock(g_bufferMutex);

    if (g_audioBuffer.size() > MAX_BUFFER_SAMPLES) {
        g_audioBuffer.clear();
    }

    g_audioBuffer.insert(g_audioBuffer.end(), pInputSamples, pInputSamples + sampleCount);
}

void playbackCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    if (pOutput == NULL) return;

    size_t sampleCount = frameCount * pDevice->playback.channels;
    float* pOutputSamples = (float*)pOutput;

    lock_guard<mutex> lock(g_bufferMutex);

    if (g_audioBuffer.size() >= sampleCount) {
        for (size_t i = 0; i < sampleCount; ++i) {
            pOutputSamples[i] = g_audioBuffer[i];
        }
        g_audioBuffer.erase(g_audioBuffer.begin(), g_audioBuffer.begin() + sampleCount);

        amplifyBuffer(pOutputSamples, sampleCount, g_gain);
    } else {
        for (size_t i = 0; i < sampleCount; ++i) {
            pOutputSamples[i] = 0.0f;
        }
    }
}

bool startAudioEngine(float gain) {
    g_gain = gain;

    ma_device_config captureConfig   = ma_device_config_init(ma_device_type_capture);
    captureConfig.capture.format     = ma_format_f32;
    captureConfig.capture.channels   = 2;
    captureConfig.sampleRate         = 48000;
    captureConfig.periodSizeInFrames = 1024;
    captureConfig.dataCallback       = captureCallback;

    if (ma_device_init(NULL, &captureConfig, &g_captureDevice) != MA_SUCCESS) {
        print("[ERROR] Failed to initialize capture device.");
        return false;
    }

    ma_device_config playbackConfig   = ma_device_config_init(ma_device_type_playback);
    playbackConfig.playback.format    = ma_format_f32;
    playbackConfig.playback.channels  = 2;
    playbackConfig.sampleRate         = 48000;
    playbackConfig.periodSizeInFrames = 1024;
    playbackConfig.dataCallback       = playbackCallback;

    if (ma_device_init(NULL, &playbackConfig, &g_playbackDevice) != MA_SUCCESS) {
        print("[ERROR] Failed to initialize playback device.");
        ma_device_uninit(&g_captureDevice);
        return false;
    }

    if (ma_device_start(&g_captureDevice) != MA_SUCCESS || ma_device_start(&g_playbackDevice) != MA_SUCCESS) {
        print("[ERROR] Failed to start audio devices.");
        ma_device_uninit(&g_captureDevice);
        ma_device_uninit(&g_playbackDevice);
        return false;
    }

    g_isInitialized = true;
    return true;
}

void stopAudioEngine() {
    if (!g_isInitialized) return;
    g_isInitialized = false;

    ma_device_stop(&g_captureDevice);
    ma_device_stop(&g_playbackDevice);
    ma_device_uninit(&g_captureDevice);
    ma_device_uninit(&g_playbackDevice);
}