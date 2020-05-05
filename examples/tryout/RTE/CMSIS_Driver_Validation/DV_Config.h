/*-----------------------------------------------------------------------------
 *      Name:         DV_Config.h
 *      Purpose:      DV Config header
 *      Version:      1.2.2
 *----------------------------------------------------------------------------
 *      Copyright(c) KEIL - An ARM Company
 *----------------------------------------------------------------------------*/
#ifndef __DV_CONFIG_H
#define __DV_CONFIG_H

#include <stdint.h>
#include "RTE_Components.h"

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h> Common Test Settings
// <o> Print Output Format <0=> Plain Text <1=> XML
// <i> Set the test results output format to plain text or XML
#ifndef PRINT_XML_REPORT
#define PRINT_XML_REPORT                0
#endif
// <h> Buffer sizes
// <i> Enable/disable buffer sizes (bytes)
// <q> 1 - 32
#define BUFFER_ELEM_1_32                0
// <q> 512
#define BUFFER_ELEM_512                 1
// <q> 1024
#define BUFFER_ELEM_1024                0
// <q> 4096
#define BUFFER_ELEM_4096                0
// <q> 32768
#define BUFFER_ELEM_32768               0
// </h>
// <o> Buffer size for baudrate test
// <i> Set the buffer sizes (bytes)
#define BUFFER_SIZE_BR                  512
// <o> Buffer pattern <0=> 0x55, 0xAA <1=> 0x12, 0x34, 0x56, 0x78
// <i> Set the buffer pattern
#define BUFFER_PATTERN_INDEX            0
#if    (BUFFER_PATTERN_INDEX == 0)
#define BUFFER_PATTERN {0x55, 0xAA}
#endif
// </h>

// <h> SPI
// <o> Driver_SPI# <0-255>
// <i> Choose the Driver_SPI# instance to be tested
#define DRV_SPI                         0
// <h> SPI bus speed
// <i> Set the SPI bus speed (kHz)
// <i> Value zero is ignored
// <o> SPI bus speed 1
#define SPI_BR_1                        400
// <o> SPI bus speed 2
#define SPI_BR_2                        1000
// <o> SPI bus speed 3
#define SPI_BR_3                        4000
// <o> SPI bus speed 4
#define SPI_BR_4                        10000
// <o> SPI bus speed 5
#define SPI_BR_5                        25000
// <o> SPI bus speed 6
#define SPI_BR_6                        50000
// </h>
// <o> Percentual trigger for bus speed test
// <i> Set the minimum margin between measured and expected bus speeds (%)
#define MIN_BUS_SPEED                   5
// <o> SPI data bits
// <i> Set the SPI data bits
#define SPI_DATA_BITS                   8
// <o> Transfer timeout
// <i> Set the transfer timeout (us)
#define SPI_TRANSFER_TIMEOUT            1000000
// <h> Test Cases
// <i> Enable/disable test cases
// <q> SPI_GetCapabilities
#define SPI_GETCAPABILITIES_EN          1
// <q> SPI_Initialization
#define SPI_INITIALIZATION_EN           1
// <q> SPI_PowerControl
#define SPI_POWERCONTROL_EN             1
// <q> SPI_Config_PolarityPhase
#define SPI_CONFIG_POLARITYPHASE_EN     1
// <q> SPI_Config_DataBits
#define SPI_CONFIG_DATABITS_EN          1
// <q> SPI_Config_BitOrder
#define SPI_CONFIG_BITORDER_EN          1
// <q> SPI_Config_SSMode
#define SPI_CONFIG_SSMODE_EN            1
// <q> SPI_Config_BusSpeed
#define SPI_CONFIG_BUSSPEED_EN          1
// <q> SPI_Config_CommonParams
#define SPI_CONFIG_COMMONPARAMS_EN      1
// <q> SPI_Send
#define SPI_SEND_EN                     1
// <q> SPI_Receive
#define SPI_RECEIVE_EN                  1
// <q> SPI_Loopback_CheckBusSpeed
#define SPI_LOOPBACK_CHECKBUSSPEED_EN   1
// <q> SPI_Loopback_Transfer
#define SPI_LOOPBACK_TRANSFER_EN        1
// <q> SPI_CheckInvalidInit
#define SPI_CHECKINVALIDINIT_EN         1
// </h>
// </h>

// <h> USART
// <o> Driver_USART# <0-255>
// <i> Choose the Driver_USART# instance to be tested
#define DRV_USART                       0
// <h> USART baudrates
// <i> Set the USART baudrates (bps)
// <i> Value zero is ignored
// <o> USART baudrate 1
#define USART_BR_1                      9600
// <o> USART baudrate 2
#define USART_BR_2                      19200
// <o> USART baudrate 3
#define USART_BR_3                      38400
// <o> USART baudrate 4
#define USART_BR_4                      57600
// <o> USART baudrate 5
#define USART_BR_5                      115200
// <o> USART baudrate 6
#define USART_BR_6                      921600
// </h>
// <o> Percentual tolerance for baudrate test
// <i> Set the tolerance between measured and expected baudrates (%)
#define TOLERANCE_BR                    5
// <o> USART data bits
// <i> Set the USART data bits
#define USART_DATA_BITS                 8
// <o> Transfer timeout
// <i> Set the transfer timeout (us)
#define USART_TRANSFER_TIMEOUT          1000000
// <h> Test Cases
// <i> Enable/disable test cases
// <q> USART_GetCapabilities
#define USART_GETCAPABILITIES_EN        1
// <q> USART_Initialization
#define USART_INITIALIZATION_EN         1
// <q> USART_PowerControl
#define USART_POWERCONTROL_EN           1
// <q> USART_Config_PolarityPhase
#define USART_CONFIG_POLARITYPHASE_EN   1
// <q> USART_Config_DataBits
#define USART_CONFIG_DATABITS_EN        1
// <q> USART_Config_StopBits
#define USART_CONFIG_STOPBITS_EN        1
// <q> USART_Config_Parity
#define USART_CONFIG_PARITY_EN          1
// <q> USART_Config_Baudrate
#define USART_CONFIG_BAUDRATE_EN        1
// <q> USART_Config_CommonParams
#define USART_CONFIG_COMMONPARAMS_EN    1
// <q> USART_Send
#define USART_SEND_EN                   1
// <q> USART_Loopback_CheckBaudrate
#define USART_LOOPBACK_CHECKBAUDRATE_EN 1
// <q> USART_Loopback_Transfer
#define USART_LOOPBACK_TRANSFER_EN      1
// <q> USART_CheckInvalidInit
#define USART_CHECKINVALIDINIT_EN       1
// </h>
// </h>

// <h> Ethernet
// <o> Driver_ETH_MAC# <0-255>
// <i> Choose the Driver_ETH_MAC# instance to be tested
#define DRV_ETH                         0
// <o> Link timeout
// <i> Set the Ethernet link timeout (us)
#define ETH_LINK_TIMEOUT                3000000
// <o> Transfer timeout
// <i> Set the transfer timeout (us)
#define ETH_TRANSFER_TIMEOUT 1000000
// <o> Time duration for PTP Control Time
// <i> Set time duration for Control Time tests (ms)
#define ETH_PTP_TIME_REF                1000
// <o> Tolerance for PTP Control Time
// <i> Set tolerance for Control Time tests (ns)
#define ETH_PTP_TOLERANCE               0
// <h> Test Cases
// <i> Enable/disable test cases
// <q> ETH_MAC_GetCapabilities
#define ETH_MAC_GETCAPABILITIES_EN      1
// <q> ETH_MAC_Initialization
#define ETH_MAC_INITIALIZATION_EN       1
// <q> ETH_MAC_PowerControl
#define ETH_MAC_POWERCONTROL_EN         1
// <q> ETH_MAC_SetBusSpeed
#define ETH_MAC_SETBUSSPEED_EN          1
// <q> ETH_MAC_Config_Mode
#define ETH_MAC_CONFIG_MODE_EN          1
// <q> ETH_MAC_Config_CommonParams
#define ETH_MAC_CONFIG_COMMONPARAMS_EN  1
// <q> ETH_MAC_PTP_ControlTimer
#define ETH_MAC_PTP_CONTROLTIMER_EN     1
// <q> ETH_PHY_Initialization
#define ETH_PHY_INITIALIZATION_EN       1
// <q> ETH_PHY_PowerControl
#define ETH_PHY_POWERCONTROL_EN         1
// <q> ETH_PHY_Config
#define ETH_PHY_CONFIG_EN               1
// <q> ETH_Loopback_Transfer
#define ETH_LOOPBACK_TRANSFER_EN        1
// <q> ETH_Loopback_PTP
#define ETH_LOOPBACK_PTP_EN             1
// <q> ETH_PHY_CheckInvalidInit
#define ETH_PHY_CHECKINVALIDINIT_EN     1
// <q> ETH_MAC_CheckInvalidInit
#define ETH_MAC_CHECKINVALIDINIT_EN     1
// </h>
// </h>

// <h> I2C
// <o> Driver_I2C# <0-255>
// <i> Choose the Driver_I2C# instance to be tested
#define DRV_I2C                         1
// <h> Test Cases
// <i> Enable/disable test cases
// <q> I2C_GetCapabilities
#define I2C_GETCAPABILITIES_EN          1
// <q> I2C_Initialization
#define I2C_INITIALIZATION_EN           1
// <q> I2C_PowerControl
#define I2C_POWERCONTROL_EN             1
// <q> I2C_SetBusSpeed
#define I2C_SETBUSSPEED_EN              1
// <q> I2C_SetOwnAddress
#define I2C_SETOWNADDRESS_EN            1
// <q> I2C_BusClear
#define I2C_BUSCLEAR_EN                 1
// <q> I2C_AbortTransfer
#define I2C_ABORTTRANSFER_EN            1
// <q> I2C_CheckInvalidInit
#define I2C_CHECKINVALIDINIT_EN         1
// </h>
// </h>

// <h> MCI
// <o> Driver_MCI# <0-255>
// <i> Choose the Driver_MCI# instance to be tested
#define DRV_MCI                         0
// <h> Test Cases
// <i> Enable/disable test cases
// <q> MCI_GetCapabilities
#define MCI_GETCAPABILITIES_EN          1
// <q> MCI_Initialization
#define MCI_INITIALIZATION_EN           1
// <q> MCI_PowerControl
#define MCI_POWERCONTROL_EN             1
// <q> MCI_SetBusSpeedMode
#define MCI_SETBUSSPEEDMODE_EN          1
// <q> MCI_Config_DataWidth
#define MCI_CONFIG_DATAWIDTH_EN         1
// <q> MCI_Config_CmdLineMode
#define MCI_CONFIG_CMDLINEMODE_EN       1
// <q> MCI_Config_DriverStrength
#define MCI_CONFIG_DRIVERSTRENGTH_EN    1
// <q> MCI_CheckInvalidInit
#define MCI_CHECKINVALIDINIT_EN         1
// </h>
// </h>

// <h> USB Device
// <o> Driver_USBD# <0-255>
// <i> Choose the Driver_USBD# instance to be tested
#define DRV_USBD                        0
// <h> Test Cases
// <i> Enable/disable test cases
// <q> USBD_GetCapabilities
#define USBD_GETCAPABILITIES_EN         1
// <q> USBD_Initialization
#define USBD_INITIALIZATION_EN          1
// <q> USBD_PowerControl
#define USBD_POWERCONTROL_EN            1
// <q> USBD_CheckInvalidInit
#define USBD_CHECKINVALIDINIT_EN        1
// </h>
// </h>

// <h> USB Host
// <o> Driver_USBH# <0-255>
// <i> Choose the Driver_USBH# instance to be tested
#define DRV_USBH                        0
// <h> Test Cases
// <i> Enable/disable test cases
// <q> USBH_GetCapabilities
#define USBH_GETCAPABILITIES_EN         1
// <q> USBH_Initialization
#define USBH_INITIALIZATION_EN          1
// <q> USBH_PowerControl
#define USBH_POWERCONTROL_EN            1
// <q> USBH_CheckInvalidInit
#define USBH_CHECKINVALIDINIT_EN        1
// </h>
// </h>

// <h> CAN
// <o> Driver_CAN# <0-255>
// <i> Choose the Driver_USBD# instance to be tested
#define DRV_CAN                         0
// <h> CAN bitrates
// <i> Set the CAN bitrates (kbit/s)
// <i> Value zero is ignored
// <o> CAN bitrate 1
#define CAN_BR_1                        125
// <o> CAN bitrate 2
#define CAN_BR_2                        250
// <o> CAN bitrate 3
#define CAN_BR_3                        500
// <o> CAN bitrate 4
#define CAN_BR_4                        1000
// <o> CAN bitrate 5
#define CAN_BR_5                        0
// <o> CAN bitrate 6
#define CAN_BR_6                        0
// </h>
// <o> Ratio data/arbitration bitrate
// <i> Set the ratio between data and arbitration bitrate for CAN FD
#define CAN_DATA_ARB_RATIO              8
// <o> Percentual trigger for bitrate test
// <i> Set the minimum margin between measured and expected birates (%)
#define MIN_BITRATE                     10
// <o> Transfer timeout
// <i> Set the transfer timeout (us)
#define CAN_TRANSFER_TIMEOUT            1000000
// <h> Test Cases
// <i> Enable/disable test cases
// <q> CAN_GetCapabilities
#define CAN_GETCAPABILITIES_EN          1
// <q> CAN_Initialization
#define CAN_INITIALIZATION_EN           1
// <q> CAN_PowerControl
#define CAN_POWERCONTROL_EN             1
// <q> CAN_Loopback_CheckBitrate
#define CAN_LOOPBACK_CHECK_BR_EN        1
// <q> CAN_Loopback_CheckBitrateFD
#define CAN_LOOPBACK_CHECK_BR_FD_EN     1
// <q> CAN_Loopback_Transfer
#define CAN_LOOPBACK_TRANSFER_EN        1
// <q> CAN_Loopback_TransferFD
#define CAN_LOOPBACK_TRANSFER_FD_EN     1
// <q> CAN_CheckInvalidInit
#define CAN_CHECKINVALIDINIT_EN         1
// </h>
// </h>

// <h> WiFi
// <o> Driver_WiFi# <0-255>
// <i> Choose the Driver_WiFi# instance to be tested
#define DRV_WIFI                        0
// <h> Configuration
// <i> Configuration of valid settings for driver functionality testing
// <h> Station
// <i> Settings relevant for Station
// <s.32> SSID
// <i> SSID of network that Station will connect to
#define WIFI_STA_SSID                   "SSID"
// <s.32> Password
// <i> Password of network that Station will connect to
#define WIFI_STA_PASS                   "Password"
// <o> Security Type
// <0=> Open <1=> WEP <2=> WPA <3=> WPA2
// <i> Security type of network that Station will connect to
#define WIFI_STA_SECURITY               3
// <o> Channel
// <i> WiFi channel of network that Station will connect to
// <i> Value 0 means Autodetect
#define WIFI_STA_CH                     0
// <s.8> WPS PIN
// <i> WiFi Protected Setup Personal Identification Number
#define WIFI_STA_WPS_PIN                "12345678"
// </h>
// <h> Access Point
// <i> Settings relevant for Access Point
// <s.32> SSID
// <i> SSID of created network
#define WIFI_AP_SSID                    "CMSIS_DV"
// <s.32> Password
// <i> Password of created network
#define WIFI_AP_PASS                    "Password"
// <o> Security Type
// <0=> Open <1=> WEP <2=> WPA <3=> WPA2
// <i> Security type of created network
#define WIFI_AP_SECURITY                3
// <o> Channel
// <i> WiFi channel of created network
// <i> Value 0 means Autoselect
#define WIFI_AP_CH                      0
// <s.8> WPS PIN
// <i> WiFi Protected Setup Personal Identification Number
#define WIFI_AP_WPS_PIN                 "12345678"
// </h>
// <h> Socket
// <s.15>SockServer IP
// <i>Static IPv4 Address of SockServer (used for testing socket operation) in text representation
#define WIFI_SOCKET_SERVER_IP           "192.168.1.10"
// </h>
// </h>
// <h> Test Cases
// <i> Enable/disable test cases
// <e> Control
// <i> Control functions tests
#define WIFI_CONTROL_EN                 1
// <q> WIFI_GetVersion
#define WIFI_GETVERSION_EN              1
// <q> WIFI_GetCapabilities
#define WIFI_GETCAPABILITIES_EN         1
// <q> WIFI_Initialization/Uninitilization
#define WIFI_INIT_UNINIT_EN             1
// <q> WIFI_PowerControl
#define WIFI_POWERCONTROL_EN            1
// <q> WIFI_GetModuleInfo
#define WIFI_GETMODULEINFO_EN           1
// </e>
// <e> Management
// <i> Management functions tests
#define WIFI_MANAGEMENT_EN              1
// <e> WIFI_SetOption/GetOption
#define WIFI_SETGETOPTION_EN            1
// <o0> ARM_WIFI_BSSID <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_BSSID_STA/WIFI_BSSID_AP (1E-30-6C-A2-45-5E)
#define WIFI_SETGETOPTION_BSSID_EN      3
// <o0> ARM_WIFI_TX_POWER <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_TX_POWER_STA/WIFI_TX_POWER_AP (16)
#define WIFI_SETGETOPTION_TX_POWER_EN   3
// <o0> ARM_WIFI_LP_TIMER <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_LP_TIMER_STA (10)
#define WIFI_SETGETOPTION_LP_TIMER_EN   3
// <o0> ARM_WIFI_DTIM <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_DTIM_STA/WIFI_DTIM_AP (3)
#define WIFI_SETGETOPTION_DTIM_EN       3
// <o0> ARM_WIFI_BEACON <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_BEACON_AP (2000)
#define WIFI_SETGETOPTION_BEACON_EN     3
// <o0> ARM_WIFI_MAC <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_MAC_STA/WIFI_MAC_AP (1E-30-6C-A2-45-5E)
#define WIFI_SETGETOPTION_MAC_EN        3
// <o0> ARM_WIFI_IP <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_IP_STA/WIFI_IP_AP (192.168.0.100)
#define WIFI_SETGETOPTION_IP_EN         3
// <o0> ARM_WIFI_IP_SUBNET_MASK <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_IP_SUBNET_MASK_STA/WIFI_IP_SUBNET_MASK_AP (255.255.255.0)
#define WIFI_SETGETOPTION_IP_SUBNET_MASK_EN 3
// <o0> ARM_WIFI_IP_GATEWAY <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_IP_GATEWAY_STA/WIFI_IP_GATEWAY_AP (192.168.0.254)
#define WIFI_SETGETOPTION_IP_GATEWAY_EN 3
// <o0> ARM_WIFI_IP_DNS1 <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_IP_DNS1_STA/WIFI_IP_DNS1_AP (8.8.8.8)
#define WIFI_SETGETOPTION_IP_DNS1_EN    3
// <o0> ARM_WIFI_IP_DNS2 <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_IP_DNS2_STA/WIFI_IP_DNS2_AP (8.8.4.4)
#define WIFI_SETGETOPTION_IP_DNS2_EN    3
// <o0> ARM_WIFI_IP_DHCP <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
#define WIFI_SETGETOPTION_IP_DHCP_EN    3
// <o0> ARM_WIFI_IP_DHCP_POOL_BEGIN <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_IP_DHCP_POOL_BEGIN_AP (192.168.0.100)
#define WIFI_SETGETOPTION_IP_DHCP_POOL_BEGIN_EN 3
// <o0> ARM_WIFI_IP_DHCP_POOL_END <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_IP_DHCP_POOL_END_AP (192.168.0.200)
#define WIFI_SETGETOPTION_IP_DHCP_POOL_END_EN   3
// <o0> ARM_WIFI_IP_DHCP_LEASE_TIME <0=>Disabled <1=>Set <2=>Get <3=>Set/Get
// <i> Default test value is set in defines WIFI_IP_DHCP_LEASE_TIME_AP (3600)
#define WIFI_SETGETOPTION_IP_DHCP_LEASE_TIME_EN 3
// </e>
// <q> WIFI_Scan
#define WIFI_SCAN_EN                    1
#define WIFI_SCAN_MAX_NUM               10
// <q> WIFI_Activate/Deactivate
#define WIFI_ACT_DEACT_EN               1
// <q> WIFI_IsConnected
#define WIFI_ISCONNECTED_EN             1
// <q> WIFI_GetNetInfo
#define WIFI_GETNETINFO_EN              1
// </e>
// <e> Management (requires user interaction)
// <i> Management functions tests that require user interaction
#define WIFI_MANAGEMENT_USER_EN         0
// <q> WIFI_Activate_AP
// <i> For this test please connect a WiFi client (for example mobile phone) to AP
// <i> and check if connection has succeeded
#define WIFI_ACT_AP                     1
#define WIFI_AP_CLIENT_CON_TIMEOUT      120000
// <e> WPS
#define WIFI_WPS_USER_EN                1
// <q> WIFI_Activate_Station_WPS_PBC
// <i> For this test please start WPS Push-button method on WiFi AP (router) manually
#define WIFI_ACT_STA_WPS_PBC            1
// <q> WIFI_Activate_Station_WPS_PIN
// <i> For this test please configure WPS PIN method on WiFi AP (router) manually
#define WIFI_ACT_STA_WPS_PIN            1
// <q> WIFI_Activate_AP_WPS_PBC
// <i> For this test please connect a WiFi client (for example mobile phone) to AP with WPS Push-button method
// <i> and check if connection has succeeded
#define WIFI_ACT_AP_WPS_PBC             1
// <q> WIFI_Activate_AP_WPS_PIN
// <i> For this test please connect a WiFi client (for example mobile phone) to AP with WPS PIN method
// <i> and check if connection has succeeded
#define WIFI_ACT_AP_WPS_PIN             1
// </e>
// </e>
// <e> Socket API (requires SockServer)
// <i> Socket functions tests
#define WIFI_SOCKET_EN                  1
#define WIFI_SOCKET_MAX_NUM             4
// <q> WIFI_SocketCreate
#define WIFI_SOCKETCREATE_EN            1
// <q> WIFI_SocketBind
#define WIFI_SOCKETBIND_EN              1
// <q> WIFI_SocketListen
#define WIFI_SOCKETLISTEN_EN            1
// <q> WIFI_SocketAccept
#define WIFI_SOCKETACCEPT_EN            1
// <q> WIFI_SocketConnect
#define WIFI_SOCKETCONNECT_EN           1
// <q> WIFI_SocketRecv
#define WIFI_SOCKETRECV_EN              1
// <q> WIFI_SocketRecvFrom
#define WIFI_SOCKETRECVFROM_EN          1
// <q> WIFI_SocketSend
#define WIFI_SOCKETSEND_EN              1
// <q> WIFI_SocketSendTo
#define WIFI_SOCKETSENDTO_EN            1
// <q> WIFI_SocketGetSockName
#define WIFI_SOCKETGETSOCKNAME_EN       1
// <q> WIFI_SocketGetPeerName
#define WIFI_SOCKETGETPEERNAME_EN       1
// <q> WIFI_SocketGetOpt
#define WIFI_SOCKETGETOPT_EN            1
// <q> WIFI_SocketSetOpt
#define WIFI_SOCKETSETOPT_EN            1
// <q> WIFI_SocketClose
#define WIFI_SOCKETCLOSE_EN             1
// <q> WIFI_SocketGetHostByName
#define WIFI_SOCKETGETHOSTBYNAME_EN     1
// <q> WIFI_Ping
#define WIFI_PING_EN                    1
// </e>
// <e> Socket Operation (requires SockServer)
// <i> Socket operation tests
#define WIFI_SOCKET_OP_EN               1
// <q> WIFI_Transfer_Fixed
// <i> Sends and receives in fixed size blocks
#define WIFI_TRANSFER_FIXED_EN          1
// <q> WIFI_Transfer_Incremental
// <i> Sends and receives in ascending size blocks
#define WIFI_TRANSFER_INCREMENTAL_EN    1
// <q> WIFI_Send_Fragmented
// <i> Sends several smaller blocks, receives one large block
#define WIFI_SEND_FRAGMENTED_EN         1
// <q> WIFI_Recv_Fragmented
// <i> Sends one large block, receives several smaller blocks
#define WIFI_RECV_FRAGMENTED_EN         1
// <q> WIFI_Test_Speed
// <i> Transmits data and measures transfer speed
#define WIFI_TEST_SPEED_EN              1
// <q> WIFI_Concurrent_Socket
// <i> Transmits data in two sockets simultaneously
#define WIFI_CONCURRENT_SOCKET_EN       1
// <q> WIFI_Downstream_Rate
// <i> Measures the downstream bandwidth
#define WIFI_DOWNSTREAM_RATE_EN         1
// <q> WIFI_Upstream_Rate
// <i> Measures the upstream bandwidth
#define WIFI_UPSTREAM_RATE_EN           1
// </e>
// </h>
// </h>

// Configuration settings of test values for Set/GetOptions
// BSSID
#define WIFI_BSSID_STA                  "1E-30-6C-A2-45-5E"
#define WIFI_BSSID_AP                   "1E-30-6C-A2-45-5E"
// Radio transmitting power in dBm
#define WIFI_TX_POWER_STA               16
#define WIFI_TX_POWER_AP                16
// Low-power deep-sleep time value in seconds
#define WIFI_LP_TIMER_STA               10
// Delivery Traffic Indication Message interval value in beacons
#define WIFI_DTIM_STA                   3
#define WIFI_DTIM_AP                    3
// Beacon interval value in milliseconds
#define WIFI_BEACON_AP                  2000
// Ethernet MAC Address in text representation
#define WIFI_MAC_STA                    "1E-30-6C-A2-45-5E"
#define WIFI_MAC_AP                     "1E-30-6C-A2-45-5E"
// Static IPv4 Address in text representation
#define WIFI_IP_STA                     "192.168.0.100"
#define WIFI_IP_AP                      "192.168.0.100"
// Local Subnet mask in text representation
#define WIFI_IP_SUBNET_MASK_STA         "255.255.255.0"
#define WIFI_IP_SUBNET_MASK_AP          "255.255.255.0"
// IP Address of Default Gateway in text representation
#define WIFI_IP_GATEWAY_STA             "192.168.0.254"
#define WIFI_IP_GATEWAY_AP              "192.168.0.254"
// IP Address of Primary DNS Server in text representation
#define WIFI_IP_DNS1_STA                "8.8.8.8"
#define WIFI_IP_DNS1_AP                 "8.8.8.8"
// IP Address of Secondary DNS Server in text representation
#define WIFI_IP_DNS2_STA                "8.8.4.4"
#define WIFI_IP_DNS2_AP                 "8.8.4.4"
// IP Address of DHCP server pool beginning in text representation
#define WIFI_IP_DHCP_POOL_BEGIN_AP      "192.168.0.100"
// IP Address of DHCP server pool end in text representation
#define WIFI_IP_DHCP_POOL_END_AP        "192.168.0.200"
// DHCP lease time value in seconds
#define WIFI_IP_DHCP_LEASE_TIME_AP      3600

// Timeout configuration that can be used for adjusting tests to limitations of the WiFi Module (in ms)
#define WIFI_SOCKET_TIMEOUT             2000
#define WIFI_SOCKET_TIMEOUT_LONG        35000

//------------------------------------------------------------------------------

// Device specific local loopback settings
#define SPI_LOCAL_LOOPBACK()
#define USART_LOCAL_LOOPBACK()

#endif /* __DV_CONFIG_H */
