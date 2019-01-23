#pragma once

#include "../../delay.hpp"
#include "../../digital.hpp"

// have no STL, only avr-libc, which is fine.
#include <stdint.h>

// ban be significantly faster! -> go into microseconds
#define ENABLE_DELAY() micro_delay(10)

// do not make this too slow
#define OPERATION_DELAY() micro_delay(1)

namespace AVRUtil
{
    template <
        typename D_I_T,
        typename RW_T,
        typename E_T,
        typename DB0_T,
        typename DB1_T,
        typename DB2_T,
        typename DB3_T,
        typename DB4_T,
        typename DB5_T,
        typename DB6_T,
        typename DB7_T,
        typename CS1_T,
        typename CS2_T,
        typename RES_T
    >
    struct DisplayPins
    {
        using D_I = D_I_T;
        using RW = RW_T;
        using E = E_T;
        using DB0 = DB0_T;
        using DB1 = DB1_T;
        using DB2 = DB2_T;
        using DB3 = DB3_T;
        using DB4 = DB4_T;
        using DB5 = DB5_T;
        using DB6 = DB6_T;
        using DB7 = DB7_T;
        using CS1 = CS1_T;
        using CS2 = CS2_T;
        using RESET = RES_T;
    };

    template <typename Pins /*A display pins struct*/>
    struct Display128x64
    {
    private:
        using pins = Pins;

        using dataBundle = OmniPinList <
            typename pins::DB0,
            typename pins::DB1,
            typename pins::DB2,
            typename pins::DB3,
            typename pins::DB4,
            typename pins::DB5,
            typename pins::DB6,
            typename pins::DB7
        >;

        using controlBundle = PinList <
            typename pins::D_I,
            typename pins::RW,
            typename pins::E,
            typename pins::CS1,
            typename pins::CS2,
            typename pins::RESET
        >;

        static void write(uint8_t byte)
        {
            pins::RW::reset();
            dataBundle::setOutput();

            setDataPort(byte);

            OPERATION_DELAY();
            enablePulse();
            OPERATION_DELAY();
        }

        /**
         *  Sets all pins to output and to low.
         */
        static void setupIo()
        {
            // sets all pins to output and sets them low.
            controlBundle::setOutput();
            controlBundle::reset();
            dataBundle::setOutput();
            dataBundle::reset();
        }

    public:
        /**
         *  Selects left and right sub display.
         *
         *  (The display is split up into left and right (64 pixel width per side).
         */
        static void selectBoth()
        {
            pins::CS1::set();
            pins::CS2::set();
        }

        /**
         *  Select the left sub display.
         *
         *  (The display is split up into left and right (64 pixel width per side).
         */
        static void selectLeft()
        {
            pins::CS1::set();
            pins::CS2::reset();
        }

        /**
         *  Select the right sub display.
         *
         *  (The display is split up into left and right (64 pixel width per side).
         */
        static void selectRight()
        {
            pins::CS1::reset();
            pins::CS2::set();
        }

        static void setDataPort(uint8_t data)
        {
            pins::DB7::setTo((data & 0b1000'0000) >> 7);
            pins::DB6::setTo((data & 0b0100'0000) >> 6);
            pins::DB5::setTo((data & 0b0010'0000) >> 5);
            pins::DB4::setTo((data & 0b0001'0000) >> 4);
            pins::DB3::setTo((data & 0b0000'1000) >> 3);
            pins::DB2::setTo((data & 0b0000'0100) >> 2);
            pins::DB1::setTo((data & 0b0000'0010) >> 1);
            pins::DB0::setTo((data & 0b0000'0001) >> 0);

            OPERATION_DELAY();
        }

        /**
         *  Send a command to the display.
         */
        static void sendCommand(uint8_t command)
        {
            pins::D_I::reset();
            OPERATION_DELAY();

            write(command);

            wait();
        }

        /**
         *  Sets the y position (int 8 pixel steps).
         */
        static void setPage(int y)
        {
            sendCommand(0b10111000 | y);
        }

        /**
         *  Sets the x position.
         */
        static void setColumn(int x)
        {
            sendCommand(0b01000000 | x);
        }

        /**
         *  Used for scrolling (not supported).
         */
        static void setAddress(int z)
        {
            sendCommand(0b11000000 | z);
        }

        /**
         *  Move the cursor into the top right corner (for the selected sub displays).
         */
        static void initializeCursor()
        {
            setAddress(0);
            setColumn(0);
            setPage(0);
        }

        /**
         *  Initializes the display.
         *
         *  Sets pins to output and sets the HIGH/LOW of the individuals pins to
         *  their initial state. Resets the display once and then turns the reset
         *  pin to be always on.
         */
        static void initialize()
        {
            setupIo();
            delay(3); // wait after setting all to low

            pins::RESET::set();
            delay(10);

            selectBoth();
            delay(1);

            wait();

            initializeCursor();
        }

        static void setDisplayOn(bool on)
        {
            uint8_t command = 0b00111110;
            if (on)
                command |= 0b1;

            selectBoth();
            sendCommand(command);
        }

        /**
         *  Writes 8 pixels at current screen position.
         */
        static void writeByte(uint8_t data)
        {
            pins::D_I::set();
            OPERATION_DELAY();
            write(data);
        }

        static void fill()
        {
            for (int page = 0; page != 8; ++page)
            {
                setPage(page);
                setColumn(0);
                for (int y = 0; y != 64; ++y)
                {
                    writeByte(0b11111111);
                }
            }
        }

        static void clear()
        {
            for (int page = 0; page != 8; ++page)
            {
                setPage(page);
                setColumn(0);
                for (int y = 0; y != 64; ++y)
                {
                    writeByte(0x0);
                }
            }
        }

        static void enablePulse()
        {
            pins::E::set();
            ENABLE_DELAY();
            pins::E::reset();
        }

        static uint8_t read()
        {
            pins::RW::set();

            dataBundle::reset();
            dataBundle::setInput();

            enablePulse();

            uint8_t result =
                (pins::DB7::get() << 7) |
                (pins::DB6::get() << 6) |
                (pins::DB5::get() << 5) |
                (pins::DB4::get() << 4) |
                (pins::DB3::get() << 3) |
                (pins::DB2::get() << 2) |
                (pins::DB1::get() << 1) |
                (pins::DB0::get() << 0)
            ;

            OPERATION_DELAY();

            return result;
        }

        static bool isBusy()
        {
            // can be improved upon, only like this until functionality is provided
            pins::D_I::reset();
            pins::RW::set();
            pins::DB7::reset();
            pins::DB7::setInput();

            enablePulse();

            bool res = pins::DB7::get();

            pins::DB7::setOutput();
            return res;
        }

        static void wait()
        {
            //for (int i = 0; isBusy() && i != 10; ++i)
            delay(3);
        }
    };
}
