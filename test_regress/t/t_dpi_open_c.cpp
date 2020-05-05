// -*- mode: C++; c-file-style: "cc-mode" -*-
//*************************************************************************
//
// Copyright 2009-2017 by Wilson Snyder. This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
// SPDX-License-Identifier: LGPL-3.0-only OR Artistic-2.0
//
//*************************************************************************

#include <cstdio>
#include <cstring>
#include <iostream>
#include "svdpi.h"

//======================================================================

#if defined(VERILATOR)
# include "Vt_dpi_open__Dpi.h"
#elif defined(VCS)
# include "../vc_hdrs.h"
#elif defined(NC)
# define NEED_EXTERNS
#elif defined(MS)
# define NEED_EXTERNS
#else
# error "Unknown simulator for DPI test"
#endif

#ifdef NEED_EXTERNS
extern "C" {
    // If get ncsim: *F,NOFDPI: Function {foo} not found in default libdpi.
    // Then probably forgot to list a function here.

    extern void dpii_unused(const svOpenArrayHandle u);

    extern void dpii_open_p0_u1(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_p1_u0(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_p1_u1(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_p1_u2(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_p1_u3(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_pw_u0(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_pw_u1(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_pw_u2(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_pw_u3(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o);

    extern void dpii_open_bit(const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_byte(const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_int(const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_integer(const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_logic(const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_byte_order(const svOpenArrayHandle i, const svOpenArrayHandle o);
    extern void dpii_open_int_order(const svOpenArrayHandle i, const svOpenArrayHandle o);

    extern int dpii_failure();
}
#endif

//======================================================================

// Untested:
// void *svGetArrElemPtr(const svOpenArrayHandle, int indx1, ...);
// void svPutBitArrElemVecVal(const svOpenArrayHandle d, const svBitVecVal* s, int indx1, ...);
// void svPutBitArrElem1VecVal(const svOpenArrayHandle d, const svBitVecVal* s, int indx1);
// void svPutLogicArrElemVecVal(const svOpenArrayHandle d, const svLogicVecVal* s, int indx1, ...);
// void svPutLogicArrElem1VecVal(const svOpenArrayHandle d, const svLogicVecVal* s, int indx1);
// void svGetBitArrElemVecVal(svBitVecVal* d, const svOpenArrayHandle s, int indx1, ...);
// void svGetBitArrElem1VecVal(svBitVecVal* d, const svOpenArrayHandle s, int indx1);
// void svGetLogicArrElemVecVal(svLogicVecVal* d, const svOpenArrayHandle s, int indx1, ...);
// void svGetLogicArrElem1VecVal(svLogicVecVal* d, const svOpenArrayHandle s, int indx1);
// svBit svGetBitArrElem(const svOpenArrayHandle s, int indx1, ...);
// svBit svGetBitArrElem1(const svOpenArrayHandle s, int indx1);
// svLogic svGetLogicArrElem(const svOpenArrayHandle s, int indx1, ...);
// svLogic svGetLogicArrElem1(const svOpenArrayHandle s, int indx1);
// void svPutBitArrElem(const svOpenArrayHandle d, svBit value, int indx1, ...);
// void svPutBitArrElem1(const svOpenArrayHandle d, svBit value, int indx1);
// void svPutLogicArrElem(const svOpenArrayHandle d, svLogic value, int indx1, ...);
// void svPutLogicArrElem1(const svOpenArrayHandle d, svLogic value, int indx1);

//======================================================================

int failure = 0;
int dpii_failure() { return failure; }

#ifdef _WIN32
# define T_PRI64 "I64"
#else  // Linux or compliant Unix flavors
# define T_PRI64 "ll"
#endif

#define CHECK_RESULT_HEX(got, exp) \
    do { \
        if ((got) != (exp)) { \
            std::cout << std::dec << "%Error: " << __FILE__ << ":" << __LINE__ << std::hex \
                      << ": GOT=" << (long)(got) << "   EXP=" << (exp) << std::endl; \
            failure = __LINE__; \
        } \
    } while (0)

#define CHECK_RESULT_HEX_NE(got, exp) \
    do { \
        if ((got) == (exp)) { \
            std::cout << std::dec << "%Error: " << __FILE__ << ":" << __LINE__ << std::hex \
                      << ": GOT=" << (long)(got) << "   EXP!=" << (exp) << std::endl; \
            failure = __LINE__; \
        } \
    } while (0)

void dpii_unused(const svOpenArrayHandle u) {}

void _dpii_all(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o) {
#ifdef TEST_VERBOSE
    printf("-:%s:%d: For case c=%d p=%d u=%d  data=%p\n",
           __FILE__, __LINE__, c, p, u, svGetArrayPtr(i));
#endif
    (void)svGetArrayPtr(i);
#ifndef NC
    // NC always returns zero and warns
    (void)svSizeOfArray(i);
#endif
#ifndef VCS  // VCS does not support dimension 0 query
    // if (p) {
    //     int d = 0;
    //     if (c == 0 || c == 1) {
    //         CHECK_RESULT_HEX(svLeft(i, d), 1);
    //         CHECK_RESULT_HEX(svRight(i, d), -1);
    //         CHECK_RESULT_HEX(svLow(i, d), -1);
    //         CHECK_RESULT_HEX(svHigh(i, d), 1);
    //         // CHECK_RESULT_HEX(svIncrement(i, d), 0);
    //         CHECK_RESULT_HEX(svSize(i, d), 3);
    //     } else if (c == 2) {
    //         CHECK_RESULT_HEX(svLeft(i, d), 95);
    //         CHECK_RESULT_HEX(svRight(i, d), 1);
    //         CHECK_RESULT_HEX(svLow(i, d), 1);
    //         CHECK_RESULT_HEX(svHigh(i, d), 95);
    //         // CHECK_RESULT_HEX(svIncrement(i, d), 0);
    //         CHECK_RESULT_HEX(svSize(i, d), 95);
    //     } else {
    //         CHECK_RESULT_HEX(0, 1);
    //     }
    // }
#endif
    if (u >= 1) {
        int d = 1;
        if (c == 0) {
            CHECK_RESULT_HEX(svLeft(i, d), -2);
            CHECK_RESULT_HEX(svRight(i, d), 2);
            CHECK_RESULT_HEX(svLow(i, d), -2);
            CHECK_RESULT_HEX(svHigh(i, d), 2);
            // CHECK_RESULT_HEX(svIncrement(i, d), 0);
            CHECK_RESULT_HEX(svSize(i, d), 5);
        } else if (c == 1) {
            CHECK_RESULT_HEX(svLeft(i, d), 2);
            CHECK_RESULT_HEX(svRight(i, d), -2);
            CHECK_RESULT_HEX(svLow(i, d), -2);
            CHECK_RESULT_HEX(svHigh(i, d), 2);
            // CHECK_RESULT_HEX(svIncrement(i, d), 0);
            CHECK_RESULT_HEX(svSize(i, d), 5);
        }
    }
    if (u >= 2) {
        int d = 2;
        if (c == 0) {
            CHECK_RESULT_HEX(svLeft(i, d), -3);
            CHECK_RESULT_HEX(svRight(i, d), 3);
            CHECK_RESULT_HEX(svLow(i, d), -3);
            CHECK_RESULT_HEX(svHigh(i, d), 3);
            // CHECK_RESULT_HEX(svIncrement(i, d), 0);
            CHECK_RESULT_HEX(svSize(i, d), 7);
        } else if (c == 1) {
            CHECK_RESULT_HEX(svLeft(i, d), 3);
            CHECK_RESULT_HEX(svRight(i, d), -3);
            CHECK_RESULT_HEX(svLow(i, d), -3);
            CHECK_RESULT_HEX(svHigh(i, d), 3);
            // CHECK_RESULT_HEX(svIncrement(i, d), 0);
            CHECK_RESULT_HEX(svSize(i, d), 7);
        }
    }

    if (c == 2 && p == 1 && u == 3) {
        for (int a = svLow(i, 1); a <= svHigh(i, 1); ++a) {
            for (int b = svLow(i, 2); b <= svHigh(i, 2); ++b) {
                for (int c = svLow(i, 3); c <= svHigh(i, 3); ++c) {
                    // printf("Copy abc %d,%d,%d\n", a,b,c);
                    svLogicVecVal vec[3];
                    svGetLogicArrElemVecVal(vec, i, a, b, c);
#ifdef NC
                    // printf("   %08lx_%08lx_%08lx\n", vec[2].a, vec[1].a, vec[0].a);
                    vec[0].a = (~vec[0].a);
                    vec[1].a = (~vec[1].a);
                    vec[2].a = (~vec[2].a) & 0x7fffffff;
                    // vec[0].b = vec[0].b;
                    // vec[1].b = vec[1].b;
                    // vec[2].b = vec[2].b;
#else
                    vec[0].aval = (~vec[0].aval);
                    vec[1].aval = (~vec[1].aval);
                    vec[2].aval = (~vec[2].aval) & 0x7fffffff;
                    // vec[0].bval = vec[0].bval;
                    // vec[1].bval = vec[1].bval;
                    // vec[2].bval = vec[2].bval;
#endif
                    svPutLogicArrElemVecVal(o, vec, a, b, c);
                }
            }
        }
    }
}

void dpii_open_p0_u1(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o) {
    _dpii_all(c, p, u, i, o);
}
void dpii_open_p1_u0(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o) {
    _dpii_all(c, p, u, i, o);
}
void dpii_open_p1_u1(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o) {
    _dpii_all(c, p, u, i, o);
}
void dpii_open_p1_u2(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o) {
    _dpii_all(c, p, u, i, o);
}
void dpii_open_p1_u3(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o) {
    _dpii_all(c, p, u, i, o);
}
void dpii_open_pw_u0(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o) {
    _dpii_all(c, p, u, i, o);
}
void dpii_open_pw_u1(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o) {
    _dpii_all(c, p, u, i, o);
}
void dpii_open_pw_u2(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o) {
    _dpii_all(c, p, u, i, o);
}
void dpii_open_pw_u3(int c, int p, int u, const svOpenArrayHandle i, const svOpenArrayHandle o) {
    _dpii_all(c, p, u, i, o);
}

void dpii_open_bit(const svOpenArrayHandle i, const svOpenArrayHandle o) {}

void dpii_open_byte(const svOpenArrayHandle i, const svOpenArrayHandle o) {
    intptr_t arrPtr = (intptr_t)svGetArrayPtr(i);
    CHECK_RESULT_HEX_NE(arrPtr, 0); // All the arrays should actually exist
#ifndef NC
    // NC always returns zero and warns
    int sizeInputOfArray = svSizeOfArray(i);
    CHECK_RESULT_HEX_NE(sizeInputOfArray, 0); // None of the test cases have zero size
    CHECK_RESULT_HEX_NE(svDimensions(i), 0); // All the test cases are unpacked arrays
#endif
}

void dpii_open_int(const svOpenArrayHandle i, const svOpenArrayHandle o) {
    intptr_t arrPtr = (intptr_t)svGetArrayPtr(i);
    CHECK_RESULT_HEX_NE(arrPtr, 0); // All the arrays should actually exist
#ifndef NC
    // NC always returns zero and warns
    int sizeInputOfArray = svSizeOfArray(i);
    CHECK_RESULT_HEX_NE(sizeInputOfArray, 0); // None of the test cases have zero size
    CHECK_RESULT_HEX_NE(svDimensions(i), 0); // All the test cases are unpacked arrays
#endif
}

void dpii_open_integer(const svOpenArrayHandle i, const svOpenArrayHandle o) {}
void dpii_open_logic(const svOpenArrayHandle i, const svOpenArrayHandle o) {}

void dpii_open_byte_order(const svOpenArrayHandle i, const svOpenArrayHandle o) {
#ifndef NC
    uint8_t* arrPtr = (uint8_t*)svGetArrayPtr(i);
    CHECK_RESULT_HEX_NE(arrPtr, 0); // All the arrays should actually exist
    int sizeInputOfArray = svSizeOfArray(i);
    CHECK_RESULT_HEX(sizeInputOfArray, 13); // None of the test cases have zero size
    CHECK_RESULT_HEX_NE(svDimensions(i), 0); // All the test cases are unpacked arrays

    int j;
    for (j=0; j<13; j++)
    {
        CHECK_RESULT_HEX(arrPtr[j], 12-j);
    }

    uint8_t* outArrPtr = (uint8_t*)svGetArrayPtr(o);
    CHECK_RESULT_HEX_NE(outArrPtr, 0); // All the arrays should actually exist
    int sizeOutputOfArray = svSizeOfArray(o);
    CHECK_RESULT_HEX(sizeOutputOfArray, 11); // None of the test cases have zero size
    CHECK_RESULT_HEX_NE(svDimensions(o), 0); // All the test cases are unpacked arrays

    for (j=0; j<sizeOutputOfArray; j++)
    {
        outArrPtr[j] = arrPtr[j] + 5;
    }

#endif
}

void dpii_open_int_order(const svOpenArrayHandle i, const svOpenArrayHandle o) {
#ifndef NC
    int* arrPtr = (int*)svGetArrayPtr(i);
    CHECK_RESULT_HEX_NE(arrPtr, 0); // All the arrays should actually exist
    int sizeInputOfArray = svSizeOfArray(i);
    CHECK_RESULT_HEX(sizeInputOfArray, 52); // None of the test cases have zero size
    CHECK_RESULT_HEX_NE(svDimensions(i), 0); // All the test cases are unpacked arrays

    int j;
    for (j=0; j<13; j++)
    {
        int inverseIdx = 12-j;
        CHECK_RESULT_HEX(arrPtr[j],
            (inverseIdx << 24) | ((inverseIdx+1) << 16) |
            ((inverseIdx+2) << 8) | ((inverseIdx+3) << 0) );
    }

    int* outArrPtr = (int*)svGetArrayPtr(o);
    CHECK_RESULT_HEX_NE(outArrPtr, 0); // All the arrays should actually exist
    int sizeOutputOfArray = svSizeOfArray(o);
    CHECK_RESULT_HEX(sizeOutputOfArray, 44); // None of the test cases have zero size
    CHECK_RESULT_HEX_NE(svDimensions(o), 0); // All the test cases are unpacked arrays

    for (j=0; j<(sizeOutputOfArray/sizeof(int)); j++)
    {
        outArrPtr[j] = arrPtr[j] + 5;
    }

#endif
}



int dpii_failed() { return failure; }
