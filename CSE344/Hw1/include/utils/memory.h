#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>

#include "utils/log.h"

void *my_malloc(size_t size);
void *my_realloc(void *ptr, size_t size);

#endif
