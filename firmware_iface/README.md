# Interface firmware
The following is a step-by-step guide on how to install and use the firmware.

## Building
To build the firmware from souce, first install `avr-gcc`, `avrdude`, `cmake`, and `make`
Then, run the following commands in this folder:
```sh
mkdir -p build
cd build
cmake ..
make
```

## Setting the fuses
The AVR microcontroller on the interface board needs the correct fuses programmed to work. This is handled automatically when flashing the board using `make flash`

## Flashing
To flash the firmware, first connect your AVRISP-MKII to the interface board. Make sure you are supplying 9V power to the interface board. Then, run `make flash` inside the build directory

## NixOS
If you are using Nix / running NixOS, you can use the included flake to install all necessary dependencies. Simply run `nix develop .#` in this directory, and all necessary tools will be installed and made available in your current shell.
