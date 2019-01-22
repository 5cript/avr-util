#pragma once

#include <util/delay.h>

#define delay(X) _delay_ms(X)
#define micro_delay(X) _delay_us(X)

namespace AVRUtil
{
    namespace detail {
        template <unsigned I>
        struct nopUnroller {
            static void volnop() {
                asm volatile ( "nop" );
                nopUnroller<I-1>::volnop();
            }
        };

        template <>
        struct nopUnroller<0> {
            static void volnop() {}
        };
    }

    template <unsigned cycles>
    struct CycleSleeper {
        static void cycle_delay() {
            detail::nopUnroller<cycles-4 /* jump */>::volnop();
        }
    };

    template <>
    struct CycleSleeper<0> {
        static void cycle_delay() {    }
    };

    template <>
    struct CycleSleeper<1> {
        static void cycle_delay() {
            detail::nopUnroller<1>::volnop();
        }
    };

    template <>
    struct CycleSleeper<2> {
        static void cycle_delay() {
            detail::nopUnroller<2>::volnop();
        }
    };

    template <>
    struct CycleSleeper<3> {
        static void cycle_delay() {
            detail::nopUnroller<3>::volnop();
        }
    };

    template <>
    struct CycleSleeper<4> {
        static void cycle_delay() {
            detail::nopUnroller<4>::volnop();
        }
    };

    template <unsigned cycles>
    inline void cycle_delay()
    {
        CycleSleeper <cycles>::cycle_delay();
    }
}
