/*! \file RcPack.c
\brief packet packing and unpacking
\details
\li Each remote call starts with header packet, followed by no or up to 256 data packages.

Remote Call header packet: an 8 byte fixed size package: 0xc0 0xde pix cad sad tid fid crc8
\code
  |--------------------------------- fixed packet start0 byte 0xc0 
  |    |---------------------------- following data package count 
  |    |   |------------------------ packet index (2 lsb packet type and and 6 msb cycle counter)
  |    |   |   |-------------------- local address byte
  |    |   |   |   |---------------- server address byte (broadcast address possible for no anser calls)
  |    |   |   |   |   |------------ type identifyer byte
  |    |   |   |   |   |   |-------- function identifyer byte
  |    |   |   |   |   |   |   |---- exclusive-or checksum byte
  v    v   v   v   v   v   v   v
0xc0  dpc pix cad sad tid fid crc8
\endcode
In case of data loss it is easy to sync by searching for 0xc0 0xde and a matchning crc8 6 bytes later. 
Just in case of a false match after a data loss inside a data stream the probability 
for a next false match is very low. So no escape mechanism is needed. Also the knowledge of the
data package structure could be used in a heuristic re-sync search (see data package description below).
The header crc8 includes all 7 bytes before (0xc0 0xde pix cad sad tid fid)


# Benutzung von ReCal auf Byte-Puffer-Ebene (ein Beispiel)
- In der Datei RcRegistry.c sind alle von ReCal nutzbaren Funktionen registriert.
- Diese Datei darf aus Kompatibilitätsgründen nur erweitert werden.
- Neue Funktionen benötigen einen passenden in FuncType[] registrierten Typ-Handler.
- Die Funktionen selbst werden im zugehörigen Typ-Array, z.B. w_coC[], registriert.
- Alle in RcRegistry.c registrierten Funktionen sind zusätzlich als Dummies mit dem Keywort __weak anzulegen, da RcRegistry.c in mehreren Projekten verwendet wird.
- Um eine Funktion direkt über eine Byte-Sequenz aufzurufen, kann diese zunächst vom TraceLog-Terminal aus testweise benutzt werden.
- Wenn dieses compiliert wird mit definiertem WIN_DEBUG_R und WIN_DEBUG_W ("C:\GitRepos\BTdev\TraceLog\TraceLogTerminal\config.h") 
so sind die Byte-Sequenzen leicht ablesbar, zum Beispiel der Funktionsaufruf stb4, der zwei ReCal Sequenzen enthält:

\code
stb4
do...

w C0 01 43 60 61 03 02 82

p 00 00 04 CE
 c0 02 41 60 61 03 02 83 <--- rcHeader

u 03 00 00 00 00 00 ce ce

u 03 00 10 00 00 00 ce ce

w C0 01 47 60 61 03 03 87

p 00 00 10 CE
 c0 02 45 60 61 03 03 86 <--- rcHeader

u 03 00 00 00 00 00 ce ce

u 0f 00 05 00 09 0f be 31 00 13 05 00 10 0f be 31 00 14 ce ce
event 0f090005 @ 130031be ms
event 0f100005 @ 140031be ms

...done
\endcode
- 1. Zeile:
\code
w C0 01 43 60 61 03 02 82 <- Ausgehender Header (w=write)
                        ^--- ex-or Checksumme
                     ^------ fid = Funktions-ID = Index 2 in w_coC[] -> Stb4FifoDepth
                  ^--------- tid = Typ-ID = Index 3 in FuncType[] -> Typ w_coC
               ^------------ Empfängeradresse (hier 0x61)
            ^--------------- Absenderadresse (hier 0x60)
         ^------------------ Bit 0 & Bit 1 sind gesetzt -> Request mit Erwartung einer Antwort, obere 6 Bit sind Zykluszähler
      ^--------------------- 0x01 = 1 -> ein Datenpaket folgt
  ^------------------------- 0xc0 = Kennung für (Command-) Header
\endcode
- 2. Zeile:
\code
p 00 00 04 CE <- Datenpaket (p=packed)
            ^--- Ende-Markierung inklusive Padding (1-4 mal 0xCE)
         ^------ 4 = Dateninhalt, hier die Anzahl der in der Antwort erwarteten Bytezahl = sizeof(uint32_t), da eine Fifo-Tiefe abgefragt wird
  lo hi <------- Anzahl minus 1 der in diesem Datenpaket verpacken Bytezahl, hier 1 Byte 
\endcode
- 3. Zeile:
\code
 c0 02 41 60 61 03 02 83 <--- rcHeader <- Das ist die Antwort
                       ^--- ex-or Checksumme
                    ^------ fid = Funktions-ID = Index 2 in w_coC[] -> Stb4FifoDepth
                 ^--------- tid = Typ-ID = Index 3 in FuncType[] -> Typ w_coC
              ^------------ Empfängeradresse (hier 0x61)
           ^--------------- Absenderadresse (hier 0x60)
        ^------------------ Bit 0 ist gesetzt & Bit 1 ist gelöscht -> erwartete Antwort, obere 6 Bit sind Zykluszähler
     ^--------------------- es folgen 2 Datenpakete
 ^------------------------- 0xc0 = Kennung für (Command-) Header: Der Antwort-Header gleicht dem ausgehenden Header bis auf das gelöschte Antwortbit und ggf. eine andere Anzahl von nachfolgenden Datenpaketen
\endcode
- 4. Zeile:
\code
u 03 00 00 00 00 00 ce ce (u=unpack)
                     ^--^------------ Ende-Markierung inklusive Padding (1-4 mal 0xCE)
         ^--^--^--^------------------ 4 Datenbytes, hier repräsentierend den Funktionsrückgabewert 0 vom Typ int32_t
   ^--^------------------------------ lo-hi Anzahl minus 1 der in diesem Datenpaket verpacken Bytezahl, hier 4 Byte 
\endcode
- 5. Zeile:
\code
u 03 00 10 00 00 00 ce ce
                     ^--^------------ Ende-Markierung inklusive Padding (1-4 mal 0xCE)
         ^--^--^--^------------------ 4 Datenbytes, hier repräsentierend den erfragten Depth-Wert 16 vom Typ uint32_t
   ^--^------------------------------ lo-hi Anzahl minus 1 der in diesem Datenpaket verpacken Bytezahl, hier 4 Byte 
\endcode

Jetzt ist klar, dass (mindestend) 16 Byte im angefragten Fifo sind.

- 6. Zeile:
\code
w C0 01 47 60 61 03 03 87
                        ^--- ex-or Checksumme
                     ^------ fid = Funktions-ID = Index 3 in w_coC[] -> Stb4FifoRead
                  ^--------- tid = Typ-ID = Index 3 in FuncType[] -> Typ w_coC
               ^------------ Empfängeradresse (hier 0x61)
            ^--------------- Absenderadresse (hier 0x60)
         ^------------------ Bit 0 & Bit 1 sind gesetzt -> Request mit Erwartung einer Antwort, obere 6 Bit sind Zykluszähler
      ^--------------------- 0x01 = 1 -> ein Datenpaket folgt
  ^------------------------- 0xc0 = Kennung für (Command-) Header
\endcode
- 7. Zeile:
\code
p 00 00 10 CE <- Datenpaket (p=packed)
            ^--- Ende-Markierung inklusive Padding (1-4 mal 0xCE)
         ^------ 16 = Dateninhalt, hier die Anzahl der in der Antwort erwarteten Bytezahl = 16, da 16 Byte im Fifo sind
  lo hi <------- Anzahl minus 1 der in diesem Datenpaket verpacken Bytezahl, hier 1 Byte 
\endcode
- 8. Zeile:
\code
 c0 02 45 60 61 03 03 86 <--- rcHeader  <- Das ist die Antwort
                        ^--- ex-or Checksumme
                     ^------ fid = Funktions-ID = Index 3 in w_coC[] -> Stb4FifoRead
                  ^--------- tid = Typ-ID = Index 3 in FuncType[] -> Typ w_coC
               ^------------ Empfängeradresse (hier 0x61)
            ^--------------- Absenderadresse (hier 0x60)
         ^------------------ Bit 0 ist gesetzt & Bit 1 ist gelöscht -> erwartete Antwort, obere 6 Bit sind Zykluszähler
      ^--------------------- es folgen 2 Datenpakete
  ^------------------------- 0xc0 = Kennung für (Command-) Header: Der Antwort-Header gleicht dem ausgehenden Header bis auf das gelöschte Antwortbit und ggf. eine andere Anzahl von nachfolgenden Datenpaketen
\endcode
- 9. Zeile:
\code
Zeile u 03 00 00 00 00 00 ce ce (u=unpack)
                           ^--^------------ Ende-Markierung inklusive Padding (1-4 mal 0xCE)
               ^--^--^--^------------------ 4 Datenbytes, hier repräsentierend den Funktionsrückgabewert 0 vom Typ int32_t
         ^--^------------------------------ lo-hi Anzahl minus 1 der in diesem Datenpaket verpacken Bytezahl, hier 4 Byte 
\endcode
- 10. Zeile:
\code
u 0f 00 05 00 09 0f be 31 00 13 05 00 10 0f be 31 00 14 ce ce (u=unpack)
                                                         ^--^------------ Ende-Markierung inklusive Padding (1-4 mal 0xCE)
         ^--^--^--^--^--^--^--^--^--^--^--^--^--^--^--^------------------ 16 Datenbytes, hier 2 Events (jeder 8 Byte)
   ^--^------------------------------ lo-hi Anzahl minus 1 der in diesem Datenpaket verpacken Bytezahl, hier 0x0f ^= 16 Byte 
\endcode

Das ist die Textausgabe der beiden Events (jeweils 8 Byte). Das erste Byte der Millisekunden ist ein Zykluszähler.

\code
event 0f090005 @ 130031be ms
event 0f100005 @ 140031be ms
\endcode


\b Remote \b call \b data \b packet (see pack and unpack)
\code
uint16_t, uint8_t, ..., uint8_t, uint8_t
 count-1,      d0, ...,      dN,   crc8
\endcode
crc8 is 0xce for count + data <=16, otherwise 8 bit exor checksum over buffer starting with 0xce

\brief structs are not exchangable between compilers
\details
Naming convention to keep type names short
  type  |shortcut|type pointer|shortcut|type pointer pointer|remark
--------------------------------------------------------------------------------------------------------------------------------------
  void  |    v   |    void*   |    V   |         Vp         |
  byte  |    b   |    byte*   |    B   |         Bp         | int8_t, uint8_t, B is pointer to one byte
  byte  |    c   |    byte*   |    C   |         Cp         | int8_t, uint8_t, C is pointer to c bytes
  half  |    h   |    half*   |    H   |         Hp         | int16_t, uint16_t, 
  word  |    w   |    word*   |    W   |         Wp         | int32_t, uint32_t, 
function|    _   |            |        |                    | Example: w_hos -> int function( uint16_t, char* )
 input  |    i   |            |        |                    | default: normally not written
 output |    o   |            |        |                    | used for pointers, ioS means string pointer address is input and output

Remote Call
\details Each device supports 2 UARTs, UARTl = left UART, UARTr = right UART. UARTl and UARTr are treated internally equally.
End devices need only one UART. Incoming remote calls on one UART (left or right) are treated this way:
\li If remote calls sad is matching device address execute it locally and send answer back to the write fifo of the incoming remote call.
\li If remote calls sad is not matching device address forward it and any following data packets to the write fifo of the other UART.
\li If remote calls sad is a broadcast address do both of the above.
It is possible to connect all devices in a ring. Then it is possible to have intact communication even one device is broken.
\startuml
\startditaa
                                ^                                                         ^                           ^
                                |                                                         |                           |
                                v                                                         v                           v
/---------------\    /---------------------\    /------------------------\    /----------------------\    /----------------------\
|               |    |                     |    |                        |    |                      |    |                      |
|  PC Service   |    |       IOTgate       |    | CoCoS & DALIcommander  |    |       DALIgate       |    |        IOgate        |
|               |    |                     |    |                    cRED|    |                      |    |                      |
|       +-------+    +-------+      +------+    +-------+        +-------+    +-------+      +-------+    +-------+      +-------+
|       | UARTr |    | UARTl |      |UARTr |    | UARTl | embed. | UARTr |    | UARTl |      | UARTr |    | UARTl |      | UARTr |
|Windows|       |    |       | RTOS |  cRED|    |       | Linux  |       |    |       | RTOS |       |    |       | RTOS |       |
|   cBLU| tx rx |    | tx rx |      |tx rx |    | tx rx |        | tx rx |    | tx rx |  cYEL| tx rx |    | tx rx |  cRED| tx rx |
\-------+-------/    \-------+------+------/    \-------+--------+-------/    \-------+------+-------/    \-------+------+-------/
           : ^          : ^           | ^          | ^              | ^          | ^            | ^          | ^            | ^  
           | |          | |           | |          | |              | |          | |            | |          | |            | |  
           | \----------/ |           | \----------/ |              | \----------/ |            | \----------/ |            | \-----
           \--------------/           \--------------/              \--------------/            \--------------/            \-------
\endditaas
\enduml



Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include "config.h" // should be first include file, at least before other project specific files

#include <stdint.h>
#include <stdarg.h>
//#include <CriticalSection.h>
//#undef TRACELOG
#if 1 // def TRACELOG
#include <stdlib.h>
#include "Fifo.h"
#endif // #ifdef TRACELOG
//#include "TraceUtilities.h"
//#include "ReadAndWrite.h"
//#include "RcUart_Interface.h"
//#include "LeftRightFifo.h"
//#include "Tr16.h"

#ifdef WINDOWS_OS
#define TRICE_OFF
#endif
#include "trice.h"

#include <string.h> // memcpy
//#include "CommonTypes.h"
//#include "DefaultMacros.h"
#include "msTicks.h"
//#include "Os.h"
#include "RcPack.h"
#include "RcRegistry.h"
//#include "TickedExecution.h"


//!< helper for file name trace log
static void reportFile( void ){
    TRICE0( Id(146), "rd_:RcPack.c" );
}

#ifdef WINDOWS_OS
#include <stdio.h> // printf
#endif


#include <stdio.h>
/*!
\param name
\param year
\param out
\retval count
*/
int greet(const char *name, int year, char *out) {
    int n;
    
    n = sprintf(out, "Hi, %s from %d! We come in peace :)", name, year);

    return n;
}


/*! Get write file descriptor based on p->sad 
\param sad server address
\return write file descriptor responsible for sad
*/
static int wFileDescriptor( uint8_t sad ){
    return LEFT; 
}


/*! Get read file descriptor based on p->sad 
\param sad server address
\return read file descriptor responsible for sad
*/
static int rFileDescriptor( uint8_t sad ){
    return LEFT; 
}


/*! internal helper function for checksum computation
\param crc8 start value
\param count amount of data bytes
\param pBuff data address
\result computed crc8
*/
static inline uint8_t computeCrc8( uint8_t crc8, size_t count, const uint8_t* pBuff ){
    while( count-->0 ){
        crc8 ^= *pBuff++;
    }
    return crc8;
}


//     lint -e718 -e24 -e5901 -e746 -e603 -e530 -e644 -e715

/*!
remote call type: (part of pix) 
      bit1      |      bit0       | meaning
  RC_CMD_FLAG   | RC_ANSWER_FLAG  |
----------------|-----------------|------------------------------------------
        1       |        1        | \b Cmd = command expecting answer
        0       |        1        | answer to a command expecting answer
        1       |        0        | \b Msg = command not expecting an answer (message)
        0       |        0        | reserved
*/

#define RC_CMD_FLAG       2 //!< bit1, see pix
#define RC_ANSWER_FLAG    1 //!< bit0, see pix


const rc_t RcCmd  = RC_CMD_FLAG | RC_ANSWER_FLAG; //!< 0x3 = command with data (expecting answer)
const rc_t RcAns  =               RC_ANSWER_FLAG; //!< 0x1 = answer with data to a command 
const rc_t RcMsg  = RC_CMD_FLAG;                  //!< 0x2 = message with data (no answer)
                                                  //   0x0 = reserved

#define RC_HEADER_SIZE 8 //!< helper for better readability

// static const size_t rcHeaderSize = 8; 
static RcRemote_t rcAnswerReceived = {0}; //!< any outgoing request expecting an answer checks against this struct
static uint8_t cycleCounter = 0x20; //!< one cycle counter for all sad is ok


/*! The cycle counter is incremented in steps of 4 because the 2 lsb are used as packet type.
\param pCycle pointer to cycle counter
*/
static void incrementCycleCounter( uint8_t* pCycle ){
    cycleCounter += 0x04;
}


/*! check if pFn is registered (search in registry for matching tid and fid) and put tid and fid into pRc struct
\param pFn function address to find
\param pRc pointer to remote call struct
\retval RESULT_OK success
\retval RESULT_ERROR not found
*/
static int findRegTidAndFid( const void* pFn, RcRemote_t* pRc) {
    uint8_t tid, fid;

    for (tid = 0u; tid < RcRegistry.TypeElements; tid++) {
        for (fid = 0u; fid < RcRegistry.pFunctionType[tid].FunctionElements; fid++) {
            void** array = (void**)(RcRegistry.pFunctionType[tid].Array);
            if (array[fid] == pFn) {
                pRc->tid = tid;
                pRc->fid = fid;
                return RESULT_OK;
            }
        }
    }
    return RESULT_ERROR;
}


/*! Write header to write fifo
\param pR pointer to remote struct to be filled
\retval RESULT_OK success
\retval RESULT_TIMEOUT write fifo full
\retval RESULT_ERROR data mismatch
*/
static int rcWrHeader( RcRemote_t * pR ){
#ifdef WIN_DEBUG_W
    printf("\nw");
#endif // #ifdef WIN_DEBUG_W
    uint8_t fdW = wFileDescriptor( pR->sad ); //! file descriptor for write
    pR->c0 = 0xc0; // fill start byte
    pR->crc8 = computeCrc8( 0xc0, RC_HEADER_SIZE - 2, &pR->dpc );
    WRITE_OR_RETURN_RESULT_TIMEOUT( fdW, &pR->c0, RC_HEADER_SIZE );
#ifdef WIN_DEBUG_W
	printf("\n");
#endif // #ifdef WIN_DEBUG_W
	return RESULT_OK;
}


/*! Read header from read fifo
\param p pointer to remote struct to be filled
\retval RESULT_OK success
\retval RESULT_TIMEOUT write fifo full
\retval RESULT_ERROR data mismatch
*/
static int rcRdHeader( RcRemote_t * const p ){
    uint8_t crc8;
    int r = rFileDescriptor( p->sad );
    int c;
    c = Read( r, &p->c0, RC_HEADER_SIZE ); // trypout code is Read1()
    if( c < 0 ){
        return c;
    }
    if (0xc0 == p->c0) {
        crc8 = computeCrc8(0xc0, RC_HEADER_SIZE-2, &p->dpc);
        if (crc8 == p->crc8) {
#ifdef WIN_DEBUG_R
			printf(" <--- rcHeader\n");
			//nextReadLine = 1;
#endif // #ifdef WIN_DEBUG_R
			return RESULT_OK;
        }
    }
#ifdef WINDOWS_OS
     printf("TLT:On device %02x some data garbage received:\n", RC_LOCAL_ADDRESS );
#else
    TRICE8_1( Id(168), "WRN:On device %02x some data garbage received:\n", RC_LOCAL_ADDRESS );
#endif
    {
        for(;;){
            // move 1 byte forward
#ifdef WINDOWS_OS
            printf("wrn:TLT: removing received byte %02x\n", p->c0);
#else
            TRICE8_1( Id(167), "wrn:STM32: removing received byte %02x\n", p->c0);
#endif
            memcpy(&(p->c0), &(p->dpc), RC_HEADER_SIZE-1); // move data 1 byte
            c = Read(r, &(p->crc8), 1); // read next byte
            if( c < 0 ){
                return c;
            }
            if (0xc0 == p->c0) {
                crc8 = computeCrc8(0xc0, RC_HEADER_SIZE-2, &p->dpc);
                if (crc8 == p->crc8) {
#ifdef WIN_DEBUG_R
					printf(" <--- rcHeader\n");

					//printf("\nr");
					//nextReadLine = 1;
#endif // #ifdef WIN_DEBUG_R
					return RESULT_OK;
                }
            }
        }
    }
}


/*! internal helper function for remotely calling pFn
This fn does not return until an answer is received or timeout is reached.
\param sad server address
\param t remote call type
\param pFn function type handler for serializing and de-serializing
\param argp pointer fo variable parameter list of variadic function
\retval RESULT_OK success
\retval RESULT_TIMEOUT write fifo full
\retval RESULT_ERROR data mismatch
\todo sad
*/
static int vCallRemote( uint8_t sad, rc_t t,  const void* pFn, const va_list argp ){
    int result;
    RcRemote_t send; // this instance lives until answer was received (or timout)
    incrementCycleCounter(&cycleCounter);
    send.pix = cycleCounter | t; // RC_ANSWER_FLAG and the like
    send.cad = RC_LOCAL_ADDRESS; // set client address
    send.sad = sad; // set destination (server address)

    // set reg, tid, fid inside send struct
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == findRegTidAndFid( pFn, &send ) );

    // serialize, call and de-serialize (see type handler functions like int v_cC_Client( RcRemote_t* pS, va_list argp))
    result = (RcRegistry.pFunctionType[send.tid].ClientSide)( &send, argp );
    return result;
}


/*! client side generic wrapper
\details only variadic parameter handling here
Usage to call FunXY on Server:

normal example: (imagine sad == DALIgate)
\code
int result;
ASSERT( RESULT_OK == Rc( sad, Cmd, FunXY, &result, x, pc ) ); // int FunXY( int x, char* pc);
\endcode

similar but distinct examples:
\code
// FunZ and Funz are called identical here, but the typehandler know how to do
int x;
ASSERT( RESULT_OK == CallRemote( sad, Cmd, FunZ, &x ) ); // void FunZ( int* x );
ASSERT( RESULT_OK == CallRemote( sad, Cmd, Funz, &x ) ); // int Funz( void );
\endcode

special case void void:
\code
ASSERT( RESULT_OK == CallRemote( sad, FunVV ) ); // void FunVV( void );
\endcode

If a remote call is expecting an anwer this is coded inside the appropriate type handler tid.

\param sad server address
\param t remote call type (Cmd, Msg, CmdNoData, MsgNoData)
\param pFn function type handler for serializing and de-serializing
\retval RESULT_OK success
\retval RESULT_TIMEOUT write fifo full
\retval RESULT_ERROR data mismatch
*/
int Rc( uint8_t sad, rc_t t, const void* pFn, ... ){
    int result;

    va_list argp;
    va_start( argp, pFn ); //lint !e718 After calling va_start, argp points at the first var-argument.

    result = vCallRemote( sad, t, pFn, argp );

    /*if( RESULT_TIMEOUT == result ){
        REPORT_FAILURE( sad );
        alternateSadDirection( sad );
    }
    result = vCallRemote( sad, t, pFn, argp );*/
    
    va_end(argp); //lint !e718
    return result;
}


/*! Pack up to 64KB in one data package 
\param p pointer to RcRemote_t struct
\param size 1...65536 are valid values
\param pBuff start address of data to pack
\retval RESULT_OK success
\retval RESULT_TIMEOUT no more data writable
\retval RESULT_ERROR no success
*/
static int pack( RcRemote_t* p, size_t size, const uint8_t* pBuff ){
    uint16_t psiz_1 = (uint16_t)(size - 1); // number - 1 value
    uint8_t crc8 = 0xce;
    uint8_t fdW = wFileDescriptor( p->sad ); //! file descriptor for write
#ifdef WIN_DEBUG_W
    printf("\np");
#endif // #ifdef WIN_DEBUG_W
    ASSERT_OR_RETURN_RESULT_ERROR( size <= 65536u );
    if( size > 16 ){
        crc8 = computeCrc8( crc8, size, pBuff );
    }
    {
        WRITE_OR_RETURN_RESULT_TIMEOUT( fdW, (uint8_t*)&psiz_1, sizeof(psiz_1)); 
        WRITE_OR_RETURN_RESULT_TIMEOUT( fdW, pBuff, size);
        WRITE_OR_RETURN_RESULT_TIMEOUT( fdW, &crc8, sizeof(crc8));
#       ifdef TRACELOG_WITH_PADDING
        {
            static const uint8_t pad[3] = { 0xce, 0xce, 0xce };
            size_t paddingSize = sizeof(psiz_1) + size + sizeof(crc8);
            size_t padSize = 0x03 & (4 - (0x03 & paddingSize));
            if( padSize ){
                WRITE_OR_RETURN_RESULT_TIMEOUT( fdW, pad, padSize );
            }
        }
#       endif // #ifdef TRACELOG_WITH_PADDING
    }
#ifdef WIN_DEBUG_W
	printf("\n");
#endif // #ifdef WIN_DEBUG_W
	return RESULT_OK;
}


/*! Get up to 64KB data from fifo. Expects in the read file a data package starting with count_1 and ending with crc8 according to description. 
\param p pointer to fifo struct
\param pSize 1...65536 are valid input and return values, maximum input bytes are unpacked, otherwise error
\param pBuff where to put values
\retval RESULT_OK success
\retval RESULT_NEXT_HEADER_IN_FIFO already 1st header start byte removed
\retval RESULT_TIMEOUT no more data
\retval RES�LT_ERROR data mismatch
*/
static int unpack( RcRemote_t* p, size_t* pSize, uint8_t* pBuff ){
    uint16_t psiz_1, psiz;
    uint8_t fd = rFileDescriptor( p->sad ); //! file descriptor for read
    uint8_t crc8cmp, crc8 = 0xce; // init crc8
#ifdef WIN_DEBUG_R
    printf("\nu");
#endif // #ifdef WIN_DEBUG_R
    READ_OR_RETURN_RESULT_TIMEOUT( fd, (uint8_t*)&psiz_1, sizeof(psiz_1) ); // get the 16bit count_1 value
    ASSERT_OR_RETURN_RESULT_ERROR(*pSize > psiz_1); // limitate
    *pSize = psiz = psiz_1 + 1;
    READ_OR_RETURN_RESULT_TIMEOUT(fd, pBuff, psiz);
    if( psiz > 16 ){
        crc8 = computeCrc8( crc8, *pSize, pBuff );   //lint !e530 update crc8
    }
    READ_OR_RETURN_RESULT_TIMEOUT( fd, &crc8cmp, 1 ); // crc8
    if (crc8 != crc8cmp) {
        return RESULT_ERROR;
    }
#   ifdef TRACELOG_WITH_PADDING
    {
        uint8_t pad[3];
        size_t paddingSize = sizeof(psiz_1) + psiz + sizeof(crc8);
        size_t padSize = 0x03 & (4 - (0x03 & paddingSize));
        if( padSize ){
            READ_OR_RETURN_RESULT_TIMEOUT( fd, pad, padSize );
        }
    }
#   endif // #ifdef TRACELOG_WITH_PADDING
#ifdef WIN_DEBUG_R
	printf("\n");
#endif // #ifdef WIN_DEBUG_R
    return RESULT_OK; //lint !e438
}


/*! Check for an answer match.
\param pSend pointer to remote struct waiting for an answer
\param pReceived pointer to a received answer header
\retval RESULT_OK match!
\retval RESULT_ERROR no match
*/
static int compareRemoteReceived( const RcRemote_t* pSend, const RcRemote_t* pReceived ){
    if((pSend->pix>>2 == pReceived->pix>>2) // cycle counter match
    && (pSend->cad == pReceived->cad) // the local address
    && (pSend->sad == pReceived->sad) // the addressed server
    && (pSend->tid == pReceived->tid)
    && (pSend->fid == pReceived->fid) ){
        //#ifndef WINDOWS_OS
        //extern int RcDebug;
        //if( RcDebug ){
        //    TRICE8_5( Id(5644), "dbg:\ncompareRemoteReceived %02x %02x %02x %02x %02x\n", pSend->pix,         pSend->cad,     pSend->sad,     pSend->tid,     pSend->fid);
        //    TRICE8_5( Id(5645),   "dbg:compareRemoteReceived %02x %02x %02x %02x %02x\n", pReceived->pix, pReceived->cad, pReceived->sad, pReceived->tid, pReceived->fid);
        //}
        //#endif
        return RESULT_OK;
    }else{
        //#ifdef WINDOWS_OS
        ////printf("\n%02x %02x %02x %02x %02x\n", pSend->pix,         pSend->cad,     pSend->sad,     pSend->tid,     pSend->fid);
        ////printf(  "%02x %02x %02x %02x %02x\n", pReceived->pix, pReceived->cad, pReceived->sad, pReceived->tid, pReceived->fid);
        //#else
        //#endif
        return RESULT_ERROR;
    }
}


/*! This function is entered after a remote call was send and when an answer is expected
\param pSend pointer to remote calling context
\param pReceived pointer to received remote call answer header
\retval RESULT_OK valid answer received
\retval RESULT_TIMEOUT no answer received within timeout
*/
static int waitForRemoteAnswer( const RcRemote_t* pSend, const RcRemote_t* pReceived ){
    uint32_t timer;
    StartMsTimer( &timer );
    do{
        Pause(); // perform write to and read from
        ASSERT_OR_RETURN_RESULT( ( ElapsedMs( &timer ) < DEFAULT_MS_TIMEOUT), RESULT_TIMEOUT );
    } while( RESULT_OK != compareRemoteReceived( pSend, pReceived ) );
    return RESULT_OK;
}



/*! this is called after a header was received
It calls the appropriate type handler for de-serialization, function call and serialization
which is defined in server specific code
\param pReceived pointer to remote interface
\param ... dummy to use va_list without compiler warnings
\retval 0 success
*/
static int rcExecute( RcRemote_t* pReceived, ... ){
    int result;
    //volatile int t; 
    TypeHandler_t pFn = RcRegistry.pFunctionType[pReceived->tid].ServerSide;
    va_list argp; // dummy
    va_start( argp, pReceived );  // dummy
    result = (pFn)(pReceived, argp); // follow up in w_coC_Server (as example)
    va_end(argp); // dummy
    return result;
}


/*! This fn assumes a valid data packet inside pRc->rd and transfers it to pRc->wr fifo
\todo implement this fn with less memory needs
\param p pRc pointer to remote call struct containing a header not for this device
\retval RESULT_OK success
\retval RESULT_NEXT_HEADER_IN_FIFO already 1st header start byte removed
\retval RESULT_TIMEOUT no more data
\retval RESULT_ERROR data mismatch
*/
static int forwardDataPacket( RcRemote_t * p ){
//	/*
//    uint16_t size;
//    uint8_t buf[40];
//    size_t c, pcnt;
//    int result = Read(p->fdR, (uint8_t*)&size, sizeof(size));
//    ASSERT_OR_RETURN_RESULT( RESULT_OK == result, result );
//    
//    pcnt = size < sizeof(buf) ? size : sizeof(buf);
//    do{
//        c = my_read( p->fdR, buf, pcnt );
//        pcnt -= c;
//    }while( pcnt );
//    */
//    // \todo: special case size>sizeof(buf) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    return RESULT_OK;
}


/*! This fn assumes a valid Header in pRc, transmits it to pRc.wr fifo and coppies any following data to pRc.wr
\param p pRc pointer to remote call struct containing a header not for this device
\retval RESULT_NEXT_HEADER_IN_FIFO = usual case
\retval RESULT_TIMEOUT = no more data
\retval RESULT_ERROR = data mismatch
\details Fn never returns RESULT_OK!
*/
static int forwardRequest( RcRemote_t * const p ){
    int result;
    uint8_t dpc;
    
    // set forward direction based on receive direction
//    p->fdW = (LEFT == p->fdW) ? RIGHT : LEFT;
    
    // forward header
    result = rcWrHeader( p );
    ASSERT_OR_RETURN_RESULT( RESULT_OK == result, result );
    
    dpc = p->dpc;
    while( (dpc-- > 0) && (RESULT_OK == result ) ){
        result = forwardDataPacket( p );
    };
    return result;
}


/*! cyclically check receive fifo and perform derived action
\param pRc pointer to fifo struct (left or right)
\retval RESULT_OK success
\retval RESULT_NEXT_HEADER_IN_FIFO already 1st header start byte removed
\retval RESULT_TIMEOUT no more data
\retval RESULT_ERROR data mismatch
*/
static int rcServe( RcRemote_t* pRc ){
    int result =  rcRdHeader( pRc ); // pRc does not fill fields d0 and de, because it is not needed here!
    if (RESULT_OK != result) {
        return result;
    }else if( (RC_LOCAL_ADDRESS != pRc->sad) && (RC_LOCAL_ADDRESS != pRc->cad) ){ // not for this device
        return forwardRequest( pRc ); 
    }else if((RC_LOCAL_ADDRESS == pRc->cad) && (RC_ANSWER_FLAG & pRc->pix) ){ // an answer for this device
        #ifndef WINDOWS_OS
        //TRICE8_1( Id(5646), "dbg:%02x:an answer for this device\n", RC_LOCAL_ADDRESS );
        #else
        //printf( "DEBUG: TLT: an answer for this device\n" );
        #endif
        memcpy( &rcAnswerReceived, pRc, sizeof(RcRemote_t) ); // do nothing, let waiting request handle this, just copy header
        Pause(); // give control to calling context expecting a match and handling it there
        return RESULT_OK;
    }else if( (RC_LOCAL_ADDRESS == pRc->sad) && (RC_CMD_FLAG & pRc->pix) ) { // a remote call received
        #ifndef WINDOWS_OS
//        TRICE8_1( Id(5647), "dbg:%02x:a remote call received\n", RC_LOCAL_ADDRESS );
        #endif
        // Inside rcExecute() an answer is generated. If for some reason an active waiting takes part, a remote call like a tracelog
        // could be triggered. This causes disorder on the Uart line
        result = rcExecute( pRc ); // handle call
        return result;
    } else {
        REPORT_FAILURE( pRc->pix );
        REPORT_FAILURE( pRc->sad );
        REPORT_FAILURE( pRc->cad );
        REPORT_FAILURE( pRc->tid );
        REPORT_FAILURE( pRc->fid );
        return RESULT_ERROR;
    }
}


///*! RcRxServer is called cyclically in Pause() and therefore it must be protected against itself
//*/
static uint8_t enableRcRxServer = 1;


/*! To be called cyclically. Needed also for answers and forwarding requests.
*/
void RcRxServer( void ){
    //REENTRY_PROTECTION_BEGIN
    RcRemote_t rc;
    if( 0 == enableRcRxServer ){
        return; // avoid data reading
    }
    enableRcRxServer = 0;{
      rcServe(&rc);
    }
    enableRcRxServer = 1;
    //REENTRY_PROTECTION_END
}


/*! Set data package count and write header to write fifo
\param dpc data package count
\param p pointer to remote struct to be filled
\retval RESULT_OK success
\retval RESULT_TIMEOUT write fifo full
\retval RESULT_ERROR data mismatch
*/
static int rcWrRequestHeader( uint8_t dpc, RcRemote_t* p ){
    p->dpc = dpc; // set following data package count
    return rcWrHeader( p ); // pack header in write fifo
}


/*! Clear CMD flag and rite header to write fifo
\param dpc data package count
\param p pointer to remote struct to be filled
\retval RESULT_OK success
\retval RESULT_TIMEOUT write fifo full
\retval RESULT_ERROR data mismatch
*/
static int rcWrAnswerHeader( uint8_t dpc, RcRemote_t* p ){
    p->pix &= ~RC_CMD_FLAG; // clear command flag
    return rcWrRequestHeader( dpc, p ); // pack header in write fifo
}


////////////////////////////////////////////// type handler //////////////////////////////////////////////
//
//


/*! client side handler for functions of type
\code
void pFn( uint8_t count, const uint8_t* buffer );
\endcode
These functions ecpect in a buffer count bytes and returns void
\param p pointer to remote interface
\param argp not used on server side
\retval RESULT_OK success
*/
int32_t v_cC_Client(RcRemote_t* p, va_list argp) {
    // get args from arg list
    uint8_t  b = va_arg(argp, int); // arguments are promoted to int
    uint8_t* B = va_arg(argp, uint8_t*); // address where to get data
    ENTER_CRITICAL_SECTION
        ASSERT_OR_RETURN_RESULT_ERROR(RESULT_OK == rcWrRequestHeader( 2, p ));
        ASSERT_OR_RETURN_RESULT_ERROR(RESULT_OK == pack(p, sizeof(uint8_t), &b));
        ASSERT_OR_RETURN_RESULT_ERROR(RESULT_OK == pack(p, b, B));
    LEAVE_CRITICAL_SECTION

    // wait
    if (RC_ANSWER_FLAG & p->pix) { // wait for matching answer now if an answer is expected
        ASSERT_OR_RETURN_RESULT_ERROR(RESULT_OK == waitForRemoteAnswer(p, &rcAnswerReceived));
    }
    enableRcRxServer = 1;
    return RESULT_OK;
}


/*! server side handler for functions of type
\code
void pFn( uint8_t count, const uint8_t* buffer );
\endcode
These functions ecpect in a buffer count bytes and returns void
\param p pointer to remote interface
\param argp not used on server side
\todo limitate max buffer size
\retval RESULT_OK success
*/
int32_t v_cC_Server(RcRemote_t* p, va_list argp) {
    v_cC_t const* const array = (v_cC_t*)(RcRegistry.pFunctionType[p->tid].Array);
    uint8_t b;
    size_t bsiz = sizeof(b);
    uint8_t B[80];
    size_t Bsiz = sizeof(B);

    // de-serialize
    ASSERT_OR_RETURN_RESULT_ERROR(RESULT_OK == unpack(p, &bsiz, &b));
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR(sizeof(uint8_t) == bsiz);
    ASSERT_OR_RETURN_RESULT_ERROR(RESULT_OK == unpack(p, &Bsiz, B));
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR(Bsiz < 8);
    enableRcRxServer = 1;

    // execute
    (*array[p->fid])(b, B);

    if (RC_ANSWER_FLAG & p->pix) { // if answer expected
        ENTER_CRITICAL_SECTION
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 0, p ) );
        LEAVE_CRITICAL_SECTION
    }

    UNUSED_PARAMETER(argp);
    return RESULT_OK;
}


/*! client side handler for functions of type
\code
void pFn( const uint8_t* buffer );
\endcode
These functions ecpect in a buffer on the first 2 bytes a LoHi count and count following bytes and return void
\param p pointer to remote interface
\param argp not used on server side
\retval RESULT_OK success
*/
int32_t v_C_Client(RcRemote_t* p, va_list argp) {
#if 0 // this code is not used becaue tracelog buffers are prefilled
    // get args from arg list
    uint16_t* B = va_arg(argp, uint16_t*); // address where to get data
    size_t b = 1 + *B++;
    ENTER_CRITICAL_SECTION
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrRequestHeader( 1, p ));
        ASSERT_OR_RETURN_RESULT_ERROR(RESULT_OK == pack(p, b, (uint8_t*)B));
    LEAVE_CRITICAL_SECTION

    // wait
    if (RC_ANSWER_FLAG & p->pix) { // wait for matching answer now if an answer is expected
        ASSERT_OR_RETURN_RESULT_ERROR(RESULT_OK == waitForRemoteAnswer(p, &rcAnswerReceived));
    }
    enableRcRxServer = 1;
#endif
    return RESULT_OK;
}


/*! server side handler for functions of type
\code
void pFn( const uint8_t* buffer );
\endcode
These functions ecpect in a buffer on the first 2 bytes a LoHi count and count following bytes and return void
\param p pointer to remote interface
\param argp not used on server side
\todo limitate max buffer size
\retval RESULT_OK success
*/
int32_t v_C_Server(RcRemote_t* p, va_list argp) {
    v_C_t const* const array = (v_C_t*)(RcRegistry.pFunctionType[p->tid].Array);
    uint8_t B[40];
    size_t Bsiz = sizeof(B);
    memset(B, 0xcc, sizeof(B)); // debug

    // de-serialize
    ASSERT_OR_RETURN_RESULT_ERROR(RESULT_OK == unpack(p, &Bsiz, B+sizeof(uint16_t)));
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR(Bsiz < sizeof(B));
    enableRcRxServer = 1;
    *(uint16_t*)B = (uint16_t)(Bsiz - 1);

    // execute
    (*array[p->fid])(B);

    if (RC_ANSWER_FLAG & p->pix) { // if answer expected
        ENTER_CRITICAL_SECTION
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 0, p ) );
        LEAVE_CRITICAL_SECTION
    }

    UNUSED_PARAMETER(argp);
    return RESULT_OK;
}


/*! client side handler for functions of type
\code
int32_t pFn( uint8_t c, OUT uint8_t* C );
\endcode
These functions exspect 1 count and a byte address where to store count bytes and returns an int32_t as result.
\param p pointer to remote interface
\param argp not used on server side
\retval RESULT_OK success
*/
int32_t w_coC_Client( RcRemote_t* p, va_list argp ){
    // get args from arg list
    int32_t* pResult = va_arg( argp, int32_t* ); // address where to put result of serverfunction itself
    uint8_t c = va_arg( argp, int ); // arguments are promoted to int
    uint8_t* C = va_arg( argp, uint8_t* ); // address where to put Response of serverfunction
    ENTER_CRITICAL_SECTION
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrRequestHeader( 1, p ));
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(uint8_t), &c ) );
    LEAVE_CRITICAL_SECTION

    // wait
    if( RC_ANSWER_FLAG & p->pix ){ // wait for matching answer now if an answer is expected
        size_t Rsiz = sizeof(pResult);
        size_t csiz = c;
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == waitForRemoteAnswer( p, &rcAnswerReceived ));

        // de-serialize
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &Rsiz, (uint8_t*)pResult ) );
        // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof( int32_t ) == Rsiz );
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &csiz, C ) );
        // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( c == csiz );
    }
    enableRcRxServer = 1;
    return RESULT_OK;
}


/*! server side handler for functions of type
\code
int32_t pFn( uint8_t c, uint8_t* C );
\endcode
These functions exspect 1 count and a byte address where to store count bytes and returns an int32_t as result.
\param p pointer to received remote interface
\param v argp not used on server side
\retval RESULT_OK success
*/
int32_t w_coC_Server( RcRemote_t* p, va_list v){
    w_coC_t const * const array = (w_coC_t*)(RcRegistry.pFunctionType[p->tid].Array);
    uint8_t c;
    size_t csiz = sizeof(c);
    uint8_t C[256];
    int result;

    // de-serialize
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &csiz, &c ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(c) == csiz );
    enableRcRxServer = 1;

    // execute
    result = (*array[p->fid])( c, C ); // call local function requested remotely

    if( RC_ANSWER_FLAG & p->pix ){ // if answer expected
        ENTER_CRITICAL_SECTION
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 2, p ) );
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(int), (uint8_t*)&result ) );
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, c, C ) );
        LEAVE_CRITICAL_SECTION
    }

    UNUSED_PARAMETER( v );
    return RESULT_OK;
}


/*! client side handler for functions of type
\code
int32_t pFn( uint8_t c, const uint8_t* C );
\endcode
These functions exspect 1 count and a byte address where count bytes and returns an int32_t as result.
\param p pointer to remote interface
\param argp not used on server side
\retval RESULT_OK success
*/
int32_t w_cC_Client( RcRemote_t* p, va_list argp ){
    // get args from arg list
    int32_t* pResult = va_arg( argp, int32_t* ); // address where to put result of serverfunction itself
    uint8_t c = va_arg( argp, int ); // arguments are promoted to int
    uint8_t* C = va_arg( argp, uint8_t* ); // address where data
    ENTER_CRITICAL_SECTION
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrRequestHeader( 2, p ));
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(uint8_t), &c) );
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, c, C ) );
    LEAVE_CRITICAL_SECTION

    // wait
    if( RC_ANSWER_FLAG & p->pix ){ // wait for matching answer now if an answer is expected
        size_t rSiz = sizeof(pResult);
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == waitForRemoteAnswer( p, &rcAnswerReceived ));

        // de-serialize
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &rSiz, (uint8_t*)pResult ) );
        // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof( int32_t ) == rSiz );
    }
    enableRcRxServer = 1;
    return RESULT_OK;
}


/*! server side handler for functions of type
\code
int32_t pFn( uint8_t c, const uint8_t* C );
\endcode
These functions exspect 1 count and a byte address where count bytes and returns an int32_t as result.
\param p pointer to remote interface
\param v argp not used on server side
\retval RESULT_OK success
*/
int32_t w_cC_Server( RcRemote_t* p, va_list v){
    w_cC_t const * const array = (w_cC_t*)(RcRegistry.pFunctionType[p->tid].Array);
    uint8_t c;
    size_t csiz = sizeof(c);
    uint8_t C[256];
    size_t Csiz = sizeof(C);
    int result;
    
    // de-serialize
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &csiz, &c ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == csiz );
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &Csiz, C ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( c == Csiz );
    enableRcRxServer = 1;

    // execute
    result = (*array[p->fid])( c, C );
    
    if( RC_ANSWER_FLAG & p->pix ){ // if answer expected
        ENTER_CRITICAL_SECTION
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 1, p ) );
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(int), (uint8_t*)&result ) );
        LEAVE_CRITICAL_SECTION
    }

    UNUSED_PARAMETER( v );
    return RESULT_OK;
}


/*! client side handler for functions of type
\code
int32_t pFn( uint8_t c, const uint8_t* C );
\endcode
These functions exspect 1 count and a byte address where count bytes and returns an int32_t as result.
\param p pointer to remote interface
\param argp not used on server side
\retval RESULT_OK success
*/
int32_t w_cCcoC_Client( RcRemote_t* p, va_list argp ){
    // get args from arg list
    int32_t* pResult = va_arg( argp, int32_t* ); // address where to put result of serverfunction itself
    uint8_t c = va_arg( argp, int ); // arguments are promoted to int
    uint8_t* C = va_arg( argp, void* ); // address where data
    uint8_t oc = va_arg( argp, int ); // arguments are promoted to int
    uint8_t* oC = va_arg( argp, void* ); // address where to put data
    ENTER_CRITICAL_SECTION
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrRequestHeader( 3, p ));
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(uint8_t), &c ) );
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, c, C ) );
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(uint8_t), &oc ) );
    LEAVE_CRITICAL_SECTION

    // wait
    if( RC_ANSWER_FLAG & p->pix ){ // wait for matching answer now if an answer is expected
        size_t rSiz = sizeof( pResult);
        size_t ocSiz = oc;
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == waitForRemoteAnswer( p, &rcAnswerReceived ));

        // de-serialize
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &rSiz, (uint8_t*)pResult ) );
        // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof( int32_t ) == rSiz);
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &ocSiz, oC ) );
        // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( oc == ocSiz);
    }
    enableRcRxServer = 1;
    return RESULT_OK;
}


/*! server side handler for functions of type
\code
int32_t pFn( uint8_t c, const uint8_t* C );
\endcode
These functions exspect 1 count and a byte address where count bytes and returns an int32_t as result.
\param p pointer to received remote interface
\param v argp not used on server side
\retval RESULT_OK success
*/
int32_t w_cCcoC_Server( RcRemote_t* p, va_list v){
    w_cCcoC_t const * const array = (w_cCcoC_t*)(RcRegistry.pFunctionType[p->tid].Array);
    uint8_t c;
    size_t csiz = sizeof(c);
    uint8_t C[256];
    size_t Csiz = sizeof(C);
    uint8_t oc;
    size_t ocsiz = sizeof(oc);
    uint8_t OC[256];
    int result;
    
    // de-serialize
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &csiz, &c ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == csiz );
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &Csiz, C ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( c == Csiz );
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &ocsiz, &oc ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == ocsiz );
    enableRcRxServer = 1;

    // execute
    result = (*array[p->fid])( c, C, oc, OC );
    
    if( RC_ANSWER_FLAG & p->pix ){ // if answer expected
        ENTER_CRITICAL_SECTION
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 2, p ) );
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(int), (uint8_t*)&result ) );
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, oc, OC ) );
        LEAVE_CRITICAL_SECTION
    }

    UNUSED_PARAMETER( v );
    return RESULT_OK;
}


/*! client side handler for functions of type
\code
int32_t pFn( uint8_t byte0, uint8_t byte1, OUT uint8_t* byte2 );
\endcode
These functions exspect 2 bytes and a byte address where to store one byte return an int32_t as result.
\param p pointer to remote interface
\param argp not used on server side
\retval RESULT_OK success
*/
int32_t w_bboB_Client( RcRemote_t* p, va_list argp ){
    // get args from arg list
    int32_t* pResult = va_arg( argp, int32_t* ); // address where to put result of serverfunction itself
    uint8_t a = va_arg( argp, int ); // arguments are promoted to int
    uint8_t b = va_arg( argp, int ); // arguments are promoted to int
    uint8_t* B = va_arg( argp, uint8_t* ); // address where to put Response of serverfunction
    ENTER_CRITICAL_SECTION
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrRequestHeader( 2, p ));
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(uint8_t), &a) );
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(uint8_t), &b) );
    LEAVE_CRITICAL_SECTION

    // wait
    if( RC_ANSWER_FLAG & p->pix ){ // wait for matching answer now if an answer is expected
        size_t rSiz = sizeof(pResult);
        size_t bsiz = sizeof(b);
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == waitForRemoteAnswer( p, &rcAnswerReceived ));

        // de-serialize
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &rSiz, (uint8_t*)pResult ) );
        // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof( int ) == rSiz);
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &bsiz, B ) );
        // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof( uint8_t ) == bsiz);
    }
    enableRcRxServer = 1;
    return RESULT_OK;
}


/*! server side handler for functions of type
\code
int32_t pFn( uint8_t byte0, uint8_t byte1, OUT uint8_t* byte2 );
\endcode
These functions exspect 2 bytes and a byte address where to store one byte return an int32_t as result.
\param p pointer to received remote interface
\param v argp not used on server side
\retval RESULT_OK success
*/
int32_t w_bboB_Server( RcRemote_t* p, va_list v){
    w_bboB_t const * const array = (w_bboB_t*)(RcRegistry.pFunctionType[p->tid].Array);
    uint8_t a;
    size_t asiz = sizeof(a);
    uint8_t b;
    size_t bsiz = sizeof(b);
    uint8_t value;
    int result;
    
    // de-serialize
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &asiz, &a ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == asiz );
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &bsiz, &b ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == bsiz );
    enableRcRxServer = 1;

    // execute
    result = (*array[p->fid])( a, b, &value );

    if( RC_ANSWER_FLAG & p->pix ){ // if answer expected
        ENTER_CRITICAL_SECTION
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 2, p ) );
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(int), (uint8_t*)&result ) );
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(value), &value ) );
        LEAVE_CRITICAL_SECTION
    }

    UNUSED_PARAMETER( v );
    return RESULT_OK;
}


/*! client side handler for functions of type
\code
int32_t pFn( uint8_t byte0, uint8_t byte1 );
\endcode
These functions exspect 2 bytes and return an int32_t as result.
\param p pointer to remote interface
\param argp not used on server side
\retval RESULT_OK success
*/
int32_t w_bb_Client( RcRemote_t* p, va_list argp ){
    // get args from arg list
    int32_t* pResult = va_arg( argp, int32_t* ); // address where to put result of serverfunction itself
    uint8_t a = va_arg( argp, int ); // arguments are promoted to int
    uint8_t b = va_arg( argp, int ); // arguments are promoted to int

    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrRequestHeader( 2, p ));
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof( uint8_t), &a ) );
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof( uint8_t), &b ) );

    // wait
    if( RC_ANSWER_FLAG & p->pix ){ // wait for matching answer now if an answer is expected
        size_t size = sizeof(pResult);
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == waitForRemoteAnswer( p, &rcAnswerReceived ));

        // de-serialize
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &size, (uint8_t*)pResult ) );
        // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof( int ) == size );
    }
    enableRcRxServer = 1;
    return RESULT_OK;
}


/*! server side handler for functions of type
\code
int32_t pFn( uint8_t byte0, uint8_t byte1 );
\endcode
These functions exspect 2 bytes and return an int32_t as result.
\param p pointer to received remote interface
\param v argp not used on server side
\retval RESULT_OK success
*/
int32_t w_bb_Server( RcRemote_t* p, va_list v){
    w_bb_t const * const array = (w_bb_t*)(RcRegistry.pFunctionType[p->tid].Array);
    uint8_t a;
    size_t asiz = sizeof(a);
    uint8_t b;
    size_t bsiz = sizeof(b);
    int result;
    
    // de-serialize
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &asiz, &a ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == asiz );
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &bsiz, &b ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == bsiz );
    enableRcRxServer = 1;

    // execute
    result = (*array[p->fid])( a, b );

    if( RC_ANSWER_FLAG & p->pix ){ // if answer expected
        ENTER_CRITICAL_SECTION
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 1, p ) );
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(int), (uint8_t*)&result ) );
        LEAVE_CRITICAL_SECTION
    }

    UNUSED_PARAMETER( v );
    return RESULT_OK;
}


/*! client side handler for functions of type
\code
int32_t pFn( uint8_t byte0, uint8_t byte1, uint8_t byte2 );
\endcode
These functions exspect 2 bytes and return an int32_t as result.
\param p pointer to remote interface
\param argp not used on server side
\retval RESULT_OK success
*/
int32_t w_bbb_Client( RcRemote_t* p, va_list argp ){
    // get args from arg list
    int32_t* pResult = va_arg( argp, void* ); // address where to put result of serverfunction itself
    uint8_t a = va_arg( argp, int ); // arguments are promoted to int
    uint8_t b = va_arg( argp, int ); // arguments are promoted to int
    uint8_t c = va_arg( argp, int ); // arguments are promoted to int
    ENTER_CRITICAL_SECTION
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrRequestHeader( 3, p ));
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof( uint8_t), &a ) );
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof( uint8_t), &b ) );
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof( uint8_t), &c ) );
    LEAVE_CRITICAL_SECTION
    // wait
    if( RC_ANSWER_FLAG & p->pix ){ // wait for matching answer now if an answer is expected
        size_t size = sizeof(pResult);
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == waitForRemoteAnswer( p, &rcAnswerReceived ));

        // de-serialize
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &size, (uint8_t*)pResult ) ); //lint !e530
        // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(int32_t) == size );
    }

    UNUSED_PARAMETER(argp);
    enableRcRxServer = 1;
    return RESULT_OK;
}


/*! server side handler for functions of type
\code
int32_t pFn( uint8_t byte0, uint8_t byte1, uint8_t byte2 );
\endcode
These functions exspect 2 bytes and return an int32_t as result.
\param p pointer to received remote interface
\param v argp not used on server side
\retval RESULT_OK success
*/
int32_t w_bbb_Server( RcRemote_t* p, va_list v){
    w_bbb_t const * const array = (w_bbb_t*)(RcRegistry.pFunctionType[p->tid].Array);
    uint8_t a, b, c;
    size_t asiz = sizeof(a), bsiz = sizeof(b), csiz = sizeof(c);
    int result;
    
    // de-serialize
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &asiz, &a ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == asiz );
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &bsiz, &b ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == bsiz );
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &csiz, &c ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == csiz );
    enableRcRxServer = 1;

    // execute
    result = (*array[p->fid])( a, b, c );

    if( RC_ANSWER_FLAG & p->pix ){ // if answer expected
        ENTER_CRITICAL_SECTION
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 1, p ) );
            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(int), (uint8_t*)&result ) );
        LEAVE_CRITICAL_SECTION
    }

    UNUSED_PARAMETER( v );
    return RESULT_OK;
}


/*! client side handler for functions of type
\code
void pFn( void );
\endcode
These functions exspect no parameteras and return void as result.
\param p pointer to remote interface
\param argp not used on server side
\retval RESULT_OK success
*/
int32_t v_v_Client( RcRemote_t* p, va_list argp ){
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrRequestHeader( 0, p ));

    // wait
    if( RC_ANSWER_FLAG & p->pix ){ // wait for matching answer now if an answer is expected
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == waitForRemoteAnswer( p, &rcAnswerReceived ));
    }

    UNUSED_PARAMETER( argp );
    enableRcRxServer = 1;
    return RESULT_OK;
}


/*! client side handler for functions of type
\code
void pFn( void );
\endcode
These functions exspect no parameteras and return void as result.
\param p pointer to receved remote interface
\param v argp not used on server side
\retval RESULT_OK success
*/
int32_t v_v_Server( RcRemote_t* p, va_list v){
    v_v_t const * const array = (v_v_t*)(RcRegistry.pFunctionType[p->tid].Array);
    enableRcRxServer = 1;

    // execute
    (*array[p->fid])();

    if( RC_ANSWER_FLAG & p->pix ){ // if answer expected
        ENTER_CRITICAL_SECTION
           ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 0, p ) );
        LEAVE_CRITICAL_SECTION
    }

    UNUSED_PARAMETER( v );
    return RESULT_OK;
}


/*! client side handler for functions of type
\code
void pFn( uint8_t id );
\endcode
These void functions have a byte parameter.
\param p pointer to remote interface
\param argp for variadic parameters
*/
int v_c_Client( RcRemote_t* p, va_list argp) {
    // get args from arg list
    uint8_t c = va_arg(argp, int); // arguments are promoted to int
    ENTER_CRITICAL_SECTION
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrRequestHeader( 1, p ));
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack(p, sizeof(uint8_t), &c ) );
    LEAVE_CRITICAL_SECTION

    // wait
    if( RC_ANSWER_FLAG & p->pix ){ // wait for matching answer now
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == waitForRemoteAnswer( p, &rcAnswerReceived ));
    }
    enableRcRxServer = 1;
    return RESULT_OK;
}


/*! server side handler for functions of type
\code
void pFn( uint8_t id );
\endcode
These void functions have a byte parameter. 
\param p pointer to remote interface
\param argp not used on server side
*/
int v_c_Server( RcRemote_t* p, va_list argp) {
    v_c_t const * const array = (v_c_t*)(RcRegistry.pFunctionType[p->tid].Array);
    uint8_t b;
    size_t bsiz = 1;
    
    // de-serialize
    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &bsiz, &b ) );
    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == bsiz );
    enableRcRxServer = 1;

    // execute
    (*array[p->fid])( b );

    if( RC_ANSWER_FLAG & p->pix ){ // if answer expected
        ENTER_CRITICAL_SECTION
        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 0, p ) );
        LEAVE_CRITICAL_SECTION
    }

    UNUSED_PARAMETER( argp );
    return RESULT_OK;
}


///*! client side handler for functions of type
//\code
//void pFn( id_t id, uint8_t count, uint8_t, ... );
//\endcode
//These void functions have as parameter a byte as count count the appropriate number of 8bit values.
//\param p pointer to remote interface
//\param argp for variadic parameters
//*/
//int v_ccC8_Client( RcRemote_t* p, va_list argp) {
//    // get args from arg list
//    TrId_t id    = va_arg(argp, int); // arguments are promoted to int
//    uint8_t count = va_arg(argp, int); // arguments are promoted to int
//    va_list ap = va_arg(argp, va_list); // address where to get data
//    uint8_t buff[8]; // not more than 8 8bit values expected
//    int i;
//    for( i = 0; i < count; i++ ){
//        buff[i] = va_arg(ap, int); // arguments are promoted to int
//    }
//    count *= sizeof(uint8_t);
//    ENTER_CRITICAL_SECTION
//    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrRequestHeader( 2, p ));
//    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(TrId_t), (uint8_t*)&id ) ); 
//    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, count, buff ) ); 
//    LEAVE_CRITICAL_SECTION
//
//    // wait
//    if( RC_ANSWER_FLAG & p->pix ){ // wait for matching answer now if an answer is expected
//        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == waitForRemoteAnswer( p, &rcAnswerReceived ));
//    }
//    enableRcRxServer = 1;
//    return RESULT_OK;
//}
//
//
///*! server side handler for functions of type
//\code
//void pFn( id_t id, uint8_t count, uint8_t, ... );
//\endcode
//These void functions have as parameter a byte as count count the appropriate number of 8bit values.
//\param p pointer to remote interface
//\param argp not used on server side
//*/
//int v_ccC8_Server( RcRemote_t* p, va_list argp) {
//    v_ccC8_t const * const array = (v_ccC8_t*)(RcRegistry.pFunctionType[p->tid].Array);
//    uint8_t id;
//    size_t idSiz = sizeof(id);
//    #define maxCount 8 // const uint8_t maxCount = 8;
//    uint8_t B[maxCount];
//    size_t Bsiz = sizeof(B);
//
//    // de-serialize
//    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &idSiz, &id ) );
//    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == idSiz );
//    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &Bsiz, B ) );
//    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( Bsiz <= sizeof(B) );
//    enableRcRxServer = 1;
//
//    // execute
//    if( 1 == Bsiz ){
//        (*array[p->fid])( id, Bsiz, B[0] );
//    } else if( 2 == Bsiz ){
//        (*array[p->fid])( id, Bsiz, B[0], B[1] );
//    } else if( 3 == Bsiz ){
//        (*array[p->fid])( id, Bsiz, B[0], B[1], B[2] );
//    } else if( 4 == Bsiz ){
//        (*array[p->fid])( id, Bsiz, B[0], B[1], B[2], B[3] );
//    } else if( 5 == Bsiz ){
//        (*array[p->fid])( id, Bsiz, B[0], B[1], B[2], B[3], B[4] );
//    } else if( 6 == Bsiz ){
//        (*array[p->fid])( id, Bsiz, B[0], B[1], B[2], B[3], B[4], B[5] );
//    } else if( 7 == Bsiz ){
//        (*array[p->fid])( id, Bsiz, B[0], B[1], B[2], B[3], B[4], B[5], B[6] );
//    } else if( 8 == Bsiz ){
//        (*array[p->fid])( id, Bsiz, B[0], B[1], B[2], B[3], B[4], B[5], B[6], B[7] );
//    } else {
//        return RESULT_ERROR;
//    }
//    
//    if( RC_ANSWER_FLAG & p->pix ){ // if answer expected
//        ENTER_CRITICAL_SECTION
//            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 0, p ) );
//        LEAVE_CRITICAL_SECTION
//    }
//
//    UNUSED_PARAMETER( argp );
//    return RESULT_OK;
//}
//
//
///*! client side handler for functions of type
//\code
//void pFn( id_t id, uint8_t count, uint32_t, ... );
//\endcode
//These void functions contain after count these number of 32 bit values.
//\param p pointer to remote interface
//\param argp for variadic parameters
//*/
//int v_ccC16_Client( RcRemote_t* p, va_list argp) {
//    // get args from arg list
//    TrId_t id     = va_arg(argp, int); // arguments are promoted to int
//    uint8_t count = va_arg(argp, int); // arguments are promoted to int
//    va_list ap = va_arg(argp, va_list); // address where to get data
//    uint16_t buff[8]; // not more than 8 8bit values expected
//    int i;
//    for( i = 0; i < count; i++ ){
//        buff[i] = va_arg(ap, int); // arguments are promoted to int
//    }
//    count *= sizeof(uint16_t);
//    ENTER_CRITICAL_SECTION
//        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrRequestHeader( 2, p ));
//        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(TrId_t), (uint8_t*)&id ) );       // 0xda 0x00 0x00 id    crc8
//        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, count, (uint8_t*)buff ) );  // 0xda 0x00 count-1 ... crc8
//    LEAVE_CRITICAL_SECTION
//
//    // wait
//    if( RC_ANSWER_FLAG & p->pix ){ // wait for matching answer now if an answer is expected
//        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == waitForRemoteAnswer( p, &rcAnswerReceived ));
//    }
//    enableRcRxServer = 1;
//    return RESULT_OK;
//}
//
//
///*! server side handler for functions of type
//\code
//void pFn( id_t id, uint8_t count, uint32_t, ... );
//\endcode
//These functions ecpect in a buffer count bytes and returns void
//\param p pointer to remote interface
//\param argp not used on server side
//*/
//int v_ccC16_Server( RcRemote_t* p, va_list argp) {
//    v_ccC16_t const * const array = (v_ccC16_t*)(RcRegistry.pFunctionType[p->tid].Array);
//    uint8_t id;
//    size_t idSiz = sizeof(id);
//    #define maxCount 8 //const uint8_t maxCount = 8;
//    uint16_t B[maxCount];
//    size_t Bsiz = sizeof(B);
//    size_t Bcnt; // count of elements in B
//
//    // de-serialize 
//    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &idSiz, &id ) );
//    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == idSiz );
//
//    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &Bsiz, (uint8_t*)B ) );
//    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( Bsiz <= sizeof(B) );
//    enableRcRxServer = 1;
//    Bcnt = Bsiz >> 1;
//
//    // execute
//    if( 1 == Bcnt ){
//        (*array[p->fid])( id, Bcnt, B[0] );
//    } else if( 2 == Bcnt){
//        (*array[p->fid])( id, Bcnt, B[0], B[1] );
//    } else if( 3 == Bcnt){
//        (*array[p->fid])( id, Bcnt, B[0], B[1], B[2] );
//    } else if( 4 == Bcnt){
//        (*array[p->fid])( id, Bcnt, B[0], B[1], B[2], B[3] );
//    } else {
//        return RESULT_ERROR;
//    }
//    
//    if( RC_ANSWER_FLAG & p->pix ){ // if answer expected
//        ENTER_CRITICAL_SECTION
//            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 0, p ) );
//        LEAVE_CRITICAL_SECTION
//    }
//
//    UNUSED_PARAMETER( argp );
//    return RESULT_OK;
//}
//
//
///*! client side handler for functions of type
//\code
//void pFn( id_t id, uint8_t count, uint32_t, ... );
//\endcode
//These void functions contain after count these number of 32 bit values.
//\param p pointer to remote interface
//\param argp for variadic parameters
//*/
//int v_ccC32_Client( RcRemote_t* p, va_list argp) {
//    // get args from arg list
//    TrId_t id    = va_arg(argp, int); // arguments are promoted to int
//    uint8_t count = va_arg(argp, int); // arguments are promoted to int
//    va_list ap = va_arg(argp, va_list); // address where to get data
//    uint32_t buff[8]; // not more than 8 32 bit values expected
//    int i;
//    for( i = 0; i < count; i++ ){
//        buff[i] = va_arg(ap, int); // arguments are promoted to int
//    }
//    count *= sizeof(uint32_t);
//    ENTER_CRITICAL_SECTION
//        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrRequestHeader( 2, p ));
//        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, sizeof(TrId_t), (uint8_t*)&id ) );       // 0xda 0x00 0x00 id    crc8
//        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == pack( p, count, (uint8_t*)buff ) );   // 0xda 0x00 count-1 ... crc8
//    LEAVE_CRITICAL_SECTION
//
//    // wait
//    if( RC_ANSWER_FLAG & p->pix ){ // wait for matching answer now if an answer is expected
//        ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == waitForRemoteAnswer( p, &rcAnswerReceived ));
//    }
//    enableRcRxServer = 1;
//    return RESULT_OK;
//}
//
//
///*! server side handler for functions of type
//\code
//void pFn( id_t id, uint8_t count, uint32_t, ... );
//\endcode
//These functions ecpect in a buffer count bytes and returns void
//\param p pointer to remote interface
//\param argp not used on server side
//*/
//int v_ccC32_Server( RcRemote_t* p, va_list argp) {
//    v_ccC32_t const * const array = (v_ccC32_t*)(RcRegistry.pFunctionType[p->tid].Array);
//    uint8_t id;
//    size_t idSiz = sizeof(id);
//    #define maxCount 8 //const uint8_t maxCount = 8;
//    uint32_t B[maxCount];
//    size_t Bsiz = sizeof(B);
//    size_t Bcnt; // count of elements in B
//
//    // de-serialize
//    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &idSiz, &id ) );
//    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( sizeof(uint8_t) == idSiz );
//
//    ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == unpack( p, &Bsiz, (uint8_t*)B ) );
//    // now done in unpack ASSERT_OR_RETURN_RESULT_ERROR( Bsiz <= sizeof(B) );
//    enableRcRxServer = 1;
//    Bcnt = Bsiz >> 2;
//
//    // execute
//    if( 1 == Bcnt ){
//        (*array[p->fid])( id, Bcnt, B[0] );
//    } else if( 2 == Bcnt){
//        (*array[p->fid])( id, Bcnt, B[0], B[1] );
//    } else {
//        return RESULT_ERROR;
//    }
//
//    if( RC_ANSWER_FLAG & p->pix ){ // if answer expected
//        ENTER_CRITICAL_SECTION
//            ASSERT_OR_RETURN_RESULT_ERROR( RESULT_OK == rcWrAnswerHeader( 0, p ) );
//        LEAVE_CRITICAL_SECTION
//    }
//
//    UNUSED_PARAMETER( argp );
//    return RESULT_OK;
//}


//
//
////////////////////////////////////////////// type handler //////////////////////////////////////////////




//#if 0 idea for routing
///*! Each sad is reached thru a predefined direction (left or right).
//In timeout case the direction is reversed in this array.
//*/
//static Fifo_t* sadDirection [] = {
//    &wrFifoL, // RC_ADDR(0)
//    &wrFifoL, // RC_ADDR(1)
//    &wrFifoL, // RC_ADDR(2)
//    &wrFifoL, // RC_ADDR(3)
//    &wrFifoL, // RC_ADDR(4)
//    &wrFifoL, // RC_ADDR(5)
//};
//
//
///*! get write fifo for a remote call to sad
//\param sad server address
//\return pointer to write fifo struct to reach sad
//*/
//static Fifo_t* rcWriteFifo( uint8_t sad ){
//    return sadDirection[sad - RC_ADDR_OFFSET];
//}
//
//
//static const unsigned sadLimit = ( sizeof( sadDirection ) / sizeof ( void*) ); //!< sad-RC_ADDR_OFFSET of a remote call must be smaller
//
///*! Change direction to reach sad.
//\param sad server address
//*/
//static void alternateSadDirection( uint8_t sad ){
//    sad -= RC_ADDR_OFFSET;
//    sadDirection[sad] = ( &wrFifoL == sadDirection[sad] ) ? &wrFifoR : &wrFifoL;
//}
//#endif
