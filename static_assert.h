#pragma once

template<bool val>
struct StaticAssert { };

template<>
struct StaticAssert<false>;

#define STATIC_ASSERT(val) StaticAssert<val>()

