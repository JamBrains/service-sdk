// Minimal MicroPython host bindings scaffold for the service-sdk
// This file provides simple wrappers that expose a `host` module to MicroPython
// which forwards calls to the existing jb_host_* API.

#include "py/obj.h"
#include "py/runtime.h"
#include "py/binary.h"

#include "host_functions.h"

// host.write(key: bytes, value: bytes) -> int
STATIC mp_obj_t mod_host_write(mp_obj_t key_obj, mp_obj_t val_obj) {
    mp_buffer_info_t keybuf;
    mp_buffer_info_t valbuf;
    mp_get_buffer_raise(key_obj, &keybuf, MP_BUFFER_READ);
    mp_get_buffer_raise(val_obj, &valbuf, MP_BUFFER_READ);

    uint64_t res = jb_host_write((uint8_t const*)keybuf.buf, (uint64_t)keybuf.len,
                                 (uint8_t const*)valbuf.buf, (uint64_t)valbuf.len);
    return mp_obj_new_int_from_uint(res);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_host_write_obj, mod_host_write);

// host.read(service_id: int, key: bytes) -> bytes or None
STATIC mp_obj_t mod_host_read(mp_obj_t sid_obj, mp_obj_t key_obj) {
    mp_int_t sid = mp_obj_get_int(sid_obj);
    mp_buffer_info_t keybuf;
    mp_get_buffer_raise(key_obj, &keybuf, MP_BUFFER_READ);

    // First, query how much data is available by calling jb_host_read with out_len=0
    uint64_t avail = jb_host_read((uint64_t)sid, (uint64_t)keybuf.buf, (uint64_t)keybuf.len, NULL, 0, 0);
    if (avail == 0) {
        return mp_const_none;
    }

    vstr_t vstr;
    vstr_init_len(&vstr, (size_t)avail);
    uint64_t got = jb_host_read((uint64_t)sid, (uint64_t)keybuf.buf, (uint64_t)keybuf.len, (uint8_t*)vstr.buf, 0, (uint64_t)avail);
    if (got == 0) {
        return mp_const_none;
    }

    return mp_obj_new_bytes((const byte*)vstr.buf, got);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_host_read_obj, mod_host_read);

// host.log(level:int, target: bytes, msg: bytes)
STATIC mp_obj_t mod_host_log(mp_obj_t level_obj, mp_obj_t target_obj, mp_obj_t msg_obj) {
    mp_int_t level = mp_obj_get_int(level_obj);
    mp_buffer_info_t targetbuf, msgbuf;
    mp_get_buffer_raise(target_obj, &targetbuf, MP_BUFFER_READ);
    mp_get_buffer_raise(msg_obj, &msgbuf, MP_BUFFER_READ);

    jb_host_log((uint64_t)level, (uint8_t const*)targetbuf.buf, (uint64_t)targetbuf.len, (uint8_t const*)msgbuf.buf, (uint64_t)msgbuf.len);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_host_log_obj, mod_host_log);

// Module globals table
STATIC const mp_rom_map_elem_t host_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&mod_host_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&mod_host_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_log), MP_ROM_PTR(&mod_host_log_obj) },
};

STATIC MP_DEFINE_CONST_DICT(host_module_globals, host_module_globals_table);

const mp_obj_module_t host_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&host_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_host, host_user_cmodule, MODULE_HOST_ENABLED);
