"""
Example Python service that uses the `host` module provided by `py_host.c`.
This demonstrates reading and writing a key and logging a message.
"""

def main():
    key = b"greeting"
    value = b"hello from python"

    # write value
    res = host.write(key, value)
    if res != 0:
        # res == 0 usually indicates success in jb_host_write convention
        host.log(1, b"example", b"write failed")

    # read back (service_id 0 for local storage example)
    data = host.read(0, key)
    if data:
        host.log(2, b"example", b"read: " + data)
    else:
        host.log(1, b"example", b"read returned none")

if __name__ == '__main__':
    main()
