// This file is part of SLim Looper.
// 
// SLim Looper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as 
// published by the Free Software Foundation.
// 
// SLim Looper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with SLim Looper. If not, see <http://www.gnu.org/licenses/>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "looper.h"

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
    Looper* looper = (Looper*)malloc(sizeof(Looper));
    looper->loop = (Loop*)malloc(sizeof(Loop));
    looper->loop->buffer = calloc(LOOP_MAX_SAMPLES, sizeof(float));
    looper->settings = (LooperSettings*)malloc(sizeof(LooperSettings));

	// Get host features
    if (features)
    {
        for (int i = 0; features[i]; ++i) {
            if (!strcmp(features[i]->URI, LV2_URID__map)) {
                looper->map = (LV2_URID_Map*)features[i]->data;
            } 
            //else if (!strcmp(features[i]->URI, LV2_WORKER__schedule)) {
            //	looper->schedule = (LV2_Worker_Schedule*)features[i]->data;
            //} 
            else if (!strcmp(features[i]->URI, LV2_LOG__log)) {
                looper->log = (LV2_Log_Log*)features[i]->data;
            }
        }
    }
    if (!looper->map) {
        lv2_log_error(&looper->logger, "Missing feature urid:map\n");
        goto fail;
    } 
    //else if (!self->schedule) {
	//	lv2_log_error(&self->logger, "Missing feature work:schedule\n");
	//	goto fail;
	//}

    looper->midi_Event = looper->map->map(looper->map->handle, LV2_MIDI__MidiEvent);

    return (LV2_Handle)looper;
fail:
    free(looper);
    return NULL;
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
    Looper* looper = (Looper*)instance;

    switch ((PortIndex)port) {
    case PORT_INPUT:
        looper->input = (const float*)data;
        break;
    case PORT_OUTPUT:
        looper->output = (float*)data;
        break;
    case PORT_CONTROL:
        looper->control_input = (const float*)data;
        break;
    case PORT_RECORD_MODE:
        looper->record_mode_input = (const float*)data;
        break;
    case PORT_MIDI_IN:
        looper->midi_input = (LV2_Atom_Sequence*) data;
        break;
    }
}

static void
activate(LV2_Handle instance)
{
    Looper* looper = (Looper*)instance;
    looper->loop->pos = 0;
    looper->loop->end = 0;
    looper->state = PAUSED;
    looper->previous_state = PAUSED;
    looper->settings->record_mode = MODE_NEW;
}

/** Process a block of audio (audio thread, must be RT safe). */
static void
run(LV2_Handle instance, uint32_t n_samples)
{
    Looper*             looper   = (Looper*)instance;
    Loop*               loop     = looper->loop;
    LooperSettings*     settings = looper->settings;

    const float* const input  = looper->input;
    float* const       output = looper->output;

    settings->record_mode  = (LooperRecordMode)(*(looper->record_mode_input));
    looper->state          = (LooperState)(*(looper->control_input));

    LV2_ATOM_SEQUENCE_FOREACH(looper->midi_input, ev) 
    {
		if (ev->body.type == looper->midi_Event) {
            lv2_log_trace(&looper->logger, "event: %u\r\n", ev->time.frames);
            lv2_log_trace(&looper->logger, "event size: %u\r\n", ev->body.size);
        }
    }

    switch(looper->state)
    {
        case RECORDING:
            slim_record(looper, n_samples);
            break;
        case PLAYING:
            slim_play(looper, n_samples);
            break;
        case PAUSED:
        default:
            memset(output, 0, n_samples * sizeof(float));
            break;
    }

    looper->previous_state = looper->state;
}

// is position after processing nsamples before loop end?
static uint8_t slim_loop_pos_before_end(Loop* loop, uint32_t n_samples)
{
    return ((loop->pos + n_samples) <= (loop->end));
}

// existence is defined as being at least one block long
static uint8_t slim_loop_exists(Loop* loop, uint32_t n_samples)
{
    return (loop->end >= n_samples);
}

static void
slim_record(Looper* looper, uint32_t n_samples)
{
    const float* const input  = looper->input;
    float* const       output = looper->output;

    Loop*               loop     = looper->loop;
    LooperSettings*     settings = looper->settings;

    LV2_ATOM_SEQUENCE_FOREACH(looper->midi_input, ev) {
        printf("event: %li\r\n", ev->time.frames);
    }

    switch(settings->record_mode) 
    {
        case MODE_NEW:
            //reset if we are not already running
            if (looper->previous_state != RECORDING)
            {
                loop->pos = 0;
                loop->end = 0;
            }
            memcpy(&(loop->buffer[loop->pos]), input, n_samples * sizeof(float));
            loop->pos += n_samples;
            loop->end += n_samples;
            memset(output, 0, n_samples * sizeof(float));
            break;

        case MODE_OVERDUB:
            if (slim_loop_pos_before_end(loop, n_samples)) 
            {
                memcpy(output, &(loop->buffer[loop->pos]), n_samples * sizeof(float));
                for (int i = 0; i < n_samples; i++)
                {   //TODO: reduce gain to stop clipping 
                    loop->buffer[loop->pos + i] += input[i];
                }
                loop->pos += n_samples;
            }
            //position is greater than loop length 
            //looping around to start as long as we have a loop
            else if (slim_loop_exists(loop, n_samples)) 
            {
                memcpy(output, loop->buffer, n_samples * sizeof(float));
                for (int i = 0; i < n_samples; i++)
                {//TODO: reduce gain to stop clipping 
                    loop->buffer[i] += input[i];
                }
                loop->pos = n_samples;
            }
            else //no loop, output silence
            {
                memset(output, 0, n_samples * sizeof(float));
            }
            break;

        case MODE_INSERT:
            if (slim_loop_exists(loop, n_samples))
            {
                memmove(&(loop->buffer[loop->pos + n_samples]), &(loop->buffer[loop->pos]), (loop->end - loop->pos) * sizeof(float));
                memcpy(&(loop->buffer[loop->pos]), input, n_samples * sizeof(float));
                loop->pos += n_samples;
                loop->end += n_samples;
            }
            memset(output, 0, n_samples * sizeof(float));
            break;

        case MODE_REPLACE:
            if (slim_loop_pos_before_end(loop, n_samples)) 
            {
                memcpy(&(loop->buffer[loop->pos]), input, n_samples * sizeof(float));
                loop->pos += n_samples;
            }
            //position is greater than loop length 
            //looping around to start as long as we have a loop
            else if (slim_loop_exists(loop, n_samples))
            {
                memcpy(loop->buffer, input, n_samples * sizeof(float));
                loop->pos = n_samples;
            }
            memset(output, 0, n_samples * sizeof(float));
            break;

        default:
            break;
    }
}

static void
slim_play(Looper* looper, uint32_t n_samples)
{
    const float* const input  = looper->input;
    float* const       output = looper->output;

    Loop*               loop     = looper->loop;
    LooperSettings*     settings = looper->settings;

    if (slim_loop_pos_before_end(loop, n_samples))
    {
        memcpy(output, &(loop->buffer[loop->pos]), n_samples * sizeof(float));
        loop->pos += n_samples;
    }
    //position is greater than loop length 
    //looping around to start as long as we have a loop
    else if (slim_loop_exists(loop, n_samples))
    {
        memcpy(output, loop->buffer, n_samples * sizeof(float));
        loop->pos = n_samples;
    }
    else //no loop, output silence
    {
        memset(output, 0, n_samples * sizeof(float));
    }
}

static void
deactivate(LV2_Handle instance)
{
}

static void
cleanup(LV2_Handle instance)
{
    Looper* looper = (Looper*)instance;
    free(looper->loop->buffer);
    free(looper->loop);
    free(looper);
}

static const void*
extension_data(const char* uri)
{
    return NULL;
}

static const LV2_Descriptor descriptor = {
    LOOPER_URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
    switch (index) {
    case 0:
        return &descriptor;
    default:
        return NULL;
    }
}

