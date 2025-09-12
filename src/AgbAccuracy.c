#include "gba/gba.h"
#include "global.h"
#include <stdarg.h>
#include "AgbAccuracy.h"
#include "main.h"
#include "gpu_regs.h"

// Standalone emulator test in a single file/header. Can be compiled stand alone, just run 
// RunAgbAccuracyTests to get the result as a return. Read the header file for the
// documentation. Assumes the standard GBA SDK definitions.

// Authors: PikalaxALT, RevoSucks
// Credit to the AGS Aging Test cartridge developers and endrift for
// helping out/advice.

// These ends are declared in the asm tests below, but GCC isn't smart
// enough to see it to let the linker handle it, even if they were global'd
// in the below assembler.
extern u32 NESPipelineTest_Internal_End(void);
extern u32 TimerPrescalerTest_End(void);

// -----------------------------------
// Assembler Routines
// -----------------------------------

#if MODERN
__attribute__((no_reorder,target("arm")))
#endif
static NAKED u32 NESPipelineTest_Internal(void) {
    asm_unified(".arm\n"
                "mov	r1, lr\n"
                "mov	r0, #0\n"
                "add	lr, pc, #8\n"
                "ldr	r0, [pc, #-16]\n"
                "str	r0, [lr]\n"
                "mov	r0, #0xFF\n"
                "mov	r0, #0xFF\n"
                "bx	r1\n"
                ".pool\n"
                "NESPipelineTest_Internal_End:");
}

#if MODERN
__attribute__((no_reorder,target("arm")))
#endif
static NAKED u32 TimerPrescalerTest(void) {
    asm_unified(".arm\n"
                "stmda sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}\n"
                "@ Load from va_list\n"
                "ldr r1, [r0, #4]\n"
                "ldr r0, [r0]\n"
                "mov r2, #0x400\n"
                "mov r4, r0\n"
                "mov r5, #0\n"
                "str r5, [r4] @ Reset\n"
                "str r1, [r4] @ Start\n"
                "TimerPrescalerLoop:\n"
                "subs r2, r2, #1\n"
                "bne TimerPrescalerLoop\n"
                "mov r0, r0\n"
                "mov r0, r0\n"
                "ldrh r0, [r4]\n"
                "str r5, [r4]\n"
                "ldmib sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}\n"
                "bx lr\n"
                "TimerPrescalerTest_End:\n");
}

// sub_800326C from AGS Aging Cart
#if MODERN
__attribute__((no_reorder))
#endif
static NAKED u32 PrefetchBufferResult_Func(void) {
    asm_unified(".thumb\n"
                "push {r4, r5, r6, r7, lr}\n"
                "ldr r4, =0x04000100\n"
                "movs r5, #0\n"
                "str r5, [r4]\n"
                "ldr r6, =0x00800000\n"
                "str r6, [r4]\n"
                "ldr r2, [r4]\n"
                "ldr r2, [r4]\n"
                "ldr r2, [r4]\n"
                "ldr r2, [r4]\n"
                "ldr r2, [r4]\n"
                "ldr r2, [r4]\n"
                "ldr r2, [r4]\n"
                "ldr r2, [r4]\n"
                "ldrh r0, [r4]\n"
                "str r5, [r4]\n"
                "pop {r4, r5, r6, r7}\n"
                "pop {r1}\n"
                "bx r1\n"
                ".pool\n");
}

// ------------------------------
// C Routines
// ------------------------------

static u16 SetIME(u16 c) {
    u16 backupIME = REG_IME;
    REG_IME = c;
    return backupIME;
}

static s32 DoTest(const char * start, const char * end, u32 expectedValue, ...)
{
    u8 buffer[(size_t)(end - start)];
    va_list va_args;
    u32 resp;
    va_start(va_args, expectedValue);

    {
    u32 * d = (u32 *)buffer;
    const u32 * s = (const u32 *)((uintptr_t)start & ~1);
    const u32 * e = (const u32 *)((uintptr_t)end & ~1);
    while (s < e) {
        *d++ = *s++;
    }
    }
    resp = ((u32 (*)(va_list))buffer)(va_args);
    return resp == expectedValue;
}

static s32 PrefetchBufferTest(void)
{
    s32 result;
    u16 waitCntBackup;
    u16 imeBak;
    u32 prefetch;

    result = 0;
    imeBak = SetIME(0);
    waitCntBackup = REG_WAITCNT;
    REG_WAITCNT = 0x4014;
    prefetch = PrefetchBufferResult_Func();
    if ( prefetch != 24 )
        result |= 1;
    REG_WAITCNT = 0x14;
    prefetch = PrefetchBufferResult_Func();
    if ( prefetch != 51 )
        result |= 2;
    REG_WAITCNT = waitCntBackup;
    SetIME(imeBak);
    return (result != 0) ? 0 : 1; // If result isnt 0, it means it failed. Return the correct error status.
}

static s32 NESPipelineTest(void)
{
    return DoTest(
        (void *)&NESPipelineTest_Internal,
        (void *)&NESPipelineTest_Internal_End,
        255
    );
}

static s32 TimingTest(void)
{
    s32 i, j;
    u32 failMask = 0;
    const u32 expected[] = {
        [TIMER_1CLK]    = 4096,
        [TIMER_64CLK]   = 64,
        [TIMER_256CLK]  = 16,
        [TIMER_1024CLK] = 4
    };
    u32 flagNo = 0;
    u16 imeBak = REG_IME;
    REG_IME = 0;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (!DoTest(
                (void *)&TimerPrescalerTest,
                (void *)&TimerPrescalerTest_End,
                expected[j],
                &REG_TMCNT(i),
                ((j | TIMER_ENABLE) << 16)
            ))
                failMask |= 1 << flagNo;
            flagNo++;
        }
    }

    REG_IME = imeBak;
    return failMask == 0;
}

// Missing libagb BIOS wrappers
s16 ArcTan(s16 x)
{
    register s16 result asm("r0") = x;
    asm("swi 0x09" : "+r"(result) :: "r1","r2","r3","lr","memory","cc");
    return result;
}

s32 DivRemainder(s32 numerator, s32 denominator)
{
    register s32 r0 asm("r0") = numerator;
    register s32 r1 asm("r1") = denominator;
    s32 quotient, remainder;

    asm volatile(
        "swi 0x06"
        : "=r"(quotient), "=r"(remainder)   // outputs
        : "r"(r0), "r"(r1)                  // inputs
        : "r2","r3","lr","memory","cc"
    );

    return remainder;
}

u16 Bios_ArcTan2(s32 x, s32 y)
{
    register s32 r0 asm("r0") = x;
    register s32 r1 asm("r1") = y;
    u16 result;

    asm volatile(
        "swi 0x0A"
        : "=r"(result)
        : "r"(r0), "r"(r1)
        : "r2", "r3", "lr", "memory", "cc"
    );

    return result;
}

struct MathTest {
    const char *name;
    u32 args[8];
};

static const struct MathTest mathTests[] = {
    // ArcTan: simple zero, positive, negative, and max values
    { "ArcTan 00000000", { 0x9, 0x00000000, 0, 0, 0, 0xEF090000, 0xA2F9, 0x0000001F } },
    { "ArcTan 00004000", { 0x9, 0x00004000, 0, 0x2000, 0xFFFFC000, 0xEF090000, 0x8000, 0x0000001F } },
    { "ArcTan FFFF8000", { 0x9, 0xFFFF8000, 0, 0xFFFFE95D, 0xFFFF0000, 0xEF090000, 0x22D45, 0x0000001F } },
    { "ArcTan FFFFFFFF", { 0x9, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xEF090000, 0xA2F9, 0x0000001F } },

    // ArcTan2: minimal quadrant coverage
    { "ArcTan2 00000001,00000000", { 0xA, 0x00000001, 0x00000000, 0, 0, 0xEF0A0000, 0x0170, 0x0000001F } },
    { "ArcTan2 00000001,00000001", { 0xA, 0x00000001, 0x00000001, 0xC000, 0, 0xEF0A0000, 0x0170, 0x0000001F } },
    { "ArcTan2 FFFF8000,00008000", { 0xA, 0xFFFF8000, 0x00008000, 0xC000, 0, 0xEF0A0000, 0x0170, 0x0000001F } },
    { "ArcTan2 FFFFFFFF,FFFF0000", { 0xA, 0xFFFFFFFF, 0xFFFF0000, 0x0000, 0, 0xEF0A0000, 0x0170, 0x0000001F } },

    // // Div: zero, divide by zero, signed division
    { "Div 00000000/00000000", { 0x6, 0x00000000, 0x00000000, 1, 0, 0xEF060000, 1, 0x0000001F } },
    { "Div 00000001/00000000", { 0x6, 0x00000001, 0x00000000, 1, 1, 0xEF060000, 1, 0x0000001F } },
    { "Div FFFFFFFF/00000000", { 0x6, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xEF060000, 1, 0x0000001F } },
    { "Div 80000000/FFFFFFFF", { 0x6, 0x80000000, 0xFFFFFFFF, 0x80000000, 0, 0xEF060000, 0x80000000, 0x0001F } },
};

static bool8 RunSingleMathTest(const struct MathTest *test)
{
    u32 callNum = test->args[0];
    u32 r0in    = test->args[1];
    u32 r1in    = test->args[2];
    u32 r0exp   = test->args[3];
    u32 r1exp   = test->args[4];

    u32 r0out = 0, r1out = 0;

    switch (callNum)
    {
    case 0x9: // ArcTan
        r0out = ArcTan(r0in); // BIOS swi 0x09
        break;
    case 0xA: // ArcTan2
        r0out = Bios_ArcTan2(r0in, r1in); // BIOS swi 0x0A
        break;
    case 0x6: // Div
        r0out = Div(r0in, r1in); // quotient in r0
        r1out = DivRemainder(r0in, r1in); // remainder in r1
        break;
    default:
        return FALSE; // unknown test
    }

    if (r0out != r0exp) {
        DebugPrintfLevel(MGBA_LOG_ERROR, "Maths Test Failed Expecting %x got %x", r0exp, r0out);
        return FALSE;
    }
    if ((callNum == 0x6) && r1out != r1exp) {
        DebugPrintfLevel(MGBA_LOG_ERROR, "Maths Test Failed Expecting %x got %x", r0exp, r0out);
        return FALSE;
    }

    return TRUE;
}

static s32 RunAllMathTests(void)
{
    u32 failures = 0;
    for (u32 i = 0; i < ARRAY_COUNT(mathTests); i++)
    {
        if (!RunSingleMathTest(&mathTests[i]))
            failures |= (1u << i);
    }
    return failures == 0;
}


volatile u16 irqTestVBlankCounter = 0;
volatile u16 irqTestHBlankCounter = 0;

void Test_VBlank_IRQ_Handler(void)
{
    irqTestVBlankCounter++;
}

void Test_HBlank_IRQ_Handler(void)
{
    irqTestHBlankCounter++;
}

static s32 RunIRQTest(void)
{
    EnableInterrupts(INTR_FLAG_HBLANK | INTR_FLAG_VBLANK);
    SetHBlankCallback(Test_HBlank_IRQ_Handler);
    SetVBlankCallback(Test_VBlank_IRQ_Handler);

    u32 timeout = 1000000;

    while (irqTestVBlankCounter < 5 && irqTestHBlankCounter < 5 && timeout--)
    {
        // Wait for interrupts
    }

    return timeout != 0;
}

enum TestList {
    TEST_PREFETCH_BUFFER,
    TEST_TIMER_PRESCALER,
    TEST_INSN_PREFETCH,
    TEST_BIOS_MATHS,
    TEST_IRQ
};

const struct TestSpec gTestSpecs[] = {
    [TEST_PREFETCH_BUFFER]            = {TRUE, "Prefetch Buffer", PrefetchBufferTest},
    [TEST_TIMER_PRESCALER]            = {TRUE, "Timer Prescaler", TimingTest},
    [TEST_INSN_PREFETCH]              = {TRUE, "Inst Prefetch"  , NESPipelineTest},
    [TEST_BIOS_MATHS]                 = {TRUE, "Bios Maths"     , RunAllMathTests},
    [TEST_IRQ]                        = {TRUE, "IRQ"            , RunIRQTest},
};

// Make sure you run this before any sound drivers such as m4a is initialized or you
// will get sound corruption due to manipulating the timers for these tests.
// NB: It's important to return TestsPassed rather than TestsFailed because really bad emus can clear values from ram
u8 RunAgbAccuracyTests()
{
    u8 failureCount = 0;

    for (u8 i = 0; i < NELEMS(gTestSpecs); i++)
    {
        if (!gTestSpecs[i].func()) 
        {
            DebugPrintfLevel(MGBA_LOG_ERROR, "Test Failed: %s", gTestSpecs[i].name);
            failureCount++;
        }
    }
    return ACC_TEST_COUNT - failureCount;
}
