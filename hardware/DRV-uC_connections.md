# Driver (DRV8316CRRGFR) - Microcontroller (STM32G474CEU6) connections

| Driver pin | Signal name | Microcontroller pin | Alternate Function (TIM) |
|---------|-------------|-------------|-------------|
| 22 nFAULT | DRV_FAULT | 2 PC13 | TIM8_CH4N |
| 27 INHA | AH | 32 PA10 | TIM1_CH3 |
| 28 INLA | AL | 28 PB15 | TIM1_CH3N |
| 29 INHB | BH | 31 PA9 | TIM1_CH2 |
| 30 INLB | BL | 27 PB14 | TIM1_CH2N |
| 31 INHC | CH | 30 PA8 | TIM1_CH1 |
| 32 INLC | CL | 26 PB13 | TIM1_CH1N |
| 33 SDO | DRV_MISO | 42 PB4 | SPI1_MISO / SPI3_MISO |
| 34 SDI | DRV_MOSI | 43 PB5 | SPI1_MOSI / SPI3_MOSI |
| 35 SCLK | DRV_CLK | 41 PB3 | SPI1_SCK / SPI3_SCK |
| 36 nSCS | DRV_CS | 16 PC4 | GPIO |
| 38 SOC | CURC | 10 PA2 | ADC |
| 39 SOB | CURB | 9 PA1 | ADC |
| 40 SOA | CURA | 8 PA0 | ADC |

## Additional Device Connections
### MT6701 Encoder (SPI)
| Encoder Pin | Signal | Microcontroller pin |
|---------|-------------|-------------|
| CS | ENC_CS | 12 PA4 |
| CLK | ENC_CLK | 13 PA5 |
| DO | ENC_SDO | 14 PA6 |

### RS485 Interface
| RS485 Pin | Signal | Microcontroller pin |
|---------|-------------|-------------|
| DI | RS485_TX | 39 PC10 |
| RO | RS485_RX | 40 PC11 |
| DE/RE | RS485_DIR | 47 PB9 |