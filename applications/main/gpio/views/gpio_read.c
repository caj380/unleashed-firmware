#include "gpio_read.h"
#include "../gpio_item.h"

#include <gui/elements.h>

struct GpioRead {
    View* view;
    GpioReadOkCallback callback;
    void* context;
};

typedef struct {
    uint8_t pin_idx;
} GpioReadModel;

static bool gpio_read_process_left(GpioRead* gpio_read);
static bool gpio_read_process_right(GpioRead* gpio_read);
static bool gpio_read_process_ok(GpioRead* gpio_read, InputEvent* event);

static void gpio_read_draw_callback(Canvas* canvas, void* _model) {
    GpioReadModel* model = _model;
    canvas_set_font(canvas, FontPrimary);
    elements_multiline_text_aligned(canvas, 64, 2, AlignCenter, AlignTop, "GPIO Input Mode Test");
    canvas_set_font(canvas, FontSecondary);
    elements_multiline_text_aligned(
        canvas, 64, 16, AlignCenter, AlignTop, "Press < or > to change pin");
    elements_multiline_text_aligned(
        canvas, 64, 32, AlignCenter, AlignTop, gpio_item_get_pin_name(model->pin_idx));
    elements_multiline_text_aligned(
        canvas, 64, 48, AlignCenter, AlignTop, gpio_item_read_pin(model->pin_idx));
}

static bool gpio_read_input_callback(InputEvent* event, void* context) {
    furi_assert(context);
    GpioRead* gpio_read = context;
    bool consumed = false;

    if(event->type == InputTypeShort) {
        if(event->key == InputKeyRight) {
            consumed = gpio_read_process_right(gpio_read);
        } else if(event->key == InputKeyLeft) {
            consumed = gpio_read_process_left(gpio_read);
        }
    } else if(event->key == InputKeyOk) {
        consumed = gpio_read_process_ok(gpio_read, event);
    }

    return consumed;
}

static bool gpio_read_process_left(GpioRead* gpio_read) {
    with_view_model(
        gpio_read->view,
        GpioReadModel * model,
        {
            if(model->pin_idx) {
                model->pin_idx--;
            }
        },
        true);
    return true;
}

static bool gpio_read_process_right(GpioRead* gpio_read) {
    with_view_model(
        gpio_read->view,
        GpioReadModel * model,
        {
            if(model->pin_idx < GPIO_ITEM_COUNT) {
                model->pin_idx++;
            }
        },
        true);
    return true;
}

static bool gpio_read_process_ok(GpioRead* gpio_read, InputEvent* event) {
    bool consumed = false;

    with_view_model(
        gpio_read->view,
        GpioReadModel * model,
        {
            if(event->type == InputTypePress) {
                if(model->pin_idx < GPIO_ITEM_COUNT) {
                    gpio_item_set_pin(model->pin_idx, true);
                } else {
                    gpio_item_set_all_pins(true);
                }
                consumed = true;
            } else if(event->type == InputTypeRelease) {
                if(model->pin_idx < GPIO_ITEM_COUNT) {
                    gpio_item_set_pin(model->pin_idx, false);
                } else {
                    gpio_item_set_all_pins(false);
                }
                consumed = true;
            }
            gpio_read->callback(event->type, gpio_read->context);
        },
        true);

    return consumed;
}

GpioRead* gpio_read_alloc() {
    GpioRead* gpio_read = malloc(sizeof(GpioRead));

    gpio_read->view = view_alloc();
    view_allocate_model(gpio_read->view, ViewModelTypeLocking, sizeof(GpioReadModel));
    view_set_context(gpio_read->view, gpio_read);
    view_set_draw_callback(gpio_read->view, gpio_read_draw_callback);
    view_set_input_callback(gpio_read->view, gpio_read_input_callback);

    return gpio_read;
}

void gpio_read_free(GpioRead* gpio_read) {
    furi_assert(gpio_read);
    view_free(gpio_read->view);
    free(gpio_read);
}

View* gpio_read_get_view(GpioRead* gpio_read) {
    furi_assert(gpio_read);
    return gpio_read->view;
}

void gpio_read_set_ok_callback(GpioRead* gpio_read, GpioReadOkCallback callback, void* context) {
    furi_assert(gpio_read);
    furi_assert(callback);
    with_view_model(
        gpio_read->view,
        GpioReadModel * model,
        {
            UNUSED(model);
            gpio_read->callback = callback;
            gpio_read->context = context;
        },
        false);
}
