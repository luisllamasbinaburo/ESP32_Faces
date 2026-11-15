// Minimal Arduino API so the library can be compiled and run on a PC.
// Time is simulated: the simulator advances it explicitly with SimAdvance().
#ifndef SIM_ARDUINO_h
#define SIM_ARDUINO_h

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern unsigned long g_simMillis;

inline unsigned long millis() { return g_simMillis; }
inline void SimAdvance(unsigned long ms) { g_simMillis += ms; }
inline void yield() { SimAdvance(1); }
inline void delay(unsigned long ms) { SimAdvance(ms); }

inline void randomSeed(unsigned long seed) { srand((unsigned)seed); }
inline long random(long howbig) { return howbig <= 0 ? 0 : (long)(rand() % howbig); }
inline long random(long howsmall, long howbig) { return howsmall >= howbig ? howsmall : howsmall + random(howbig - howsmall); }

#ifndef constrain
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#endif

template <class A, class B> inline auto min(A a, B b) -> decltype(a < b ? a : b) { return a < b ? a : b; }
template <class A, class B> inline auto max(A a, B b) -> decltype(a > b ? a : b) { return a > b ? a : b; }

#endif
