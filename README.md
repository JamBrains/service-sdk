# PBA Demo

## Building the Service

The easiest way to build the service is to use our provided Docker image (Apple silicon only). See the next section in case that you cannot use it.

The project uses [`just`](https://github.com/casey/just) to run commands. Install it with:

```bash
cargo install just

# Tell the script whether you prefer docker or podman
just docker # or podman
```

You can now compile the PBA demo in the `examples` folder with the `just examples` command:
```bash
just examples
```

You should see the following output:

```pre
mkdir -p output
just compile output/02-pba-demo.elf 02-pba-demo.c
just link output/02-pba-demo.jam output/02-pba-demo.elf
```

There is also a `watch` command that will re-build when you change the example code: `just examples watch`.

This will result in a **`02-pba-demo.jam`** file in the `output` folder. This is the final service blob according to the Gray Paper.

### Non-Apple Silicon

You can build the Docker image also on Debian or other systems with `podman build -t service-builder .` (or `docker build -t service-builder .`) and then change the expected image name in the `.env` file to `service-builder`.

If you do not want to use Docker at all, you can also build the dependencies manually like described in [here](https://github.com/JamBrains/polkaports). You then still have to modify the `compile` step of the [Justfile](examples/Justfile) to use it directly.

## Run the Service

You need checkout branch `oty-pvm-pba-demo` and run the test tagged with `only` in the file `packages/gm-core/test/jam/pvm/blob_test.exs`. Concretely:

```bash
cd graymatter
git checkout oty-pvm-pba-demo
cd packages/gm-core
GM_SERVICE_LOG=5 GM_LOG=info mix test test/jam/pvm/blob_test.exs --only only
```

The output should print something like this:

```pre
...
11:13:52.805 [info] LOG(JB Demo): Items: 118, balance: 10000, storage deposit: 9858, used: 98.58%
11:13:52.809 [info] LOG(JB Demo): Items: 119, balance: 10000, storage deposit: 9940, used: 99.40%
11:13:52.812 [info] LOG(JB Demo): Items: 120, balance: 10000, storage deposit: 10022, used: 100.22%
11:13:52.814 [error] LOG(JB Demo): Writing to the storage failed: ERR_INSUFFICIENT_BALANCE
```

It demonstrates the threshold balance that a service needs to keep in order to write storage. You can play around with the code in `examples/02-pba-demo.c` to see how it works.

If you want a watch-command template for [`entr`](https://formulae.brew.sh/formula/entr) (replace the path with your own):  
`echo "/Users/vados/Documents/work/polkavm-examples/examples/output/02-pba-demo.pvm" | entr -cs 'GM_SERVICE_LOG=5 GM_LOG=info mix test test/jam/pvm/blob_test.exs --only only'`
