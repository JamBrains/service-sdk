# JAM Service SDK in C23

## Building the Service

The easiest way to build the service is to use our provided Docker image (Apple silicon only). See
the next section in case that you cannot use it.

The project uses [`just`](https://github.com/casey/just) to run commands. Install it with:

```bash
cargo install just

# Tell the script whether you prefer docker or podman
just docker # or podman
```

You can now compile the example services in the `services` command:
```bash
just services build
```

You should see the following output:

```pre
Linked output/01-hello-world.jam
Linked output/02-pba-demo.jam
Linked output/03-storage-test.jam
...
Linked output/multi-field-test.jam
Linked output/refine-test.jam
Linked output/transfer-test.jam
```

This will result in a **`02-pba-demo.jam`** file in the `services/output/` folder. The service Blobs
are currently *not* conformant to the Gray Paper since Polkatool is using a custom format. You can
use the [Playground](https://playground.jamcha.in/) to download GP conformant service blobs.

### Non-Apple Silicon

You can build the Docker image also on Debian or other systems with `podman build -t service-builder.`
(or `docker build -t service-builder .`) and then change the expected image name in
`services/Justfile` to `service-builder`.

If you do not want to use Docker at all, you can also build the dependencies manually like described
in [here](https://github.com/JamBrains/polkaports). You then still have to modify the
[Justfile](services/Justfile) to use it directly.
