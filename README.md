# TinyTemp - ATtiny85 Temperature Sensor

Temperature sensor project using ATtiny85 microcontroller with OLED display.

## Hardware
- ATtiny85 microcontroller
- SSD1306 OLED display
- MCP1700 voltage regulator
- Custom PCB design

## Files

### Code/
- `tinytemp.c` - Main temperature sensor code
- `tinyOLED.c/h` - OLED display driver
- `makefile` - Build configuration

### PCB Design
- `tinyTemp.kicad_*` - KiCAD project files
- `project2_N_Robinson_S_Chicoine.kicad_pcb` - Final PCB layout

### Custom Components
- `Symbols and Footprints/` - Custom KiCAD symbols and footprints
  - ATtiny85, SSD1306, MCP1700, capacitors, connectors

## Build
```bash
cd Code/
make
```

## Authors
Nathan Robinson & Sam Chicoine