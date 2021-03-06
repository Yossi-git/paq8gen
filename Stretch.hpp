#ifndef PAQ8GEN_STRETCH_HPP
#define PAQ8GEN_STRETCH_HPP

#include "Squash.hpp"
#include <cassert>
#include <cstdint>

/**
 * Inverse of @ref squash. d = ln(p/(1-p)), d scaled by 8 bits, p by 12 bits.
 * d has range -2047 to 2047 representing -8 to 8. p has range 0 to 4095.
 */
#ifndef CHALLENGE
struct Stretch {
    constexpr Stretch() {
      int pi = 0;
      for( int x = -2047; x <= 2047; ++x ) {
        int i = squash(x);
        for( int j = pi; j <= i; ++j ) {
          t[j] = static_cast<short>(x);
        }
        pi = i + 1;
      }
      t[4095] = 2047;
    }

    short t[4096] {};
};

constexpr auto stretch = Stretch();
#define stretch(x) stretch.t[x]

#else
#include "Array.hpp"
class Stretch {
    Array<short> t;
private:
    Stretch() : t( 4096 ) {
      int pi = 0;
      for( int x = -2047; x <= 2047; ++x ) { // invert squash()
        int i = squash( x );
        for( int j = pi; j <= i; ++j )
          t[j] = x;
        pi = i + 1;
      }
      t[4095] = 2047;
    }
    Stretch(Stretch const & /*unused*/);
    auto operator=(Stretch const & /*unused*/) -> Stretch & { return *this; }
    static Stretch * instance;

public:
    static auto getInstance() -> Stretch&;
    int operator()( int p ) const {
      assert( p >= 0 && p < 4096 );
      return t[p];
    }
};

#define stretch(x) stretch->operator()(x)

#endif

#endif //PAQ8GEN_STRETCH_HPP
