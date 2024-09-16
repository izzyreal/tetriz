#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#define PI 3.14159265358979f

float wave_state1 = 0.0f;
float audiotime1 = 0.0f;
float current_freq1 = 0.0f;
float sample_rate = 44100.0f;
int note_index1 = 0;

const float ticks_per_second = 238.4f;

typedef struct {
    float note; // MIDI note number
    float pos; // ticks
    float duration; // ticks
} Note;

Note voice1_melody[] = {{64, 0, 96}, {59, 96, 48}, {60, 144, 48}, {62, 192, 96}, {60, 288, 48}, {59, 336, 48}, {57, 384, 96}, {57, 480, 48}, {60, 528, 48}, {64, 576, 96}, {62, 672, 48}, {60, 720, 48}, {59, 768, 96}, {59, 864, 48}, {60, 912, 48}, {62, 960, 96}, {64, 1056, 96}, {60, 1152, 96}, {57, 1248, 96}, {57, 1344, 96}}; // 4 bars

const uint8_t number_of_bars = 4;

int melody_length1 = sizeof(voice1_melody) / sizeof(Note);

const int fade_in_duration_ticks = 3;
const int fade_out_duration_ticks = 21;

void synthesize_note(float* pFrameOut, float frequency, float* pWaveState, float fadeFactor)
{
    float phase_increment = frequency / sample_rate;
    *pFrameOut = fadeFactor * sinf(2.0f * PI * (*pWaveState));
    *pWaveState += phase_increment;
    if (*pWaveState >= 1.0f) *pWaveState -= 1.0f;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    float* pFramesOut = (float*)pOutput;
    float total_ticks = number_of_bars * 384.0f;

    for (ma_uint32 f = 0; f < frameCount; f++)
    {
        const float current_time_ticks = audiotime1 * ticks_per_second;

        const float note_start_ticks = voice1_melody[note_index1].pos;
        const float note_end_ticks = note_start_ticks + voice1_melody[note_index1].duration;

        float fadeFactor = 1.0f;

        if (current_time_ticks < note_start_ticks || current_time_ticks >= note_end_ticks)
        {
            pFramesOut[f] = 0.0f;
        }
        else
        {
            const float fade_out_start_ticks = note_end_ticks - fade_out_duration_ticks;

            if (current_time_ticks < note_start_ticks + fade_in_duration_ticks)
            {
                float fadeInFactor = (current_time_ticks - note_start_ticks) / (fade_in_duration_ticks);
                fadeFactor = fminf(1.0f, fadeInFactor);
            }
            else if (current_time_ticks >= fade_out_start_ticks)
            {
                float fadeOutFactor = 1.0f - ((current_time_ticks - fade_out_start_ticks) / (fade_out_duration_ticks));
                fadeFactor = fmaxf(0.0f, fadeOutFactor);
            }

            const int midi_note1 = voice1_melody[note_index1].note;
            current_freq1 = 440.0f * powf(2.0f, (midi_note1 - 69) / 12.0f);
            synthesize_note(&pFramesOut[f], current_freq1, &wave_state1, fadeFactor);
        }

        if (audiotime1 >= (voice1_melody[note_index1].duration + voice1_melody[note_index1].pos) / ticks_per_second)
        {
            note_index1++;

            if (note_index1 >= melody_length1)
            {
                note_index1 = 0;
                audiotime1 -= total_ticks / ticks_per_second;
            }
        }

        audiotime1 += 1.0f / sample_rate;
    }
}

void init_audio(ma_device_config* config, ma_device* device)
{
    config->playback.format   = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
    config->playback.channels = 1;               // Set to 0 to use the device's native channel count.
    config->sampleRate        = 44100;           // Set to 0 to use the device's native sample rate.
    config->dataCallback      = data_callback;   // This function will be called when miniaudio needs more data.
    config->periodSizeInFrames    = 32768;
    //config->pUserData         = pMyCustomData;   // Can be accessed from the device object (device.pUserData).

    if (ma_device_init(NULL, config, device) != MA_SUCCESS) {
        return;  // Failed to initialize the device.
    }

    ma_device_start(device);     // The device is sleeping by default so you'll need to start it manually.
}
