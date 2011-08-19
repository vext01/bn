#ifndef BN_H_
#define BN_H_

union bnum {
	uint8_t		uint8;
	int8_t		int8;
	uint16_t	uint16;
	int16_t		int16;
	uint32_t	uint32;
	int32_t		int32;
	uint64_t	uint64;
	int64_t		int64;
};

#endif
