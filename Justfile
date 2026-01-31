set quiet
set dotenv-load

mod services
mod authorizers

default:
    just --list

env:
    #!/usr/bin/env sh
    if [ ! -f .env ]; then
        echo "Please run either 'just docker' or 'just podman' first in the root folder"
        exit 1
    fi

# Set docker as the preferred container runtime
docker:
    echo "DOCKER=docker" > .env

# Set podman as the preferred container runtime
podman:
    echo "DOCKER=podman" > .env

build-image:
    podman build -m=24000m -t service-builder .

build: env
    just services build
    just authorizers build
