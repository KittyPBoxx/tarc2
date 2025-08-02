#ifndef _AGB_ACCURACY_H
#define _AGB_ACCURACY_H

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
