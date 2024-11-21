# Passive Sonar / Ambisonic Microphone
This repo contains a design for a working ambisonic microphone, based on AD2427W A2B transcievers, along with Infineon IM73D122V01XTMA1 MEMS microphones.

## Structure
- kicad_micboard/ : Design files for the microphone board (firmware supports up to 16 of these)
- kicad_iface/ : Design files for the MCHStreamer interface board (only one of these needed)
- firmware_iface/ : Firmware for the MCHStreamer interface board

These folders each contain a README.md, describing their contents in more detail, along with an ORDERING.md giving details on how to order each board.

## Cabling
The cables between nodes are **straight-through twisted pair 2-pin JST-PH** cables. I crimped these cables myself, but you could easily order single-ended pre-crimped cables and then twist and solder them manually

## Known issues
- The voltage given to the interface board dictates how many nodes you can connect in series. For 5 volts, it can only support 6 nodes. For 15 nodes, you need to power the interface board with between 8V and 9V. It will *probably* work on 10V, but won't work on 12V. Some cheap 9V power supplies output *way* more than 9V when unloaded, so be careful with what power supply you choose.
- The A2B transciever on the interface board is missing connections for the I2C address lines. In theory, the actual I2C address of the transciever is undefined, in practice it works. If it doesn't, change the I2C address in the firmware.

## Future work
- The microphone boards have an expansion port, allowing the system to support up to 32 microphones (in theory, this hasn't been tested). The daughter-board to connect to said port hasn't been designed yet.
- The firmware is still very early, and doesn't correctly handle every edgecase. Ideally it should do line-fault diagnostics, but this is currently not implemented.

# TODO INTERNAL
- [x] Fix kicad syms and libs
- [x] Make sure 3D-models are in the right place
- [x] Make documentation for ordering (stackup and so on)
- [ ] Take and insert pictures into docs
- [ ] Upload CAD models
- [x] Document wiring (diagram would be nice)
