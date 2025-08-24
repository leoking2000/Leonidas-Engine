#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// int types

typedef char i8; // 8 bit signed integer
typedef unsigned char u8; // 8 bit unsigned integer

typedef short i16; // 16 bit signed integer
typedef unsigned short u16; // 16 bit unsigned integer

typedef int i32; // 32 bit signed integer
typedef unsigned int u32; // 32 bit unsigned integer

typedef long long i64; // 64 bit signed integer
typedef unsigned long long u64; // 64 bit unsigned integer

// float types

typedef float  f32; // 32 bit float
typedef double f64; // 64 bit float

static_assert(sizeof(i8) == 1);
static_assert(sizeof(u8) == 1);

static_assert(sizeof(i16) == 2);
static_assert(sizeof(u16) == 2);

static_assert(sizeof(i32) == 4);
static_assert(sizeof(u32) == 4);

static_assert(sizeof(i64) == 8);
static_assert(sizeof(u64) == 8);

static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);
