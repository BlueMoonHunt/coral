#include "uuid.h"
#include <random>

static std::random_device s_RD;
static std::mt19937_64 s_Engine(s_RD());
static std::uniform_int_distribution<uint64_t> s_Dist;


coral::UUID::UUID()
    : m_UUID(s_Dist(s_Engine)) {
}