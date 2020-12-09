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

;
#if defined(STM32F10X) || defined(STM32F2XX) || defined(STM32F4XX) || defined(STM32L1XX)

#include <assert.h>
#include <stdint.h>
#include <drv_i2cm.h>
#include <time.h>
#include <timing.h>
#include <devices.h>
#include <drv_stm32_i2cm_internal.h>

#define I2CM_TIMEOUT (CLOCKS_PER_SEC / 10 + 1)

static inline void wait( void )
{
}


/**
 * This function generates a stop condition on the bus and than returns with said returnval.
 * It should be used to return from error situations so that the bus is brought into a
 * defined state for the next operation.
 *
 * In any place where you'ld want to:
 *   if ( error ) return I2C_ERRORNO;
 * you should:
 *   if ( error ) return i2cm_abort( drv, I2C_ERRNO );
 *
 * param drv pointer to driver context
 * param retval Return value to be communicated ack
 */

static inline int i2cm_abort( i2cm_t * drv, int retval )
{
    drv->i2cx->SR1 = 0x00;
    drv->i2cx->SR2 = 0x00;
    I2C_GenerateSTOP( drv->i2cx, ENABLE );
    I2C_SoftwareResetCmd( drv->i2cx, ENABLE );
    I2C_GenerateSTOP( drv->i2cx, DISABLE );
    I2C_SoftwareResetCmd( drv->i2cx, DISABLE );

    /* re-initialize the peripheral */
    I2C_Init( drv->i2cx, &drv->init );

    return retval;
}



/**
 * @brief Put a character on the I2C bus
 *
 * @param drv Pointer to driver context as returned from i2cm_open()
 * @param start First generate a start condition if true
 * @param data Data to be send
 *
 * @return 0 if okay
 */

int i2cm_putchar( i2cm_t * restrict drv, bool start, const uint8_t data )
{
    if ( start )
    {
        I2C_GenerateSTART( drv->i2cx, ENABLE );
        while(!I2C_CheckEvent(drv->i2cx, I2C_EVENT_MASTER_MODE_SELECT)) wait();
    }
    I2C_GenerateSTART( drv->i2cx, DISABLE );
    I2C_SendData( drv->i2cx, data );
    while( I2C_GetFlagStatus( drv->i2cx, I2C_FLAG_BUSY ) == SET ) wait();
    return 0;
}

/**
 * @brief Get a character from the I2C bus
 *
 * @param drv Pointer to driver context as returned from i2cm_open()
 * @param start First generate a start condition if true
 * @param ack Generate an ack if set, negative ack if not set
 *
 * @return -1 on error, character received from bus otherwise
 */

int i2cm_getchar( i2cm_t * restrict drv, bool start, bool ack )
{
    clock_t timeout;
    if ( start )
    {
        I2C_GenerateSTART( drv->i2cx, ENABLE );
        while(!I2C_CheckEvent(drv->i2cx, I2C_EVENT_MASTER_MODE_SELECT)) wait();
//        while( I2C_GetFlagStatus( drv->i2cx, I2C_FLAG_SB ) == RESET ) wait();
    }
    I2C_AcknowledgeConfig( drv->i2cx, ack ? ENABLE : DISABLE );

    timeout = clock() + I2CM_TIMEOUT;

    // Start receiver
    while (!I2C_CheckEvent( drv->i2cx, I2C_EVENT_MASTER_BYTE_RECEIVED))  /*!< EV7 */
    {
        if ( timeout < clock() ) return i2cm_abort( drv, -1 );
        wait();
    }

    return I2C_ReceiveData( drv->i2cx );
}


/**
 * @brief Generate a STOP condition on the bus
 *
 * @param drv Pointer to driver context as returned from i2cm_open()
 *
 * @return Nothing
 */

int i2cm_stop( i2cm_t * restrict drv )
{
    I2C_GenerateSTOP( drv->i2cx, ENABLE );
    return 0;
}

/**
 * @brief Get I2C bus driver control
 *
 * Note: this function is a stub for backward compatibility.
 *
 * @param drv Pointer to driver context as returned from i2cm_open()
 *
 * @return 0 on success, failure otherwise.
 */

int i2cm_get_bus( i2cm_t * restrict drv )
{
    return 0;
}

/**
 * @brief Release I2C bus driver control
 *
 * Note: this function is a stub for backward compatibility.
 *
 * @param drv Pointer to driver context as returned from i2cm_open()
 *
 * @return Nothing
 */

void i2cm_release_bus( i2cm_t * restrict drv )
{
}
/**
 * @brief Read registers from a I2C slave
 *
 * Most I2C slave peripherals require a write pointer to be sent before a read can be done.
 * This function provides functionality to select the slave, send the register address than switch
 * to I2C read and perform the actual read.
 *
 * @param drv Pointer to driver context as returned from i2cm_open()
 * @param slave Address of slave
 * @param reg Register address
 * @param addrsize  Size of register address
 * @param data Array where received data is to be stored
 * @param length Number of bytes to be received
 *
 * @return < 0: error code, number of bytes successfully received otherwise
 */

int i2cm_read_reg( i2cm_t * restrict drv, uint8_t slave, uint32_t reg, size_t addrsize, void * data, size_t length )
{
    uint8_t * buf = data;
    clock_t timeout;
    I2C_TypeDef * i2c = drv->i2cx;

    timeout = clock() + I2CM_TIMEOUT;
    while( I2C_GetFlagStatus( i2c, I2C_FLAG_BUSY ) == SET )
    {
        if ( clock() > timeout ) return i2cm_abort( drv, -1 );
    }

    if ( addrsize )
    {
        I2C_GenerateSTART( i2c, ENABLE );
        timeout = clock() + I2CM_TIMEOUT;
        while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_MODE_SELECT))
        {
            if ( clock() > timeout ) return i2cm_abort( drv, -1 );
        }

        // Send slave address for write
        I2C_Send7bitAddress( i2c, slave, I2C_Direction_Transmitter );
        timeout = clock() + I2CM_TIMEOUT;
        while (!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) )
        {
            if ( clock() > timeout ) return i2cm_abort( drv, -2 );
        }

        switch( addrsize )
        {
        case 4 :
            I2C_SendData( i2c, (reg >> 24 ) & 0xFF );
            timeout = clock() + I2CM_TIMEOUT;
            while (!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) )
            {
                if ( clock() > timeout ) return i2cm_abort( drv, -3 );
            }
            // Fallthrough
        case 3 :
            I2C_SendData( i2c, (reg >> 16 ) & 0xFF );
            timeout = clock() + I2CM_TIMEOUT;
            while (!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) )
            {
                if ( clock() > timeout ) return i2cm_abort( drv, -3 );
            }
            // Fallthrough
        case 2 :
            I2C_SendData( i2c, (reg >> 8 ) & 0xFF );
            timeout = clock() + I2CM_TIMEOUT;
            while (!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) )
            {
                if ( clock() > timeout ) return i2cm_abort( drv, -3 );
            }
            // Fallthrough
        default :
            I2C_SendData( i2c, (reg >> 0 ) & 0xFF );
            timeout = clock() + I2CM_TIMEOUT;
            while ( I2C_GetFlagStatus( i2c, I2C_FLAG_BTF) == RESET )
            {
                if ( clock() > timeout ) return i2cm_abort( drv, -3 );
            }
        }
    }

    // Must resend slave address with a restart condition
    I2C_GenerateSTART( i2c, ENABLE );
    timeout = clock() + I2CM_TIMEOUT;
    while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if ( clock() > timeout ) return i2cm_abort( drv, -4 );
    }

    I2C_Send7bitAddress( i2c, slave, I2C_Direction_Receiver );
    timeout = clock() + I2CM_TIMEOUT;
    while ( I2C_GetFlagStatus( i2c, I2C_FLAG_ADDR) == RESET )
    {
        if ( clock() > timeout ) return i2cm_abort( drv, -5 );
    }

    // Clear ADDR flag by reading the status register
    i2c->SR1;
    i2c->SR2;

    for ( int i = length; i; i-- )
    {
         if ( i == 1 )
         {
            I2C_AcknowledgeConfig( i2c, DISABLE );
            I2C_GenerateSTOP( i2c, ENABLE );
        }
        else
        {
            I2C_AcknowledgeConfig( i2c, ENABLE );
            I2C_GenerateSTOP( i2c, DISABLE );
        }
        timeout = clock() + I2CM_TIMEOUT;
//        while (!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_RECEIVED))
        while( I2C_GetFlagStatus( i2c, I2C_FLAG_RXNE ) == RESET )
        {
            if ( clock() > timeout ) return i2cm_abort( drv, -6 );
        }

        *buf = I2C_ReceiveData( i2c );
        buf++;
    }

    // Wait to make sure that STOP control bit has been cleared
    timeout = clock() + I2CM_TIMEOUT;
    while( i2c->CR1 & I2C_CR1_STOP)
    {
        if ( clock() > timeout ) return i2cm_abort( drv, -7 );
    }

    // Re-Enable Acknowledgement to be ready for another reception
    I2C_AcknowledgeConfig( i2c, ENABLE);
    return length;
}

/**
 * @brief Write registers to a I2C slave
 *
 * Most I2C slave peripherals require a write pointer to be sent before a write can be done.
 * This function provides functionality to select the slave, send the register address and than
 * perform the actual write.
 *
 * @param drv Pointer to driver context as returned from i2cm_open()
 * @param slave Address of slave
 * @param reg Register address
 * @param addrsize  Size of register address
 * @param data Data to be transmitted
 * @param length Number of bytes to be send
 *
 * @return < 0: error code, number of bytes successfully send otherwise
 */

int i2cm_write_reg( i2cm_t * restrict drv, uint8_t slave, uint32_t reg, size_t addrsize, void * data, size_t length )
{
    uint8_t * buf = data;
    clock_t timeout;
    I2C_TypeDef * i2c = drv->i2cx;

    // Wait until bus is ready
    timeout = clock() + I2CM_TIMEOUT;
    while(I2C_GetFlagStatus( i2c, I2C_FLAG_BUSY ))
    {
        if ( clock() > timeout ) return i2cm_abort( drv, -1 );
    }

    // Generate start condition
    I2C_GenerateSTART( i2c, ENABLE );
    timeout = clock() + I2CM_TIMEOUT;
    while(!I2C_CheckEvent( i2c, I2C_EVENT_MASTER_MODE_SELECT) )
    {
        if ( clock() > timeout ) return i2cm_abort( drv, -1 );
    }

    // Send slave address for write
    I2C_Send7bitAddress( i2c, slave, I2C_Direction_Transmitter );

    timeout = clock() + I2CM_TIMEOUT;
    while (!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) )
    {
        if ( clock() > timeout ) return i2cm_abort( drv, -2 );
    }

    switch ( addrsize )
    {
    case 0 :    // Do not write a register address
        break;
    case 4 :
        I2C_SendData( i2c, (reg >> 24) & 0xFF );
        timeout = clock() + I2CM_TIMEOUT;
        while (!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) )
        {
            if ( clock() > timeout ) return i2cm_abort( drv, -3 );
        }
        // Fallthrough
    case 3 :
        I2C_SendData( i2c, (reg >> 16) & 0xFF );
        timeout = clock() + I2CM_TIMEOUT;
        while (!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) )
        {
            if ( clock() > timeout ) return i2cm_abort( drv, -4 );
        }
        // Fallthrough
    case 2 :
        I2C_SendData( i2c, (reg >> 8) & 0xFF );
        timeout = clock() + I2CM_TIMEOUT;
        while (!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) )
        {
            if ( clock() > timeout ) return i2cm_abort( drv, -5 );
        }
        // Fallthrough
    default :
        I2C_SendData( i2c, (reg >> 0) & 0xFF );
        timeout = clock() + I2CM_TIMEOUT;
        while (!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) )
        {
            if ( clock() > timeout ) return i2cm_abort( drv, -6 );
        }
    }

    timeout = clock() + I2CM_TIMEOUT;
    while( I2C_GetFlagStatus( i2c, I2C_FLAG_TXE ) == RESET )
    {
        if ( clock() > timeout ) return i2cm_abort( drv, -7 );
    }

    for ( int i = length; i; i-- )
    {
        I2C_SendData( i2c, *buf );
        buf++;
        timeout = clock() + I2CM_TIMEOUT;
        while( I2C_GetFlagStatus( i2c, I2C_FLAG_TXE ) == RESET )
        {
            if ( clock() > timeout )
            {
                return i2cm_abort( drv, -8 );
            }
        }
    }

    I2C_GenerateSTOP( i2c, ENABLE );
    // Wait to make sure that STOP control bit has been cleared
    timeout = clock() + I2CM_TIMEOUT;
    while( i2c->CR1 & I2C_CR1_STOP)
    {
        if ( clock() > timeout ) return i2cm_abort( drv, -9 );
    }

    return length;
}
#endif

