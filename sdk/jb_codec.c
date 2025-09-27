#include "jb_codec.h"

jb_result_t jb_codec_decode_general_int(uint8_t** buff, uint64_t* remaining, uint64_t* out) {
	if (!out)
		return JB_ERR_NO_OUT;
	if (!buff || *remaining < 1)
		return JB_ERR_NO_DATA;

	// Case: zero
	uint8_t x0 = **buff;

	// advance pointer and len
	*buff += 1;
	*remaining -= 1;

	if (x0 == 0) {
		*out = 0;
		return JB_OK;
	}

	// Case: 1 byte value
	if ((x0 & 0x80) == 0) {
		*out = x0;
		return JB_OK;
	}

	// Case: 8 byte value
	if (x0 == 0xFF) {
		if (*remaining < 8)
			return JB_ERR_NO_DATA;

		// PVM is a little endian target so we wont bother with the possibility of
		// big endian targets.
		*out = *(uint64_t*)*buff;
		*buff += 8;
	}

	// TODO use gnu89 range switch
	uint8_t l = 0, m = 0;
	if (x0 < 128)
		return JB_ERR_UNREACHABLE; // already checked
	else if (x0 < 192)
		l = 1, m = 128;
   	else if (x0 < 224)
		l = 2, m = 192;
    else if (x0 < 240)
		l = 3, m = 224;
    else if (x0 < 248)
		l = 4, m = 240;
    else if (x0 < 252)
		l = 5, m = 248;
    else if (x0 < 254)
		l = 6, m = 252;
    else if (x0 < 255)
		l = 7, m = 254;
	else
		return JB_ERR_UNDECODABLE;

	if (*remaining < l)
		return JB_ERR_NO_DATA;

	uint64_t v = 0;
	for (uint64_t i = 0; i < l; i++) {
		v += (uint64_t)*buff << (l - i);
		*buff += 1;
	}

	uint64_t s = x0 - m;
	v += s << (8 * l);

	*out = v;
	return JB_OK;
}
