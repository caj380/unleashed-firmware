#pragma once

typedef enum {
    GpioStartEventOtgOff = 0,
    GpioStartEventOtgOn,
    GpioStartEventManualControl,
    GpioStartEventMonitor,
    GpioStartEventUsbUart,

    GpioCustomEventErrorBack,

    GpioUsbUartEventConfig,
    GpioUsbUartEventConfigSet,
} GpioCustomEvent;
