#pragma once

#include <avr/io.h>

#define AVR_UTIL_IOMEXT_INCLUDED

#if defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#   include "iom328ext.hpp"
#elif defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__)
#   include "iom168ext.hpp"
#endif

#define AVR_UTIL_ADDRESS_OFFSET __SFR_OFFSET