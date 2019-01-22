#pragma once

#include "iomext.hpp"

#define AVR_UTIL_PORT(X) PORT ## X ## _ADDR, DDR ## X ## _ADDR, PIN ## X ## _ADDR

namespace AVRUtil::detail
{
	//-------------------------------------------------------------------
	// set IO bit
	//-------------------------------------------------------------------
	template <unsigned PORT, unsigned NUM>
	inline void sbi() {
		_MMIO_BYTE(PORT + AVR_UTIL_ADDRESS_OFFSET) |= (1u << NUM);
	}
	template <unsigned PORT, unsigned NUM>
	inline void set() { sbi<PORT, NUM>(); }
	//-------------------------------------------------------------------
	// clear IO bit
	//-------------------------------------------------------------------
	template <unsigned PORT, unsigned NUM>
	inline void cbi() {
		_MMIO_BYTE(PORT + AVR_UTIL_ADDRESS_OFFSET) &= ~(1u << NUM);
	}
	template <unsigned PORT, unsigned NUM>
	inline void reset() { cbi<PORT, NUM>(); }
	//-------------------------------------------------------------------
	// can be compiled to sbis
	//-------------------------------------------------------------------
	template <unsigned PIN, unsigned NUM>
	inline bool is_set() {
		return ((_MMIO_BYTE(PIN + AVR_UTIL_ADDRESS_OFFSET) & (1U << NUM)) == 0u);
	}
	//-------------------------------------------------------------------
	// can be compiled to cbis
	//-------------------------------------------------------------------
	template <unsigned PIN, unsigned NUM>
	inline bool is_not_set() {
		return !is_set<PIN, NUM>();
	}
	//-------------------------------------------------------------------
	//
	//-------------------------------------------------------------------
	template <unsigned PORT, unsigned NUM>
	inline void set_to(bool&& bit) {
		if (bit)
			sbi <PORT, NUM>();
		else
			cbi <PORT, NUM>();
		//_MMIO_BYTE(PORT + ADDRESS_OFFSET) |= (bit << NUM);
	}
	//-------------------------------------------------------------------
}

namespace AVRUtil
{
    struct Nullpin {};

    template <unsigned Port, unsigned Ddr, unsigned Pin, unsigned Num>
    struct PinDescriptor
    {
        constexpr static auto port = Port;
        constexpr static auto ddr = Ddr;
        constexpr static auto num = Num;
        constexpr static auto pin = Pin;
    };

    template <typename Descriptor>
    class StaticOutputPin
    {
    public:
        static void setOutput()
        {
            detail::cbi <Descriptor::port, Descriptor::num>();
            detail::sbi <Descriptor::ddr, Descriptor::num>();
        }

        static void set()
        {
            detail::sbi <Descriptor::port, Descriptor::num>();
        }

        static void reset()
        {
            detail::cbi <Descriptor::port, Descriptor::num>();
        }

        static void toggle()
        {
            _MMIO_BYTE(Descriptor::port + AVR_UTIL_ADDRESS_OFFSET) ^= (1u << Descriptor::num);
        }
    };

    template <typename Descriptor>
    class StaticInputPin
    {
    public:
        template <bool Pullup = true>
        static void setInput()
        {
            detail::cbi<Descriptor::ddr, Descriptor::num>();
            if constexpr (Pullup)
                detail::sbi<Descriptor::port, Descriptor::num>();
            else
                detail::cbi<Descriptor::port, Descriptor::num>();
        }

        static bool get()
        {
            return detail::is_set<Descriptor::pin, Descriptor::num>();
        }
    };

    // Its your responsibility to not read from an output pin or set an input pin.
    // no checking is performed.
    template <typename Descriptor>
    struct OmniPin
        : StaticOutputPin <Descriptor>
        , StaticInputPin <Descriptor>
    {
    };

    template <typename... List>
    struct PinList
    {
        static void setOutput()
        {
            (List::setOutput(), ...);
        }

        static void toggle()
        {
            (List::toggle(), ...);
        }

        static void set()
        {
            (List::set(), ...);
        }

        static void reset()
        {
            (List::reset(), ...);
        }
    };

#define DEFINE_OUTPUT(port, num) \
    AVRUtil::StaticOutputPin <AVRUtil::PinDescriptor <AVR_UTIL_PORT(port), num>>

#define DEFINE_INPUT(port, num) \
    AVRUtil::StaticInputPin <AVRUtil::PinDescriptor <AVR_UTIL_PORT(port), num>>

#define DEFINE_OMNI(port, num) \
    AVRUtil::OmniPin <AVRUtil::PinDescriptor <AVR_UTIL_PORT(port), num>>

#define MAKE_OUTPUT(descriptor) \
    AVRUtil::StaticOutputPin <descriptor>

#define MAKE_INPUT(descriptor) \
    AVRUtil::StaticInputPin <descriptor>

#define MAKE_OMNI(descriptor) \
    AVRUtil::OmniPin <descriptor>
}
