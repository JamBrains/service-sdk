MicroPython service prototype
=============================

This folder contains a minimal example showing how a Python service can use the SDK
via a C extension (`sdk/py_host.c`) which exposes a `host` module.

Quick steps:

1. Build or obtain `micropython` and `mpy-cross` for your host.
2. Run `mpy-cross example.py` to produce `example.mpy`.
3. Embed `example.mpy` into your service binary or load it via the MicroPython filesystem.
4. Ensure `py_host.c` is compiled/linked into the runtime and `MODULE_HOST_ENABLED` is defined.

Notes:
- This is a prototype; gas metering and sandboxing are required before production use.
- `py_host.c` assumes `jb_host_*` functions are available to the final runtime.
