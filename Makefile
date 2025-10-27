# Minimal Makefile targets for MicroPython prototype

MPY_CROSS ?= mpy-cross

.PHONY: python-mpy
python-mpy:
	@echo "Compiling MicroPython example to .mpy"
	$(MPY_CROSS) services/python_service/example.py -o services/python_service/example.mpy

# Placeholder target showing where embedding would occur
.PHONY: embed-mpy
embed-mpy: python-mpy
	@echo "Embed services/python_service/example.mpy into binary or fs image"
