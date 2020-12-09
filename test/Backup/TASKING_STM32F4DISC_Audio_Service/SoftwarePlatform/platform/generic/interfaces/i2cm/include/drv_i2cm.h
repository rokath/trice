/* ------------------------------------------------------------
**
**  Copyright (c) 2013-2015 Altium Limited
**
**  This software is the proprietary, copyrighted property of
**  Altium Ltd. All Right Reserved.
**
**  SVN revision information:
**  $Rev: 14907 $:
**  $Date: 2015-01-19 13:30:51 +0100 (Mon, 19 Jan 2015) $:
**
** ------------------------------------------------------------
*/

/**
 * @file
 * Device driver for I2C master.
 */

#ifndef _DRV_I2CM_H
#define _DRV_I2CM_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define I2CM_RD(A)  ((A) | 0x01)
#define I2CM_WR(A)  ((A) & ~0x01)

/**
 * @brief    I2CM context
 *
 * A pointer to the I2CM context is returned by the i2cm_open() function and must be passed to other functions.
 * The content of the context is not available to the application.
 */

typedef struct drv_i2cm_s   i2cm_t;

extern i2cm_t * i2cm_open( unsigned int id );

// Deprecated functions: i2cm_putchar, i2cm_getchar, i2cm_stop. Do not use for new designs!
extern int i2cm_putchar( i2cm_t * restrict drv, bool start, const uint8_t data );
extern int i2cm_getchar( i2cm_t * restrict drv, bool start, bool ack );
extern int i2cm_stop( i2cm_t * restrict drv );

extern int i2cm_read_reg( i2cm_t * restrict drv, uint8_t slave, uint32_t reg, size_t addrsize, void * data, size_t length );
extern int i2cm_write_reg( i2cm_t * restrict drv, uint8_t slave, uint32_t reg, size_t addrsize, void * data, size_t length );

extern int i2cm_get_bus( i2cm_t * restrict drv );
extern void i2cm_release_bus( i2cm_t * restrict drv );

/**
 * @brief Send data stream only to an I2C slave device
 *
 * This function writes a single block of data to the I2C device.
 *
 * @param drv Pointer to device driver context as returned by i2cm_open()
 * @param slave Slave address to write to
 * @param data Pointer to storage containing data to write
 * @param length Number of bytes to write
 *
 * @returns Error condition if < 0, size of data actually written otherwise
 */

static inline int i2cm_write( i2cm_t * restrict drv, uint8_t slave, uint8_t * data, size_t length )
{
    return i2cm_write_reg( drv, slave, 0, 0, data, length );
}

/**
 * @brief Read data stream only from an I2C slave device
 *
 * This function reads a single block of data from the I2C device.
 *
 * @param drv Pointer to device driver context as returned by i2cm_open()
 * @param slave Slave address to read from
 * @param data Pointer to storage where read data should be written
 * @param length Number of bytes to read
 *
 * @returns Error condition if < 0, size of data actually read otherwise
 */

static inline int i2cm_read( i2cm_t * restrict drv, uint8_t slave, uint8_t * data, size_t length )
{
    return i2cm_read_reg( drv, slave, 0, 0, data, length );
}

/**
 * @brief Read an 8-bit register from the I2C device
 *
 * This function reads an 8-bit register from the specified I2C device. It assumes the register
 * will be addressed using a single byte only.
 *
 * @param drv Pointer to I2C device driver context as previously returned by i2cm_open()
 * @param slave Slave slave from I2C device to read from
 * @param reg Sub-slave in I2C device to read from
 * @param data Pointer to byte where read data should be stored
 *
 * @return 1 if successful, error otherwise
 */

static inline int i2cm_read_reg8( i2cm_t * restrict drv, uint8_t slave, uint8_t reg, uint8_t * data )
{
    return i2cm_read_reg( drv, slave, reg, 1, data, 1 );
}

/**
 * @brief Read a 16-bit register from the I2C device
 *
 * This function reads a 16-bit register from the specified I2C device. It assumes the register
 * will be addressed using a single byte only.
 *
 * Note: it is assumed the I2C slave stores data in big endian mode
 *
 * @param drv Pointer to I2C device driver context as previously returned by i2cm_open()
 * @param slave Slave slave from I2C device to read from
 * @param reg Sub-slave in I2C device to read from
 * @param data Pointer to half word where read data should be stored
 *
 * @return 2 if successful, error otherwise
 */

static inline int i2cm_read_reg16( i2cm_t * restrict drv, uint8_t slave, uint8_t reg, uint16_t * data )
{
    int retval;
#ifdef __BIG_ENDIAN__
    retval = i2cm_read_reg( drv, slave, reg, 1, data, 2 );
#else
    uint16_t buf;
    retval = i2cm_read_reg( drv, slave, reg, 1, &buf, 2 );
    *data = (buf << 8) | (buf >> 8);
#endif
    return retval;
}

/**
 * @brief Read a 32-bit register from the I2C device
 *
 * This function reads a 32-bit register from the specified I2C device. It assumes the register
 * will be addressed using a single byte only.
 *
 * Note: it is assumed the I2C slave stores data in big endian mode
 *
 * @param drv Pointer to I2C device driver context as previously returned by i2cm_open()
 * @param slave Slave slave from I2C device to read from
 * @param reg Sub-slave in I2C device to read from
 * @param data Pointer to word where read data should be stored
 *
 * @return 4 if successful, error otherwise
 */

static inline int i2cm_read_reg32( i2cm_t * restrict drv, uint8_t slave, uint8_t reg, uint32_t * data )
{
    int retval;
#ifdef __BIG_ENDIAN__
    retval = i2cm_read_reg( drv, slave, reg, 1, data, 4 );
#else
    uint32_t buf;
    retval = i2cm_read_reg( drv, slave, reg, 1, &buf, 4 );
    *data = (buf << 24) | ((buf & 0x0000FF00) << 8) | ((buf & 0x00FF0000) >> 8) | (buf >> 24);
#endif
    return retval;
}

/**
 * @brief Write an 8-bit register to the I2C device
 *
 * This function writes an 8-bit register into the specified I2C device. It assumes the register
 * will be addressed using a single byte only.
 *
 * @param drv Pointer to I2C device driver context as previously returned by i2cm_open()
 * @param slave Slave slave from I2C device to write to
 * @param reg Sub-slave in I2C device to write to
 * @param data Data to be written
 *
 * @return 1 if successful, error otherwise
 */

static inline int i2cm_write_reg8( i2cm_t * restrict drv, uint8_t slave, uint8_t reg, uint8_t data )
{
    return i2cm_write_reg( drv, slave, reg, 1, &data, 1 );
}

/**
 * @brief Write an 16-bit register to the I2C device
 *
 * This function writes a 16-bit register into the specified I2C device. It assumes the register
 * will be addressed using a single byte only.
 *
 * Note: it is assumed the I2C slave stores data in big endian mode
 *
 * @param drv Pointer to I2C device driver context as previously returned by i2cm_open()
 * @param slave Slave slave from I2C device to write to
 * @param reg Sub-slave in I2C device to write to
 * @param data Data to be written
 *
 * @return 2 if successful, error otherwise
 */

static inline int i2cm_write_reg16( i2cm_t * restrict drv, uint8_t slave, uint8_t reg, uint16_t data )
{
#ifndef __BIG_ENDIAN__
    data = (data << 8) | (data >> 8);
#endif
    return i2cm_write_reg( drv, slave, reg, 1, &data, 2 );
}

/**
 * @brief Write an 32-bit register to the I2C device
 *
 * This function writes a 32-bit register into the specified I2C device. It assumes the register
 * will be addressed using a single byte only.
 *
 * Note: it is assumed the I2C slave stores data in big endian mode
 *
 * @param drv Pointer to I2C device driver context as previously returned by i2cm_open()
 * @param slave Slave slave from I2C device to write to
 * @param reg Sub-slave in I2C device to write to
 * @param data Data to be written
 *
 * @return 4 if successful, error otherwise
 */

static inline int i2cm_write_reg32( i2cm_t * restrict drv, uint8_t slave, uint8_t reg, uint32_t data )
{
#ifndef __BIG_ENDIAN__
    data = (data << 24) | ((data & 0x0000FF00) << 16) | ((data & 0x00FF0000) >> 8) | (data >> 24);
#endif
    return i2cm_write_reg( drv, slave, reg, 1, &data, 4 );
}

#ifdef  __cplusplus
}
#endif

#endif /* _DRV_I2CM_H */
