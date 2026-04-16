# Phase 1.5: Current Sense Validation

This example demonstrates how to read motor phase currents using the integrated current-sense amplifiers of the DRV8316.

## Key Features
- **Integrated CSA**: Uses the DRV8316's internal current sense amplifiers.
- **SimpleFOC Integration**: Uses `InlineCurrentSense` to map ADC readings to Amperes.
- **Offset Calibration**: Automatically calibrates the zero-current offset at startup.
- **Serial Monitor Output**: Prints CURA, CURB, and CURC in Amps for debugging.

## Hardware Setup
- **MCU Pins**: PA0 (CURA), PA1 (CURB), PA2 (CURC).
- **Driver**: DRV8316 must be powered (VM > 8V) for the amplifiers to work.
- **Gain**: Configured via SPI (defaulting to 0.3 V/A in this test).

## How to Test
1. Flash the firmware.
2. Open the serial monitor at 115200 baud.
3. The board will first calibrate offsets (ensure motor is not moving and no current is flowing).
4. Observe the current readings. Without motor load, they should be close to 0.00A.
5. If you rotate the motor manually or apply a load in open loop, you should see the values change.
