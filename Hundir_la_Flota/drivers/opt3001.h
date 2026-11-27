#ifndef HAL_OPT3001_H_
#define HAL_OPT3001_H_

// I2C SLAVE ADDRESS
#define OPT3001_SLAVE_ADDRESS 0x44

// REGISTERS
#define RESULT_REG 0x00
#define CONFIG_REG 0x01
#define LOWLIMIT_REG 0x02
#define HIGHLIMIT_REG 0x03
#define MANUFACTUREID_REG 0x7E
#define DEVICEID_REG 0x7F

// CONFIG REGISTER PRESETS
#define DEFAULT_CONFIG 0xCC10 // 800ms
#define DEFAULT_CONFIG_100 0xC410 // 100ms

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
//! \brief Configures the OPT3001 ambient light sensor (I2C).
//!
//! Writes the default configuration (continuous conversion, 100 ms) into
//! the sensor CONFIG register. Call once after configuring the I2C HAL and
//! before reading.
//!
//! \return None.
//
//*****************************************************************************
void OPT3001_config(void);

//*****************************************************************************
//
//! \brief Reads the ambient illuminance in lux.
//!
//! Reads the RESULT register and converts the sensor format (mantissa +
//! exponent) into lux as described in the datasheet.
//!
//! \return Illuminance in lux.
//
//*****************************************************************************
uint32_t OPT3001_get_lux(void);

//*****************************************************************************
//
//! \brief Reads the sensor manufacturer identifier.
//!
//! Useful to verify the I2C communication (should read 0x5449, "TI").
//!
//! \return Contents of the MANUFACTUREID register.
//
//*****************************************************************************
uint16_t OPT3001_read_manufacturer_id(void);

//*****************************************************************************
//
//! \brief Reads the sensor device identifier.
//!
//! \return Contents of the DEVICEID register (0x3001).
//
//*****************************************************************************
uint16_t OPT3001_read_device_id(void);

//*****************************************************************************
//
//! \brief Reads the sensor configuration register.
//!
//! \return Contents of the CONFIG register.
//
//*****************************************************************************
uint16_t OPT3001_read_config_reg(void);

//*****************************************************************************
//
//! \brief Reads the low-limit register (window comparator).
//!
//! \return Contents of the LOWLIMIT register.
//
//*****************************************************************************
uint16_t OPT3001_read_low_limit_reg(void);

//*****************************************************************************
//
//! \brief Reads the high-limit register (window comparator).
//!
//! \return Contents of the HIGHLIMIT register.
//
//*****************************************************************************
uint16_t OPT3001_read_high_limit_reg(void);

#ifdef __cplusplus
}
#endif

#endif /* HAL_OPT3001_H_ */
