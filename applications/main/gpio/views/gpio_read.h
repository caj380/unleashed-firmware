#pragma once

#include <gui/view.h>

typedef struct GpioRead GpioRead;
typedef void (*GpioReadOkCallback)(InputType type, void* context);

GpioRead* gpio_read_alloc();

void gpio_read_free(GpioRead* gpio_read);

View* gpio_read_get_view(GpioRead* gpio_read);

void gpio_read_set_ok_callback(GpioRead* gpio_read, GpioReadOkCallback callback, void* context);
