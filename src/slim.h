// 
// copyright 2013-2014 Kaspar Emanuel
// 
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
// 

#ifndef __SLIM_H__
#define __SLIM_H__

#include "looper.h"
#include "slim_socket.h"
#include "protocol/nanopb/pb_decode.h"
#include "protocol/slim.pb.h"

#define MAX_MESSAGES 32

typedef struct {
    const float* input;
    float* output;
    uint32_t n_loopers;
    SlimSocket* socket;
    Looper** looper_array;
    char msg_buffer[MSG_BUFFER_SIZE];
    SlimMessage messages[MAX_MESSAGES];
} Slim;

Slim* slim_new(uint32_t n_loopers, uint32_t max_n_samples);
void slim_activate(Slim* slim);
void slim_run(Slim* slim , uint32_t n_samples);
void slim_free(Slim* slim);
void slim_connect(Slim* slim, void* input, void* output);
//void slim_work_loop(Slim* slim);
static void slim_parse_looper_message(Slim* slim, const SlimMessage msg);
static void slim_parse_global_message(Slim* slim, const SlimMessage msg);
static void slim_parse_messages(Slim* slim, const uint32_t n_bytes, char* msg_buffer);
#endif// __SLIM_H__
