# Build stage - contains all build dependencies
FROM debian:13-slim AS builder

RUN apt-get update && apt-get install -y wget lsb-release gnupg build-essential curl git rsync librust-lazy-static-dev && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /opt
RUN wget https://apt.llvm.org/llvm.sh && chmod u+x llvm.sh && ./llvm.sh 22

# apt.llvm.org installs only versioned binaries (clang-22); polkaports' setup.sh and the
# polkavm-cc/-c++ wrappers invoke the unversioned names (clang, clang++, lld, llvm-ar, ...).
RUN for f in /usr/bin/*-22; do ln -sf "$f" "${f%-22}"; done

# Install Rust
RUN curl https://sh.rustup.rs -sSf | bash -s -- -y
ENV PATH="/root/.cargo/bin:${PATH}"

ENV AR=llvm-ar-22
ENV AS=llvm-as-22
ENV CC=clang-22
ENV CXX=clang++-22
ENV LLD=lld-22
ENV NM=llvm-nm-22
ENV OBJCOPY=llvm-objcopy-22
ENV OBJDUMP=llvm-objdump-22
ENV RANLIB=llvm-ranlib-22
ENV STRIP=llvm-strip-22

ADD thirdparty/polkaports /opt/thirdparty/polkaports
WORKDIR /opt/thirdparty/polkaports

RUN ./setup.sh

# Set up environment for integrity checks
ENV COREVM_SYSROOT="/opt/thirdparty/polkaports/sysroot"
ENV PATH="/opt/thirdparty/polkaports/sysroot/bin:/opt/bin:${PATH}"

RUN cargo install --git https://github.com/JamBrains/polkavm-to-jam --branch master --quiet --locked --root /opt/

#################################
# Stage 2
#################################
FROM debian:13-slim AS runtime

RUN apt-get update && apt-get install -y wget lsb-release gnupg build-essential jq && apt-get clean

# Install lld-22 to match clang-22
RUN wget https://apt.llvm.org/llvm.sh && chmod u+x llvm.sh && ./llvm.sh 22

# Symlink the unversioned tool names that the polkavm-cc/-c++ wrappers exec at runtime.
RUN for f in /usr/bin/*-22; do ln -sf "$f" "${f%-22}"; done

COPY --from=builder /opt /opt

# Set environment variables for runtime
ENV AR=llvm-ar-22
ENV AS=llvm-as-22
ENV CC=clang-22
ENV CXX=clang++-22
ENV LLD=lld-22
ENV NM=llvm-nm-22
ENV OBJCOPY=llvm-objcopy-22
ENV OBJDUMP=llvm-objdump-22
ENV RANLIB=llvm-ranlib-22
ENV STRIP=llvm-strip-22

ENV COREVM_SYSROOT="/opt/thirdparty/polkaports/sysroot"
ENV PATH="/opt/thirdparty/polkaports/sysroot/bin:/opt/bin:${PATH}"

# Integrity checks in builder
RUN polkavm-cc --help && polkavm-c++ --help && polkatool --help

WORKDIR /app

# Copy SDK files and hello-world example for single-file compilation mode
# These are placed at the end to avoid invalidating Docker cache
COPY sdk /opt/sdk

# Copy examples directory for generate-examples command
COPY services /opt/examples

# Install the single-file compilation script
COPY single-file /usr/local/bin/single-file
RUN chmod +x /usr/local/bin/single-file

COPY generate-examples /usr/local/bin/generate-examples
RUN chmod +x /usr/local/bin/generate-examples

# Set this as default command, but allow people to also use CC instead.
CMD ["clang++-22"]
