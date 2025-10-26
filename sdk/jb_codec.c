#include "jb_codec.h"

#include <string.h>
#include <stdarg.h>

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
		*remaining -= 8;
		return JB_OK;
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
		uint64_t number = (uint64_t)**buff;
		v += number << (8 * i);
		*buff += 1;
		*remaining -= 1;
	}

	uint64_t s = x0 - m;
	v += s << (8 * l);

	*out = v;
	return JB_OK;
}

// === Basic type decoders ===

jb_result_t jb_codec_decode_bool(uint8_t** buff, uint64_t* remaining, bool* out) {
	if (!out)
		return JB_ERR_NO_OUT;
	if (!buff || *remaining < 1)
		return JB_ERR_NO_DATA;
	
	uint8_t value = **buff;
	*buff += 1;
	*remaining -= 1;
	
	if (value == 0) {
		*out = false;
		return JB_OK;
	} else if (value == 1) {
		*out = true;
		return JB_OK;
	}
	
	return JB_ERR_UNDECODABLE;
}

jb_result_t jb_codec_decode_u8(uint8_t** buff, uint64_t* remaining, uint8_t* out) {
	if (!out)
		return JB_ERR_NO_OUT;
	if (!buff || *remaining < 1)
		return JB_ERR_NO_DATA;
	
	*out = **buff;
	*buff += 1;
	*remaining -= 1;
	return JB_OK;
}

jb_result_t jb_codec_decode_u16(uint8_t** buff, uint64_t* remaining, uint16_t* out) {
	if (!out)
		return JB_ERR_NO_OUT;
	if (!buff || *remaining < 2)
		return JB_ERR_NO_DATA;
	
	*out = *(uint16_t*)*buff;
	*buff += 2;
	*remaining -= 2;
	return JB_OK;
}

jb_result_t jb_codec_decode_u24(uint8_t** buff, uint64_t* remaining, uint32_t* out) {
	if (!out)
		return JB_ERR_NO_OUT;
	if (!buff || *remaining < 3)
		return JB_ERR_NO_DATA;
	
	*out = ((uint32_t)(*buff)[0]) | 
	       ((uint32_t)(*buff)[1] << 8) | 
	       ((uint32_t)(*buff)[2] << 16);
	*buff += 3;
	*remaining -= 3;
	return JB_OK;
}

jb_result_t jb_codec_decode_u32(uint8_t** buff, uint64_t* remaining, uint32_t* out) {
	if (!out)
		return JB_ERR_NO_OUT;
	if (!buff || *remaining < 4)
		return JB_ERR_NO_DATA;
	
	*out = *(uint32_t*)*buff;
	*buff += 4;
	*remaining -= 4;
	return JB_OK;
}

jb_result_t jb_codec_decode_u64(uint8_t** buff, uint64_t* remaining, uint64_t* out) {
	if (!out)
		return JB_ERR_NO_OUT;
	if (!buff || *remaining < 8)
		return JB_ERR_NO_DATA;
	
	*out = *(uint64_t*)*buff;
	*buff += 8;
	*remaining -= 8;
	return JB_OK;
}

// === Binary/string decoders ===

jb_result_t jb_codec_decode_binary(uint8_t** buff, uint64_t* remaining, uint8_t** out_data, uint64_t* out_len) {
	if (!out_data || !out_len)
		return JB_ERR_NO_OUT;
	
	// First decode the length as a general integer
	uint64_t len;
	jb_result_t result = jb_codec_decode_general_int(buff, remaining, &len);
	if (result != JB_OK)
		return result;
	
	if (*remaining < len)
		return JB_ERR_NO_DATA;
	
	*out_data = *buff;
	*out_len = len;
	*buff += len;
	*remaining -= len;
	return JB_OK;
}

jb_result_t jb_codec_decode_bounded_binary(uint8_t** buff, uint64_t* remaining, uint8_t** out_data, uint64_t* out_len, uint64_t max_len) {
	jb_result_t result = jb_codec_decode_binary(buff, remaining, out_data, out_len);
	if (result != JB_OK)
		return result;
	
	if (*out_len > max_len)
		return JB_ERR_UNDECODABLE;
	
	return JB_OK;
}

jb_result_t jb_codec_decode_fixed_binary(uint8_t** buff, uint64_t* remaining, uint8_t* out_data, uint64_t expected_len) {
	if (!out_data)
		return JB_ERR_NO_OUT;
	if (!buff || *remaining < expected_len)
		return JB_ERR_NO_DATA;
	
	memcpy(out_data, *buff, expected_len);
	*buff += expected_len;
	*remaining -= expected_len;
	return JB_OK;
}

// === Option/Maybe decoder ===

jb_result_t jb_codec_decode_maybe(uint8_t** buff, uint64_t* remaining, bool* out_has_value) {
	if (!out_has_value)
		return JB_ERR_NO_OUT;
	if (!buff || *remaining < 1)
		return JB_ERR_NO_DATA;
	
	uint8_t discriminator = **buff;
	*buff += 1;
	*remaining -= 1;
	
	if (discriminator == 0) {
		*out_has_value = false;
		return JB_OK;
	} else if (discriminator == 1) {
		*out_has_value = true;
		return JB_OK;
	}
	
	return JB_ERR_UNDECODABLE;
}

// === List decoder ===

jb_result_t jb_codec_decode_list_len(uint8_t** buff, uint64_t* remaining, uint64_t* out_len) {
	return jb_codec_decode_general_int(buff, remaining, out_len);
}

// === Basic type encoders ===

jb_result_t jb_codec_encode_general_int(uint8_t** buff, uint64_t* remaining, uint64_t value) {
	if (!buff || !remaining)
		return JB_ERR_NO_OUT;
	
	// Case: zero
	if (value == 0) {
		if (*remaining < 1)
			return JB_ERR_NO_DATA;
		**buff = 0;
		*buff += 1;
		*remaining -= 1;
		return JB_OK;
	}
	
	// Case: 1 byte value (0-127)
	if (value < 128) {
		if (*remaining < 1)
			return JB_ERR_NO_DATA;
		**buff = (uint8_t)value;
		*buff += 1;
		*remaining -= 1;
		return JB_OK;
	}
	
	// Case: 8 byte value
	if (value >= 0x100000000000000ULL) {
		if (*remaining < 9)
			return JB_ERR_NO_DATA;
		**buff = 0xFF;
		*buff += 1;
		*remaining -= 1;
		*(uint64_t*)*buff = value;
		*buff += 8;
		*remaining -= 8;
		return JB_OK;
	}
	
	// Variable length encoding
	uint8_t l = 0, m = 0;
	if (value < (1ULL << 14)) {
		l = 1; m = 128;
	} else if (value < (1ULL << 22)) {
		l = 2; m = 192;
	} else if (value < (1ULL << 30)) {
		l = 3; m = 224;
	} else if (value < (1ULL << 38)) {
		l = 4; m = 240;
	} else if (value < (1ULL << 46)) {
		l = 5; m = 248;
	} else if (value < (1ULL << 54)) {
		l = 6; m = 252;
	} else {
		l = 7; m = 254;
	}
	
	if (*remaining < (uint64_t)(l + 1))
		return JB_ERR_NO_DATA;
	
	uint64_t s = (value >> (8 * l));
	**buff = (uint8_t)(m + s);
	*buff += 1;
	*remaining -= 1;
	
	for (uint64_t i = 0; i < l; i++) {
		**buff = (uint8_t)(value >> (8 * i));
		*buff += 1;
		*remaining -= 1;
	}
	
	return JB_OK;
}

jb_result_t jb_codec_encode_bool(uint8_t** buff, uint64_t* remaining, bool value) {
	if (!buff || !remaining)
		return JB_ERR_NO_OUT;
	if (*remaining < 1)
		return JB_ERR_NO_DATA;
	
	**buff = value ? 1 : 0;
	*buff += 1;
	*remaining -= 1;
	return JB_OK;
}

jb_result_t jb_codec_encode_u8(uint8_t** buff, uint64_t* remaining, uint8_t value) {
	if (!buff || !remaining)
		return JB_ERR_NO_OUT;
	if (*remaining < 1)
		return JB_ERR_NO_DATA;
	
	**buff = value;
	*buff += 1;
	*remaining -= 1;
	return JB_OK;
}

jb_result_t jb_codec_encode_u16(uint8_t** buff, uint64_t* remaining, uint16_t value) {
	if (!buff || !remaining)
		return JB_ERR_NO_OUT;
	if (*remaining < 2)
		return JB_ERR_NO_DATA;
	
	*(uint16_t*)*buff = value;
	*buff += 2;
	*remaining -= 2;
	return JB_OK;
}

jb_result_t jb_codec_encode_u24(uint8_t** buff, uint64_t* remaining, uint32_t value) {
	if (!buff || !remaining)
		return JB_ERR_NO_OUT;
	if (*remaining < 3)
		return JB_ERR_NO_DATA;
	
	(*buff)[0] = (uint8_t)(value & 0xFF);
	(*buff)[1] = (uint8_t)((value >> 8) & 0xFF);
	(*buff)[2] = (uint8_t)((value >> 16) & 0xFF);
	*buff += 3;
	*remaining -= 3;
	return JB_OK;
}

jb_result_t jb_codec_encode_u32(uint8_t** buff, uint64_t* remaining, uint32_t value) {
	if (!buff || !remaining)
		return JB_ERR_NO_OUT;
	if (*remaining < 4)
		return JB_ERR_NO_DATA;
	
	*(uint32_t*)*buff = value;
	*buff += 4;
	*remaining -= 4;
	return JB_OK;
}

jb_result_t jb_codec_encode_u64(uint8_t** buff, uint64_t* remaining, uint64_t value) {
	if (!buff || !remaining)
		return JB_ERR_NO_OUT;
	if (*remaining < 8)
		return JB_ERR_NO_DATA;
	
	*(uint64_t*)*buff = value;
	*buff += 8;
	*remaining -= 8;
	return JB_OK;
}

// === Binary/string encoders ===

jb_result_t jb_codec_encode_binary(uint8_t** buff, uint64_t* remaining, const uint8_t* data, uint64_t len) {
	if (!data && len > 0)
		return JB_ERR_NO_OUT;
	
	// First encode the length
	jb_result_t result = jb_codec_encode_general_int(buff, remaining, len);
	if (result != JB_OK)
		return result;
	
	// Then encode the data
	if (*remaining < len)
		return JB_ERR_NO_DATA;
	
	if (len > 0) {
		memcpy(*buff, data, len);
		*buff += len;
		*remaining -= len;
	}
	
	return JB_OK;
}

jb_result_t jb_codec_encode_fixed_binary(uint8_t** buff, uint64_t* remaining, const uint8_t* data, uint64_t len) {
	if (!data && len > 0)
		return JB_ERR_NO_OUT;
	if (!buff || !remaining)
		return JB_ERR_NO_OUT;
	if (*remaining < len)
		return JB_ERR_NO_DATA;
	
	if (len > 0) {
		memcpy(*buff, data, len);
		*buff += len;
		*remaining -= len;
	}
	
	return JB_OK;
}

// === Option/Maybe encoders ===

jb_result_t jb_codec_encode_maybe_none(uint8_t** buff, uint64_t* remaining) {
	if (!buff || !remaining)
		return JB_ERR_NO_OUT;
	if (*remaining < 1)
		return JB_ERR_NO_DATA;
	
	**buff = 0;
	*buff += 1;
	*remaining -= 1;
	return JB_OK;
}

jb_result_t jb_codec_encode_maybe_some(uint8_t** buff, uint64_t* remaining) {
	if (!buff || !remaining)
		return JB_ERR_NO_OUT;
	if (*remaining < 1)
		return JB_ERR_NO_DATA;
	
	**buff = 1;
	*buff += 1;
	*remaining -= 1;
	return JB_OK;
}

// === List encoder ===

jb_result_t jb_codec_encode_list_len(uint8_t** buff, uint64_t* remaining, uint64_t len) {
	return jb_codec_encode_general_int(buff, remaining, len);
}
