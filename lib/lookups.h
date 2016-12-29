extern const uint8_t BIT_REVERSE_TABLE[256];


/// to set bit p in buffer to 1, use buffer |= ONE_MASKS[p]
extern const uint32_t ONE_MASKS[32];


/// to set bit p in buffer to 0, use buffer &= ZERO_MASKS[p]
extern const uint32_t ZERO_MASKS[32];
