/* 
Uses part of nongnu's normalize
Copyright (C) 1999--2005 Chris Vaill

Adaptation to SoX
Copyright (C) 2016 Michael Larouche

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA*/

#include "sox_i.h"
#include <ctype.h>
#include <string.h>

/* anything less than EPSILON is considered zero */
#ifndef EPSILON
# define EPSILON 0.00000000001
#endif

typedef struct {
    double *buf;
    int buflen;  /* elements allocated to buffer */
    int start;   /* index of first element in buffer */
    int n;       /* num of elements in buffer */
} datasmooth_t;

typedef struct
{
    double targetLevel;
    FILE* tempFile;
    double* sums;
    datasmooth_t *powsmooth;
    int smoothEnd;
    long sampleMin;
    long sampleMax;

    // Results
    int isResultSetuped;
    double maxPower;
    double level;
    double gain;
} priv_t;

/*
 * Takes a full smoothing window, and returns the value of the center
 * element, smoothed.  Currently, just does a mean filter, but we could
 * do a median or gaussian filter here instead.
 */
static inline double get_smoothed_data(datasmooth_t *s)
{
    int i;
    /*int center = (s->n + 1) / 2;*/
    double smoothed;

    smoothed = 0;
    for (i = 0; i < s->n; i++)
        smoothed += s->buf[i];
    smoothed = smoothed / s->n;

    return smoothed;
}

static int create(sox_effect_t* effp, int argc, char** argv)
{
    priv_t* p = (priv_t*)effp->priv;

    /* Skip effect name */
    --argc; ++argv;

    do { NUMERIC_PARAMETER(targetLevel, -HUGE_VAL, HUGE_VAL) } while (0);

    return argc ? lsx_usage(effp) : SOX_SUCCESS;
}

static int start(sox_effect_t * effect)
{
    priv_t* p = (priv_t*)effect->priv;
    unsigned int channel = 0;
    int bytes_per_sample = 0;

    p->tempFile = lsx_tmpfile();
    if (!p->tempFile)
    {
        lsx_fail("Can't create temporary file: %s", strerror(errno));
        return SOX_EOF;
    }

    bytes_per_sample = (effect->in_encoding->bits_per_sample - 1) / 8 + 1;
    p->sampleMax = (1 << (bytes_per_sample * 8 - 1)) - 1;
    p->sampleMin = -p->sampleMax - 1;

    p->sums = (double*)lsx_malloc(effect->in_signal.channels * sizeof(double));
    p->powsmooth = (datasmooth_t*)lsx_malloc(effect->in_signal.channels * sizeof(datasmooth_t));
    p->smoothEnd = 0;
    p->maxPower = 0.0;
    p->isResultSetuped = 0;
    p->level = 0.0;
    p->gain = 0.0;

    for (channel = 0; channel < effect->in_signal.channels; ++channel)
    {
        p->sums[channel] = 0.0;
        p->powsmooth[channel].buflen = 100; /* Use a 100-element (1 second) window */
        p->powsmooth[channel].buf = (double*)lsx_malloc(p->powsmooth[channel].buflen * sizeof(double));
        p->powsmooth[channel].start = p->powsmooth[channel].n = 0;
    }

    return SOX_SUCCESS;
}

static int flow(sox_effect_t* effect, const sox_sample_t* inputBuffer,
    sox_sample_t* outputBuffer, size_t* inputBufferSize, size_t* outputBufferSize)
{
    priv_t* p = (priv_t*)effect->priv;
    size_t bufferLength = 0;
    sox_sample_t sample = 0;
    unsigned int channel = 0;
    double power = 0;
    SOX_SAMPLE_LOCALS;

    if (fwrite(inputBuffer, sizeof(*inputBuffer), *inputBufferSize, p->tempFile) != *inputBufferSize)
    {
        lsx_fail("Error writing temporary file: %s", strerror(errno));
        return SOX_EOF;
    }

    if (*inputBufferSize > 0)
    {
        for (channel = 0; channel < effect->in_signal.channels; ++channel)
        {
            p->sums[channel] = 0;
        }

        for (bufferLength = *inputBufferSize; bufferLength; --bufferLength, ++inputBuffer)
        {
            for (channel = 0; channel < effect->in_signal.channels; ++channel)
            {
                sample = SOX_SAMPLE_TO_SIGNED_16BIT(*inputBuffer, effect->clips);
                p->sums[channel] += sample * (double)sample;
            }
        }

        /* Compute power for each channel */
        for (channel = 0; channel < effect->in_signal.channels; ++channel)
        {
            power = p->sums[channel] / (double)(*inputBufferSize / 2);

            p->smoothEnd = (p->powsmooth[channel].start + p->powsmooth[channel].n) % p->powsmooth[channel].buflen;

            p->powsmooth[channel].buf[p->smoothEnd] = power;

            if (p->powsmooth[channel].n == p->powsmooth[channel].buflen)
            {
                p->powsmooth[channel].start = (p->powsmooth[channel].start + 1) % p->powsmooth[channel].buflen;
                power = get_smoothed_data(&p->powsmooth[channel]);

                if (power > p->maxPower)
                {
                    p->maxPower = power;
                }
            }
            else
            {
                p->powsmooth[channel].n++;
            }
        }
    }

    /* Samples does not output until drain */
    *outputBufferSize = 0;

    return SOX_SUCCESS;
}

static void setup_drain(sox_effect_t * effect)
{
    priv_t * p = (priv_t *)effect->priv;
    int srcBytesPerSample = 0;
    int destBytesPerSample = 0;
    double power = 0.0;
    unsigned int channel = 0;

    srcBytesPerSample = (effect->in_encoding->bits_per_sample - 1) / 8 + 1;
    destBytesPerSample = (effect->out_encoding->bits_per_sample - 1) / 8 + 1;

    if (p->maxPower < EPSILON)
    {
        for (channel = 0; channel < effect->in_signal.channels; ++channel)
        {
            power = get_smoothed_data(&p->powsmooth[channel]);
            if (power > p->maxPower)
            {
                p->maxPower = power;
            }
        }
    }

    /* Scale the power value to be in the range 0.0 -- 1.0 */
    p->maxPower = p->maxPower / (p->sampleMin * (double)p->sampleMin);

    p->level = sqrt(p->maxPower);

    p->gain = dB_to_linear(p->targetLevel) / p->level;

    if (destBytesPerSample != srcBytesPerSample)
    {
        p->gain *= pow(256.0, destBytesPerSample - srcBytesPerSample);
    }

    p->isResultSetuped = 1;

    rewind(p->tempFile);
}

static int drain(sox_effect_t * effect, sox_sample_t * outputBuffer, size_t * outputBufferSize)
{
    priv_t * p = (priv_t *)effect->priv;
    size_t length = 0;
    int result = SOX_SUCCESS;
    sox_sample_t sample = 0;

    if (!p->isResultSetuped)
    {
        setup_drain(effect);
    }

    *outputBufferSize -= *outputBufferSize % effect->in_signal.channels;

    length = fread(outputBuffer, sizeof(*outputBufferSize), *outputBufferSize, p->tempFile);
    if (length != *outputBufferSize && !feof(p->tempFile))
    {
        result = SOX_EOF;
    }

    for (*outputBufferSize = length; length; --length, ++outputBuffer)
    {
        sample = *outputBuffer;
        *outputBuffer = SOX_ROUND_CLIP_COUNT(sample * p->gain, effect->clips);
    }

    return result;
}

static int stop(sox_effect_t * effect)
{
    priv_t * p = (priv_t *)effect->priv;
    unsigned int channel = 0;

    fclose(p->tempFile);

    free(p->sums);
    p->sums = NULL;

    for (channel = 0; channel < effect->in_signal.channels; ++channel)
    {
        free(p->powsmooth[channel].buf);
    }

    free(p->powsmooth);
    p->powsmooth = NULL;

    return SOX_SUCCESS;
}

sox_effect_handler_t const * lsx_nongnunormalize_effect_fn(void)
{
    static sox_effect_handler_t handler = {
        "nongnunormalize", NULL, SOX_EFF_GAIN,
        create, start, flow, drain, stop, NULL, sizeof(priv_t) };
    static char const * lines[] = {
        "gain-dB\t normalize the volume to the target amplitude in dB",
    };
    static char * usage;
    handler.usage = lsx_usage_lines(&usage, lines, array_length(lines));
    return &handler;
}
