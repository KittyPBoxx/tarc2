#ifndef _AGB_ACCURACY_H
#define _AGB_ACCURACY_H

/**
 * AGB Accuracy Tests Documentation
 * 
 * This file is designed to where when RunAgbAccuracyTests it will run the tests defined
 * to be enabled in the value passed to the function as a bitfield.
 *
 * The following is a list of all tests, in order, defined as (1 << n) in the bitfield:
 *
 * Memory Tests:
 * CPU EXTERNAL WORK RAM
 * CPU INTERAL WORK RAM
 * PALETTE RAM
 * VRAM
 * OAM
 * CARTRIDGE TYPE FLAG
 * PREFETCH BUFFER
 * WAIT STATE WAIT CONTROL
 * CARTRIDGE RAM WAIT CONTROL
 * 
 * Lcd Tests:
 * V COUNTER
 * V COUNT INTR FLAG
 * H BLANK INTR FLAG
 * V BLANK INTR FLAG
 * V COUNT STATUS
 * H BLANK STATUS
 * V BLANK STATUS
 *
 * Timer Tests:
 * TIMER PRESCALER
 * TIMER CONNECT
 * TIMER INTR FLAG
 *
 * DMA Tests:
 * DMA0 ADDRESS CONTROL
 * DMA1 ADDRESS CONTROL
 * DMA2 ADDRESS CONTROL
 * DMA3 ADDRESS CONTROL
 * DMA V BLANK START
 * DMA H BLANK START
 * DMA DISPLAY START
 * DMA INTR FLAG
 * DMA PRIORITY
 *
 * COM Tests:
 * MULTI PLAY SIO
 *
 * Key Tests:
 * KEY INPUT
 * KEY INPUT SIMPLE
 *
 * Interrupt Tests:
 * V BLANK INTR
 * H BLANK INTR
 * V COUNT INTR
 * TIMER INTR
 * SIO INTR
 * DMA INTR
 * KEY INTR
 *
 * NOTE: Not every test has been added to the library.
 */
 
// ----------------------------------------
// Test List Configuration
// ----------------------------------------

// Name will point to an ASCII name for the test. Please be sure to, if needed, convert it to
// a game specific charmap if printing is desired.
typedef s32 (*TestFunc)(void);
struct TestSpec {
    bool8 enabled;
    const char *name;
    TestFunc func;
};

u8 RunAgbAccuracyTests();

#endif // _AGB_ACCURACY_H
