//  -------- -------- -------- -------- -------- -------- -------- --------  //
//  emg_features.c
//  flynn, michael
//
//  functions to extract and organize features from a given emg signal.
//
//  -------- -------- -------- -------- -------- -------- -------- --------  //

#include "emg_features.h"

#include <math.h>
#include <stdlib.h>


float extract_feature(emg_signal_t *sig, emg_feature_t feat, float param)
{
    switch (feat)
    {
        case emg_feat_MAV:
            return mean_absolute_value(sig);
        case emg_feat_VAR:
            return variance(sig);
        case emg_feat_WAMP:
            return wilson_amplitude(sig, param);
        case emg_feat_WL:
            return waveform_length(sig);
        case emg_feat_ZC:
            return zero_crossings(sig, param);
        default:
            return 0;
    }
}


float mean_absolute_value(emg_signal_t *sig)
{
    float mav = 0.f;

    for (int i = 0; i < sig->length; i++)
        mav += fabs(sig->samples[i]) / (float)sig->length;

    return mav;
}


float variance(emg_signal_t *sig)
{
    float mean = 0.f, sq_mean = 0.f;

    for (int i = 0; i < sig->length; i++)
    {
        emg_sample_t s = sig->samples[i];

        mean    +=   s   / (float)sig->length;
        sq_mean += (s*s) / (float)sig->length;
    }

    return sq_mean - mean * mean;
}


float wilson_amplitude(emg_signal_t *sig, float threshold)
{
    int count = 0;

    for (int i = 0; i < sig->length-1; i++)
        if ( fabs(sig->samples[i]-sig->samples[i+1]) >= threshold )
            count++;

    return count;
}


float waveform_length(emg_signal_t *sig)
{
    float wl = 0;

    for (int i = 1; i < sig->length; i++)
        wl += fabs(sig->samples[i]-sig->samples[i-1]);

    return wl;
}


float zero_crossings(emg_signal_t *sig, float threshold)
{
    int count = 0;

    for (int i = 0; i < sig->length-1; i++)
        if (abs(sig->samples[i]-sig->samples[i+1]) >= threshold)
            if (-1 * sig->samples[i] * sig->samples[i+1] > 0)
                count++;

    return count;
}
