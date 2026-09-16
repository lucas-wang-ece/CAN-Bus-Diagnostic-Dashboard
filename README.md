# CAN Bus Diagnostic Dashboard

Arduino-based automotive telemetry project using two Arduino Uno boards and MCP2515 CAN modules.

## Features

- Simulated vehicle speed, RPM, coolant temperature, and battery voltage
- CAN communication between sender and receiver
- SPI communication with MCP2515 and microSD card
- CSV telemetry logging to microSD
- Checksum generation
- Warning LED
- 16x2 LCD display

## Hardware

- 2x Arduino Uno
- 2x MCP2515 CAN modules
- microSD card module
- 16x2 LCD
- LED
- Breadboard and jumper wires

## Communication

- CAN: sender to receiver communication
- SPI: Arduino to MCP2515 and microSD card
- UART: Serial Monitor debugging

## Development

- V1 - Simulated vehicle telemetry
- V2 - CSV telemetry output
- V3 - microSD CSV logging
- V4 - Checksum
- V6 - Warning LED
- V7 - LCD dashboard
- V8 - CAN loopback testing
- V9 - CAN sender and receiver
