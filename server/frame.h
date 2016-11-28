#ifndef __FRAME_H
#define __FRAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "chat.h"

void pack(client *c, size_t *length);

void unpack();

#endif