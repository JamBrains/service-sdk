#pragma once

#include <stdint.h>
#include "jb_result.h"

#define MAX_MEMO_SIZE 128
#define HASH_SIZE 32

// Note that we dont give a shit about the byte order since PVM has little endian.
// We "decode" this just by casting.
struct _jb_service_info {
	char code_hash[32];
	uint64_t balance;
	uint64_t threshold_balance;
	uint64_t min_gas_accumulate;
	uint64_t min_gas_on_transfer;
	uint64_t bytes_in_storage;
	uint32_t items_in_storage;
	uint64_t gratis_storage_offset;
	uint32_t creation_slot;
	uint32_t last_accumulation_slot;
	uint32_t parent_slot;
} __attribute__((packed));
typedef struct _jb_service_info jb_service_info_t;

#define JB_SERVICE_INFO_SIZE 96 // Needs to match with GP 0.6.7

struct _jb_chain_params {
	uint64_t item_deposit;
	uint64_t byte_deposit;
	uint64_t base_deposit;
	uint16_t core_count;
	uint32_t expunge_period;
	uint32_t epoch_length;
	uint64_t report_acc_gas;
	uint64_t package_auth_gas;
	uint64_t package_ref_gas;
	uint64_t block_acc_gas;
	uint16_t recent_history_len;
	uint16_t max_package_items;
	uint16_t max_report_deps;
	uint16_t max_block_tickets;
	uint32_t max_lookup_anchorage;
	uint16_t ticket_entries;
	uint16_t auth_pool_size;
	uint16_t slot_seconds;
	uint16_t auth_queue_size;
	uint16_t rotation_period;
	uint16_t max_package_exts;
	uint16_t assurance_timeout_period; 
	uint16_t val_count;
	uint32_t max_auth_code_size;
	uint32_t max_bundle_size;
	uint32_t max_service_code_size;
	uint32_t ec_piece_size;
	uint32_t max_package_imports;
	uint32_t segment_ec_pieces;
	uint32_t max_report_var_size;
	uint32_t memo_size;
	uint32_t max_package_exports;
	uint32_t epoch_tail_start;
} __attribute__((packed));
typedef struct _jb_chain_params jb_chain_params_t;

#define JB_CHAIN_PARAMS_SIZE 134 // Needs to match with GP 0.6.7

uint64_t jb_chain_params_fmt(jb_chain_params_t* params, char* buffer, uint64_t buffer_len);

#define JB_SERVICE_ID_SELF (uint64_t)0xffffffffffffffff

#define DIGEST_OK 0
#define DIGEST_INFINITY 1
#define DIGEST_LIGHTNING 2
#define DIGEST_INVALID_EXPORTS 3
#define DIGEST_OVERSIZE 4
#define DIGEST_BAD 5
#define DIGEST_BIG 6

typedef uint8_t jb_work_digest_output_status_t;

typedef struct {
	// One of the above constants.
	jb_work_digest_output_status_t status;
	// Only available if `status == DIGEST_OK`.
	uint64_t data_len;
	uint8_t* data;
} jb_work_digest_output_t;

jb_result_t jb_work_digest_output_decode(uint8_t** in_out_buff, uint64_t* in_out_remaining, jb_work_digest_output_t* out);

typedef struct {
	uint8_t work_package_hash[32];
	uint8_t work_package_segment_root[32];
	uint8_t work_report_authorizer_hash[32];
	uint8_t work_digest_payload_hash[32];
	uint64_t gas_limit;
	jb_work_digest_output_t digest_output;
	uint64_t work_report_output_len;
	uint8_t* work_report_output;
} jb_accumulate_operand_t;

typedef struct {
	uint32_t sender;
	uint32_t receiver;
	uint64_t amount;
	uint8_t memo[MAX_MEMO_SIZE];
	uint64_t gas_limit;
} jb_accumulate_transfer_t;

typedef struct {
	bool is_transfer;
	// Only present if `is_transfer` is false.
	jb_accumulate_operand_t operand;
	// Only present if `is_transfer` is true.
	jb_accumulate_transfer_t transfer;
} jb_accumulate_input_t;

typedef struct {
	uint64_t inputs_len;
	jb_accumulate_input_t* inputs;
} jb_accumulate_inputs_t;

jb_result_t jb_accumulate_operand_decode(uint8_t** in_buff, uint64_t* in_remaining, jb_accumulate_operand_t* out);
jb_result_t jb_accumulate_transfer_decode(uint8_t** in_buff, uint64_t* in_remaining, jb_accumulate_transfer_t* out);
jb_result_t jb_accumulate_input_decode(uint8_t** in_buff, uint64_t* in_remaining, jb_accumulate_input_t* out);
jb_result_t jb_accumulate_inputs_decode(uint8_t** in_buff, uint64_t* in_remaining, jb_accumulate_inputs_t* out);

typedef struct {
	uint32_t timeslot;
	uint32_t service_id;
	uint64_t num_inputs;
} jb_accumulate_arguments_t;

/// Decode function for `jb_accumulate_arguments_t`. Final; does not chain.
jb_result_t jb_accumulate_arguments_decode(uint8_t* in_buff, uint64_t in_remaining, jb_accumulate_arguments_t* out_value);

uint64_t jb_accumulate_arguments_fmt(jb_accumulate_arguments_t* args, char* buffer, uint64_t buffer_len);

typedef struct {
	uint32_t core_index;
	uint32_t work_item_index;
	uint32_t service_id;
	uint8_t* work_payload;
	uint64_t work_payload_len;
	uint8_t work_package_hash[32];
} jb_refine_arguments_t;

/// Decode function for `jb_refine_arguments_t`. Final; does not chain.
jb_result_t jb_refine_arguments_decode(uint8_t* in_buff, uint64_t in_remaining, jb_refine_arguments_t* out_value);

uint64_t jb_refine_arguments_fmt(jb_refine_arguments_t* args, char* buffer, uint64_t buffer_len);

typedef struct {
	uint16_t core_index;
} jb_is_authorized_arguments_t;

/// Decode function for `jb_is_authorized_arguments_t`. Final; does not chain.
jb_result_t jb_is_authorized_arguments_decode(uint8_t* in_buff, uint64_t in_remaining, jb_is_authorized_arguments_t* out_value);
