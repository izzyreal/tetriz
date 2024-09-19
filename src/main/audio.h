#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#define VERBLIB_IMPLEMENTATION
#include "verblib.h"

#include "melodies.h"

#define PI 3.14159265358979f

#define TETRIZ_SAMPLE_RATE 44100
#define TETRIZ_NUM_CHANNELS 1

const float ticks_per_second = 238.4f; // 149BPM at 96 ticks per quarter note

#define MAX_VOICES 2

const int melody_lengths[MAX_VOICES] = { lead_melody_length, bass_melody_length };
Note* melodies[MAX_VOICES] = { lead_melody, bass_melody };

typedef enum { SINE, SQUARE } Osc;

const Osc oscs[MAX_VOICES] = { SQUARE, SINE };

typedef enum { DISABLED, ENABLED } Reverb;

const Reverb reverbs[MAX_VOICES] = { ENABLED, DISABLED };

typedef struct {
    float wave_state;     // Current phase of the waveform
    float audiotime;      // Time in seconds
    float current_freq;   // Current frequency
    int note_index;       // Index of the current note in the melody
    Note* melody;         // Pointer to the melody array
    int melody_length;    // Length of the melody array
    Osc osc;
    Reverb reverb;
} Voice;

Voice voices[MAX_VOICES];

verblib tetriz_verblib;

void init_voices()
{
    for (int i = 0; i < MAX_VOICES; i++)
    {
        voices[i].wave_state = 0.0f;
        voices[i].audiotime = 0.0f;
        voices[i].current_freq = 0.0f;
        voices[i].note_index = 0;
        voices[i].melody_length = melody_lengths[i];
        voices[i].melody = melodies[i];
        voices[i].osc = oscs[i];
        voices[i].reverb = reverbs[i];
    }
}

const uint8_t NUMBER_OF_BARS = 24;
const uint16_t TOTAL_TICKS = NUMBER_OF_BARS * 384;

const uint8_t FADE_IN_DURATION_TICKS = 3;
const uint8_t FADE_OUT_DURATION_TICKS = 6;

void synthesize_note(float* pFrameOut, const float frequency, float* pWaveState, const float fadeFactor, const Osc osc, const Reverb reverb)
{
    const float phase_increment = frequency / TETRIZ_SAMPLE_RATE;

    if (osc == SQUARE)
    {
        *pFrameOut = fadeFactor * ((*pWaveState < 0.5f) ? 0.1f : -0.1f);
    }
    else
    {
        *pFrameOut = fadeFactor * sinf(2.0f * PI * (*pWaveState)) * 0.4f;
    }

    *pWaveState += phase_increment;
    if (*pWaveState >= 1.0f) *pWaveState -= 1.0f;
}

float compute_fade_factor(float note_end_ticks, float current_time_ticks, float note_start_ticks)
{
    const float fade_out_start_ticks = note_end_ticks - FADE_OUT_DURATION_TICKS;

    float fade_factor = 1.0f;

    if (current_time_ticks < note_start_ticks + FADE_IN_DURATION_TICKS)
    {
        fade_factor = fminf(1.0f, (current_time_ticks - note_start_ticks) / FADE_IN_DURATION_TICKS);
    }
    else if (current_time_ticks >= fade_out_start_ticks)
    {
        fade_factor = fmaxf(0.0f, 1.0f - ((current_time_ticks - fade_out_start_ticks) / FADE_OUT_DURATION_TICKS));
    }

    return fade_factor;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    float* pFramesOut = (float*)pOutput;

    for (ma_uint32 f = 0; f < frameCount; f++)
    {
        float mixed_sample = 0.0f;

        for (int v = 0; v < MAX_VOICES; v++)
        {
            Voice* const voice = &voices[v];
            const float current_time_ticks = voice->audiotime * ticks_per_second;

            const float note_start_ticks = voice->melody[voice->note_index].pos;
            const float note_end_ticks = note_start_ticks + voice->melody[voice->note_index].duration;

            float sample = 0.0f;

            if (current_time_ticks >= note_start_ticks && current_time_ticks < note_end_ticks)
            {
                float fade_factor = compute_fade_factor(note_end_ticks, current_time_ticks, note_start_ticks);
                int midi_note1 = voice->melody[voice->note_index].note;
                voice->current_freq = 440.0f * powf(2.0f, (midi_note1 - 69) / 12.0f);
                synthesize_note(&sample, voice->current_freq, &voice->wave_state, fade_factor, voice->osc, voice->reverb);
            }

            if (voice->reverb == ENABLED)
            {
                const float reverb_input[1] = { sample };
                verblib_process(&tetriz_verblib, &sample, &sample, 1);
            }

            if (voice->audiotime >= (voice->melody[voice->note_index].duration + voice->melody[voice->note_index].pos) / ticks_per_second)
            {
                voice->note_index++;

                if (voice->note_index >= voice->melody_length)
                {
                    voice->note_index = 0;
                    voice->audiotime -= TOTAL_TICKS / ticks_per_second;
                }
            }

            voice->audiotime += 1.0f / TETRIZ_SAMPLE_RATE;

            mixed_sample += sample;
        }

        pFramesOut[f] = mixed_sample;
    }
}

void init_audio(ma_device_config* config, ma_device* device)
{
    init_voices();

    config->playback.format   = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
    config->playback.channels = TETRIZ_NUM_CHANNELS;
    config->sampleRate        = TETRIZ_SAMPLE_RATE;           // Set to 0 to use the device's native sample rate.
    config->dataCallback      = data_callback;   // This function will be called when miniaudio needs more data.
    config->periodSizeInFrames    = 256;
    //config->pUserData         = pMyCustomData;   // Can be accessed from the device object (device.pUserData).

    if (ma_device_init(NULL, config, device) != MA_SUCCESS) {
        return;  // Failed to initialize the device.
    }

    verblib_initialize(&tetriz_verblib, TETRIZ_SAMPLE_RATE, TETRIZ_NUM_CHANNELS);
    verblib_set_room_size(&tetriz_verblib, 1.0f);
    verblib_set_wet(&tetriz_verblib, 0.1f);
    verblib_set_dry(&tetriz_verblib, 1.0f);

    ma_device_start(device);     // The device is sleeping by default so you'll need to start it manually.
}
