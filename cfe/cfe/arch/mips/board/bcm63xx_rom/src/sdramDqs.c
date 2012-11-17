/***************************************************************************
*
*  Copyright (c) 2004 Broadcom Corporation, All Rights Reserved.
*  Contains proprietary and confidential information.
*
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*
*        Broadcom Corporation
*        16215 Alton Parkway
*        P.O. Box 57013
*        Irvine, California 92619-7013
*
*  All information contained in this document is Broadcom Corporation
*  company private, proprietary, and trade secret.
*
****************************************************************************/
#include "lib_types.h"
#include "bcm_map.h"

/* ---- Private Constants and Types -------------------------------------- */
#define VCDL_PHASE_DEFAULT    16
#define VCDL_PHASE_MAX        31
#define VCDL_PHASE_MIN        0

/* ---- Private Function Prototypes -------------------------------------- */
static int MemoryTest2(void);
static int MemoryTest3(void);
static void PI_upper_set(volatile uint32 *, int);
static void PI_shmoo(volatile uint32 *PI_reg, int minValue, int maxValue, int increment);
static void shmooVcdl(int minValue, int maxValue, int increment);
 
#if DEBUG_SHMOO
static void dumpChar(uint8 c)
{
    //wait till tx fifo below threshold
    while (UART->txf_levl > 14);
    UART->Data = c;
}

static void dumpString(uint8 *s)
{
    while (*s != 0) {
        dumpChar(*s++);
    }
    dumpChar('\r');
    dumpChar('\n');
}
#else
#define dumpChar(c)
#define dumpString(s)
#endif

/* ==== Public Functions ================================================= */
void sdramDqsPhaseSet(void);

/*******************************************************************************/
void sdramDqsPhaseSet(void)
{
    int  dqOutPhaseMax;

    // Turn on automatic PI counters.
    DDR->MIPSPhaseCntl = 0x77170000;

    // Calculate max phase offset from PLL config register.
    dqOutPhaseMax = ((DDR->MIPSDDRPLLConfig & DDR_MDIV_MASK) >> DDR_MDIV_SHFT) * 8;

    dumpChar('\n');
    dumpChar('\r');

    // Start by setting VCDL to the default.  This almost always works.
    DDR->VCDLPhaseCntl = (VCDL_PHASE_DEFAULT << 8) | VCDL_PHASE_DEFAULT;

    // Now shmoo over DQ phase to find an optimum value.
    dumpString("DDR2");
    PI_shmoo(&DDR->DDR1_2PhaseCntl0, -dqOutPhaseMax, dqOutPhaseMax, 1);

    dumpString("VCDL");
    shmooVcdl(VCDL_PHASE_MIN, VCDL_PHASE_MAX, 1);
}

/* ==== Private Functions ================================================ */

static void PI_upper_set(volatile uint32 *PI_reg, int shift)
{
    uint32  oldVal;
    uint32  saveVal;
    int32   oldPhase;
    int32   newPhase;
    uint32  newVal;
    int     equalCount      = 0;

    oldVal   = *PI_reg;
    // Save current lower PI value
    saveVal  = oldVal & 0xffff;

    // Shift the sign bit to bit 31, and sign extend
    oldPhase = oldVal << 3;
    oldPhase = oldPhase  >> 19;

    newPhase = shift & 0x1fff;
    if (shift > oldPhase)
        newPhase = newPhase | 0x2000;

    newVal = saveVal | (newPhase << 16);
    *PI_reg = newVal;

    // Wait until we match several times in a row, to be sure we wait long enough
    do {
        if (*PI_reg == newVal)
            equalCount++;
        else
            equalCount = 0;
    } while (equalCount < 3);
}


static void PI_shmoo(volatile uint32 *PI_reg, int minValue, int maxValue, int increment)
{
    int piPhase;
    int passSum      = 0;
    int passCnt      = 0;
    int i;

    PI_upper_set(PI_reg, minValue);

    for (piPhase =  minValue; piPhase <= maxValue; piPhase += increment) {
        if (MemoryTest2() && MemoryTest3()) {
            passSum += piPhase;
            passCnt++;
            dumpChar('p');
        }
        else {
            dumpChar('.');
        }

        for (i = 0; i < increment; i++)
            *PI_reg = (*PI_reg + 0x10000) | 0x20000000;

    }

    if (passCnt != 0) {
        piPhase = passSum / passCnt;
        PI_upper_set(PI_reg, piPhase);
    }
    else {
        PI_upper_set(PI_reg, 0);
    }

    dumpChar('\n');
    dumpChar('\r');
}


static void shmooVcdl(int minValue, int maxValue, int increment)
{
    UINT32              dqsInPhase;
    UINT32              dqsInSum        = 0;
    UINT32              passCnt         = 0;

    for (dqsInPhase = minValue; dqsInPhase <= maxValue; dqsInPhase += increment) {
        DDR->VCDLPhaseCntl = (dqsInPhase << 8) | dqsInPhase;
        if (MemoryTest2() && MemoryTest3()) {
            dqsInSum += dqsInPhase;
            passCnt++;
            dumpChar('p');
        }
        else {
            dumpChar('.');
        }
    }
    if (passCnt > 0) {
        dqsInPhase  = dqsInSum / passCnt;
        DDR->VCDLPhaseCntl = (dqsInPhase << 8) | dqsInPhase;
    }
    else {
        DDR->VCDLPhaseCntl = (VCDL_PHASE_DEFAULT << 8) | VCDL_PHASE_DEFAULT;
    }

    dumpChar('\n');
    dumpChar('\r');
}

// This is a memory test of a sliding 1 address (2, 8, 16, ... 8M)
static int MemoryTest2(void)
{
    // Test 16-bit write/read
    volatile uint16 *memAddr;
    uint32           memBase;
    uint16           testValue;
    int              i;
    int              j;
    int              k;

    memBase = 0xa0000000;
    for (i = 1; i < 24; i++) {
        memAddr = (void *) (memBase + (1 << i));
        for (k = 0; k < 2; k++) {
            testValue = 1;
            for (j = 0; j < 16; j++) {
                *memAddr = testValue;
                if (*memAddr != testValue)
                    return 0;

                testValue = (testValue << 1);
            }
            testValue = 0xfffe;
            for (j = 0; j < 16; j++) {
                *memAddr = testValue;
                if (*memAddr != testValue)
                    return 0;

                testValue = (testValue << 1) | 1;
            }
        }
    }
    return 1;
}

static int MemoryTest3(void)
{
    uint32  base     = 0xa0000000;
    uint32 *membase  = (uint32 *) base;
    uint32  offset;
    uint32 *testaddr;

    offset = 0x00800000;
    while (offset >= 4) {
        uint32 testvalue;
        testaddr = (uint32 *)(offset + base);
        testvalue = 0xffff;
        while (testvalue != 0) {
            *testaddr = testvalue;
            *membase  = 0;
            if (*testaddr != testvalue)
                return 0;

            testvalue <<= 1;
        }
        offset >>= 1;
    }
    offset = 0x00800000;
    while (offset >= 4) {
        uint32 testvalue;
        testaddr = (uint32 *)(offset + base);
        testvalue = 1;
        while (testvalue != 0) {
            *testaddr = testvalue;
            *membase  = 0;
            if (*testaddr != testvalue)
                return 0;

            testvalue <<= 1;
        }
        offset >>= 1;
    }
    return 1;
}
