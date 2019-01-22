#pragma once

#include "../digital.hpp"

namespace AVRUtil
{
    namespace ArduinoNano
    {
        // D0 -> RX
        // D1 -> TX
        using D2 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(D), 2>;
        using D3 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(D), 3>;
        using D4 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(D), 4>;
        using D5 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(D), 5>;
        using D6 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(D), 6>;
        using D7 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(D), 7>;
        using D8 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(B), 0>;
        using D9 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(B), 1>;
        using D10 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(B), 2>;
        using D11 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(B), 3>;
        using D12 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(B), 4>;
        using D13 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(B), 5>;
        using A0 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(C), 0>;
        using A1 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(C), 1>;
        using A2 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(C), 2>;
        using A3 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(C), 3>;
        using A4 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(C), 4>;
        using A5 = AVRUtil::PinDescriptor <AVR_UTIL_PORT(C), 5>;
        // A4 -> PC4?
        // A5 -> PC5?

        using InternalLed = D13;
    }
}
