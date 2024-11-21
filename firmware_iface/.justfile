default: build

alias b := build
alias c := clean
alias cb := cleanbuild

clean:
    rm -rf build

build:
    #!/usr/bin/env bash
    set -euxo pipefail
    mkdir -p build
    cd build
    cmake ..
    make
    cp compile_commands.json ..

flash:
    cd build && make flash

monitor:
    minicom -D /dev/ttyUSB0 -b 9600

cleanbuild: clean build
