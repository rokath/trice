/*! \file triceCheck.c
\brief trices for tool evaluation
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include <float.h>
//#define TRICE_OFF // enable this line to disable trice code generation in this file object
#include "trice.h"
#define TRICE_FILE Id(52023) 

int32_t FloatToInt32( float f ){
	  if( f >= 0 ){
			  return (int32_t)f;
		}
		return -(int32_t)-f;
}

int64_t DoubleToInt64( double f ){
	  if( f >= 0 ){
			  return (int64_t)f;
		}
		return -(int64_t)-f;
}

//! TriceCheckSet writes out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
void TriceCheckSet(int index) {
    char* s;
		float  x = 1089.6082763671875; // 0x44883377
		double y = 518.0547492508867; // 0x4080307020601050
    switch (index) {
        case 0:                    
            TRICE( Id(59034), "att:Various single arguments\n" );
            TRICE8( Id(64006), "rd:TRICE8 line %t (%%t ,0)\n", 0 ); 
            TRICE8( Id(45005), "rd:TRICE8 line %t (%%t ,2)\n", 2 ); 
            TRICE8( Id(36514), "rd:TRICE8 line %u (%%u ,-1)\n", -1 ); 
            TRICE8( Id(61927), "rd:TRICE8 line %b (%%b ,-1)\n", -1 ); 
            TRICE8( Id(36474), "rd:TRICE8 line %o (%%o ,-1)\n", -1 ); 
            TRICE8( Id(34551), "rd:TRICE8 line %O (%%O ,-1)\n", -1 ); 
            TRICE8( Id(55192), "rd:TRICE8 line %X (%%X ,-1)\n", -1 ); 
            TRICE8( Id(50701), "rd:TRICE8 line %x (%%x ,-1)\n", -1 ); 
            TRICE8( Id(44661), "rd:TRICE8 line %d (%%d ,-1)\n", -1 ); 
            TRICE16_1( Id(46787), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 ); 
            TRICE16_1( Id(56721), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 ); 
            TRICE16_1( Id( 35169), "rd:TRICE16_1 line %u (%%u -1)\n", -1 ); 
            TRICE16_1( Id( 35133), "rd:TRICE16_1 line %b (%%b -1)\n", -1 ); 
            TRICE16_1( Id( 51463), "rd:TRICE16_1 line %o (%%o -1)\n", -1 ); 
            TRICE16_1( Id( 53632), "rd:TRICE16_1 line %x (%%x -1)\n", -1 ); 
            TRICE16_1( Id( 37985), "rd:TRICE16_1 line %d (%%d -1)\n", -1 ); 
            TRICE( Id(45626), "rd:TRICE line %t (%%t -1)\n", 0 ); 
            TRICE( Id(65238), "rd:TRICE line %t (%%t -1)\n", 2 ); 
            TRICE( Id(48024), "rd:TRICE line %u (%%u)\n", -1 ); 
            TRICE( Id(58374), "rd:TRICE line %b (%%b)\n", -2 ); 
            TRICE( Id(45055), "rd:TRICE line %o (%%o)\n", -3 ); 
            TRICE( Id(37423), "rd:TRICE line %x (%%x)\n", -4 ); 
            TRICE( Id(53816), "rd:TRICE line %d (%%d)\n", -5 ); 
            TRICE( Id(33703), "rd:TRICE line %E (%%E)\n", aFloat(-555555555.5555555555) ); 
            TRICE( Id(55209), "rd:TRICE line %F (%%F)\n", aFloat(-555555555.5555555555) ); 
            TRICE( Id(59991), "rd:TRICE line %G (%%G)\n", aFloat(-555555555.5555555555) ); 
            TRICE32_1( Id(62543), "rd:TRICE32_1 line %u (%%u)\n", -1 ); 
            TRICE32_1( Id(45989), "rd:TRICE32_1 line %b (%%b)\n", -1 ); 
            TRICE32_1( Id(36491), "rd:TRICE32_1 line %o (%%o)\n", -1 ); 
            TRICE32_1( Id(42294), "rd:TRICE32_1 line %O (%%O)\n", -1 ); 
            TRICE32_1( Id(47959), "rd:TRICE32_1 line %X (%%X)\n", -1 ); 
            TRICE32_1( Id(35192), "rd:TRICE32_1 line %x (%%x)\n", -1 ); 
            TRICE32_1( Id(58755), "rd:TRICE32_1 line %d (%%d)\n", -1 ); 
            TRICE64_1( Id(50978), "rd:TRICE64_1 line %u (%%u)\n", -1 ); 
            TRICE64_1( Id(35922), "rd:TRICE64_1 line %b (%%b)\n", -1 ); 
            TRICE64_1( Id(36296), "rd:TRICE64_1 line %o (%%o)\n", -1 ); 
            TRICE64_1( Id(42767), "rd:TRICE64_1 line %O (%%O)\n", -1 ); 
            TRICE64_1( Id(49735), "rd:TRICE64_1 line %X (%%C)\n", -1 ); 
            TRICE64_1( Id(42819), "rd:TRICE64_1 line %x (%%x)\n", -1 ); 
            TRICE64_1( Id(52591), "rd:TRICE64_1 line %d (%%d)\n", -1 ); 
            TRICE64( Id(41643), "rd:TRICE64 line %E (%%E)\n", aDouble(-555555555.5555555555) ); 
            TRICE64( Id(58794), "rd:TRICE64 line %F (%%F)\n", aDouble(-555555555.5555555555) ); 
            TRICE64( Id(53221), "rd:TRICE64 line %G (%%G)\n", aDouble(-555555555.5555555555) ); 
        break;
        case 1:
            TRICE( Id(37345), "sig:Some time measurements\n" );
            TRICE( Id(62870), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(48522), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(49831), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(64348), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(58441), "tim:TRICE START time message\n");
            TRICE( Id(57298), "tim:TRICE STOP time message\n");
            TRICE( Id(58441), "tim:TRICE START time message\n");
            TRICE( Id(57298), "tim:TRICE STOP time message\n");
            TRICE( Id(58441), "tim:TRICE START time message\n");
            TRICE64( Id( 52183), "rd:TRICE64 %d, %d\n", 1, 2 );
            TRICE( Id(57298), "tim:TRICE STOP time message\n");
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE64( Id( 52183), "rd:TRICE64 %d, %d\n", 1, 2 );
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE64( Id( 52183), "rd:TRICE64 %d, %d\n", 1, 2 );
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(59836), "tim:TRICE --------------------------------------------------\n"); 
            TRICE( Id(40965),  "rd:TRICE32_1 line %d\n", __LINE__ );
            TRICE64_1( Id( 51133),  "rd:TRICE64_1 %d\n", __LINE__ );
            TRICE32_2( Id( 55925),  "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
            TRICE64_2( Id( 36492),  "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );
        break;
				case 2:
            TRICE( Id(55538), "sig:TRICE with 1 to 12 values\n" );					
            TRICE( Id(59739),"rd:TRICE %d\n", -1 );
            TRICE( Id(55403),"rd:TRICE %d, %d\n", -1, -2 );
            TRICE( Id(43921),"rd:TRICE %d, %d, %d\n", -1, -2, -3 );
            TRICE( Id(57336),"rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE( Id(55041),"rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE( Id(39495),"rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE( Id(59569),"rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE( Id(54807),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );      
            TRICE( Id(60151),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );      
            TRICE( Id(44906),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);      
            TRICE( Id(64671),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );      
            TRICE( Id(62612),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); 

						TRICE( Id(42203), "sig:TRICE_n with 1 to 12 values\n" );
            TRICE_1( Id(46143),   "rd:TRICE_1 %d\n", 1 );
            TRICE_2( Id(44383),   "rd:TRICE_2 %d, %d\n", 1, 2 );
            TRICE_3( Id(46151),   "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE_4( Id(37546),   "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE_5( Id(62341),   "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE_6( Id(51647),   "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE_7( Id(64373),   "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE_8( Id(35605),   "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            TRICE_9( Id(36627),   "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );      
            TRICE_10( Id(34716),  "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);      
            TRICE_11( Id(53794),  "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );      
            TRICE_12( Id(42708),  "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );    				
				break;
        case 3:
            TRICE( Id(49302), "sig:TRICE8 with 1 to 12 values\n" );					
            TRICE8( Id( 63495),"rd:TRICE8 %d\n", -1 );
            TRICE8( Id( 40220),"rd:TRICE8 %d, %d\n", -1, -2 );
            TRICE8( Id( 56315),"rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
            TRICE8( Id( 35199),"rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE8( Id( 64677),"rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE8( Id( 63697),"rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8( Id( 62807),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE8( Id( 59840),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );      
            TRICE8( Id( 40569),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );      
            TRICE8( Id( 45470),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);      
            TRICE8( Id( 38100),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );      
            TRICE8( Id( 39583),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
				
						TRICE( Id(40643), "sig:TRICE8_n with 1 to 12 values\n" );
            TRICE8_1( Id( 46820),   "rd:TRICE8_1 %d\n", 1 );
            TRICE8_2( Id( 56991),   "rd:TRICE8_2 %d, %d\n", 1, 2 );
            TRICE8_3( Id( 40111),   "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE8_4( Id( 42772),   "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE8_5( Id( 34440),   "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE8_6( Id( 60589),   "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE8_7( Id( 43242),   "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8( Id( 49170),   "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            TRICE8_9( Id( 55550),   "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );      
            TRICE8_10( Id( 63824),  "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);      
            TRICE8_11( Id( 61700),  "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );      
            TRICE8_12( Id( 61564),  "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );      
        break;
        case 4:				
            TRICE( Id(37891), "sig:TRICE16 with 1 to 12 values\n" );					
            TRICE16( Id(57058),"rd:TRICE16 %d\n", -1 );
            TRICE16( Id(37371),"rd:TRICE16 %d, %d\n", -1, -2 );
            TRICE16( Id(43191),"rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
            TRICE16( Id(65295),"rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE16( Id(49499),"rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE16( Id(44336),"rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id(59873),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id(54204),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );      
            TRICE16( Id(44576),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );      
            TRICE16( Id(61187),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);      
            TRICE16( Id(36155),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );      
            TRICE16( Id(33882),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

						TRICE( Id(62183), "sig:TRICE16_n with 1 to 12 values\n" );
            TRICE16_1( Id( 46719),  "rd:TRICE16_1 %d\n", 1 );
            TRICE16_2( Id( 38144),  "rd:TRICE16_2 %d, %d\n", 1, 2 );
            TRICE16_3( Id( 64454),  "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE16_4( Id( 44382),  "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE16_5( Id( 63486),  "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE16_6( Id( 55607),  "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE16_7( Id( 55482),  "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE16_8( Id( 59674),  "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            TRICE16_9( Id( 50852),  "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );      
            TRICE16_10( Id( 45071), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);      
            TRICE16_11( Id( 54163), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );      
            TRICE16_12( Id( 49865), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
						
						TRICE( Id(51949), "sig:TRICE16 with 1 to 12 pointer\n" );	
            TRICE16( Id(52595),"rd:TRICE16 %p\n", -1 );
            TRICE16( Id(61211),"rd:TRICE16 %p, %p\n", -1, -2 );
            TRICE16( Id(33852),"rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
            TRICE16( Id(56495),"rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
            TRICE16( Id(33551),"rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
            TRICE16( Id(40988),"rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id(54862),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id(36008),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );      
            TRICE16( Id(36845),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );      
            TRICE16( Id(33989),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);      
            TRICE16( Id(52211),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );      
            TRICE16( Id(57429),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
						
						TRICE( Id(36764), "sig:TRICE16 with 1 to 12 hex\n" );	
            TRICE16( Id(64409),"rd:TRICE16 %X\n", -1 );
            TRICE16( Id(61352),"rd:TRICE16 %X, %X\n", -1, -2 );
            TRICE16( Id(47324),"rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
            TRICE16( Id(57017),"rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
            TRICE16( Id(57775),"rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
            TRICE16( Id(39967),"rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id(59096),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id(43200),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );      
            TRICE16( Id(58574),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );      
            TRICE16( Id(65180),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);      
            TRICE16( Id(47916),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );      
            TRICE16( Id(38229),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
						
        break;
        case 5:
            TRICE( Id(50008), "sig:TRICE32 with 1 to 12 values\n" );					
            TRICE32( Id(53927),"rd:TRICE32 %d\n", -1 );
            TRICE32( Id(46310),"rd:TRICE32 %d, %d\n", -1, -2 );
            TRICE32( Id(65178),"rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
            TRICE32( Id(50671),"rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE32( Id(60995),"rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE32( Id(57960),"rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32( Id(43557),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE32( Id(43229),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );      
            TRICE32( Id(55633),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );      
            TRICE32( Id(39620),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);      
            TRICE32( Id(46387),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );      
            TRICE32( Id(37649),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );      

						TRICE( Id(63319), "signal:TRICE32_n with 1 to 12 values\n" );
            TRICE32_1( Id( 36476),  "rd:TRICE32_1 %d\n", 1 );
            TRICE32_2( Id( 49557),  "rd:TRICE32_2 %d, %d\n", 1, 2 );
            TRICE32_3( Id( 64262),  "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE32_4( Id( 34114),  "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE32_5( Id( 55498),  "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE32_6( Id( 38760),  "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE32_7( Id( 56936),  "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE32_8( Id( 62294),  "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            TRICE32_9( Id( 60777),  "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );      
            TRICE32_10( Id( 62433), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);      
            TRICE32_11( Id( 50975), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );      
            TRICE32_12( Id( 51177), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );      
        break;
        case 6:                    
            TRICE( Id(60996), "sig:TRICE64 with 1 to 12 values\n" );					
            TRICE64( Id(62366),"rd:TRICE64 %d\n", -1 );
            TRICE64( Id(63090),"rd:TRICE64 %d, %d\n", -1, -2 );
            TRICE64( Id(49173),"rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
            TRICE64( Id(39833),"rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE64( Id(35858),"rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE64( Id(64921),"rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64( Id(64535),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE64( Id(56000),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );      
            TRICE64( Id(45722),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );      
            TRICE64( Id(38016),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);      
            TRICE64( Id(38575),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );      
            TRICE64( Id(41695),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );      

				    TRICE( Id(45085), "sig:TRICE64_n with 1 to 12 values\n" );
				    TRICE64_1( Id( 52898),  "rd:TRICE64_1 %d\n", 1 );
            TRICE64_2( Id( 60756),  "rd:TRICE64_2 %d, %d\n", 1, 2 );
            TRICE64_3( Id( 35346),  "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE64_4( Id( 61021),  "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE64_5( Id( 50194),  "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE64_6( Id( 46977),  "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE64_7( Id( 32919),  "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE64_8( Id( 50184),  "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            TRICE64_9( Id( 45499),  "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );      
            TRICE64_10( Id( 41642), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);      
            TRICE64_11( Id( 43327), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );      
            TRICE64_12( Id( 35845), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );      
        break;
        case 7:
					  TRICE( Id(52600), "sig:Colors and ticks\n" );
            TRICE(Id(60496), "--------------------------------------------------\n");
            TRICE(Id(42876), "--------------------------------------------------\n");
            TRICE(Id(51389), "dbg:12345 as 16bit is %016b\n", 12345);
            TRICE(Id(57315), "--------------------------------------------------\n");
            TRICE(Id(57781), "sig:This ASSERT error is just a demo and no real error:\n");
            TRICE(Id(65278), "--------------------------------------------------\n");
            TRICE(Id(54995), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(62491), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(46970), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(43737), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(33334), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(49309), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(42185), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(52419), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(60387), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(46494), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(40525), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(52028), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
           
            TRICE(Id(37222), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
        break;
        case 8:
					  TRICE( Id(42158), "sig:Various ranges and formats\n"	);				
            TRICE8( Id(58504), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(34672), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(35756), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(48003), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(35768), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(64932), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(56876), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(64852), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(43671), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);

    				TRICE16(Id(56581), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16(Id(49438), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
            TRICE16(Id(52004), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
				
            TRICE32(Id(56112), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
            TRICE32(Id(64512), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
				    TRICE32(Id(45690), "att:TRICE32 %40b\n", 0xAAAAAAAA);
				
            TRICE64(Id(60605), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
            TRICE64(Id(60231), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
				    TRICE64(Id(53220), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break;
        case 9:
            TRICE( Id(53548), "sig:Legacy TRICE8\n" );
            TRICE8_1(Id( 52582), "tst:TRICE8_1 %02x\n", 0xA1);
            TRICE8_2(Id( 58020), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
            TRICE8_3(Id( 40122), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
            TRICE8_4(Id( 50551), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
            TRICE8_5(Id( 33857), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
            TRICE8_6(Id( 55004), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
            TRICE8_7(Id( 49025), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
            TRICE8_8(Id( 53622), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
				
            TRICE8_1(Id( 55503), "tst:TRICE8_1 %d\n",                      -1);
            TRICE8_2(Id( 64874), "tst:TRICE8_2 %d %d\n",                   -1, -2);
            TRICE8_3(Id( 49107), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
            TRICE8_4(Id( 34673), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
            TRICE8_5(Id( 59637), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
            TRICE8_6(Id( 60537), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
            TRICE8_7(Id( 45261), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
            TRICE8_8(Id( 63789), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
				
            TRICE8_1(Id( 54804), "tst:TRICE8_1 %d\n", 1);
            TRICE8_2(Id( 42494), "tst:TRICE8_2 %d %d\n", 1, 2);
            TRICE8_3(Id( 44072), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
            TRICE8_4(Id( 51249), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
            TRICE8_5(Id( 56554), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            TRICE8_6(Id( 37866), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            TRICE8_7(Id( 35663), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8(Id( 53316), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
						
            TRICE8_1(Id( 59290), "tst:TRICE8_1 %u\n", 201);
            TRICE8_2(Id( 53955), "tst:TRICE8_2 %u %u\n", 201, 202);
            TRICE8_3(Id( 33982), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
            TRICE8_4(Id( 64087), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
            TRICE8_5(Id( 56344), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
            TRICE8_6(Id( 64958), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
            TRICE8_7(Id( 37787), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
            TRICE8_8(Id( 44560), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
						
            TRICE8_8(Id( 39887), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
				break;
        case 10:
					  TRICE( Id(37942), "sig:Legacy TRICE16-64\n" );
            TRICE16_1(Id( 35206), "tst:TRICE16_1 %d\n", -111);
            TRICE16_2(Id( 45725), "tst:TRICE16_2 %d %d\n", -111, -222);
            TRICE16_3(Id( 63897), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
            TRICE16_4(Id( 55371), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
            TRICE32_1(Id( 58781), "tst:TRICE32_1 %08x\n", 0x0123cafe);
            TRICE32_1(Id( 54389), "tst:TRICE32_1 %d\n", -111);
            TRICE32_2(Id( 59795), "tst:TRICE32_2 %x %x\n", -111, -222);
            TRICE32_2(Id( 47183), "tst:TRICE32_2 %d %d\n", -111, -222);
            TRICE32_3(Id( 39898), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
            TRICE32_3(Id( 40901), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
            TRICE32_4(Id( 58747), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
            TRICE32_4(Id( 59716), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
            TRICE64_1(Id( 59670), "tst:TRICE64_1 %d\n", -111);
            TRICE64_2(Id( 46159), "tst:TRICE64_2 %d %d\n", -111, -222);
				    TRICE16_1(Id( 57408), "tst:TRICE16_1 %u\n", 60001);
            TRICE16_2(Id( 64554), "tst:TRICE16_2 %u %u\n", 60001, 60002);
            TRICE16_3(Id( 35559), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
            TRICE16_4(Id( 53522), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
            TRICE32_1(Id( 53612), "tst:TRICE32_1 %u\n", 4000000001);
            TRICE32_2(Id( 64117), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
            TRICE32_3(Id( 46431), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
            TRICE32_4(Id( 50279), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
            TRICE64_1(Id( 59485), "tst:TRICE64_1 %x\n", -1);
            TRICE64_2(Id( 63675), "tst:TRICE64_2 %x %x\n", -1, -2);
            TRICE64_1(Id( 49602), "tst:TRICE64_1 %u\n", -1);
            TRICE64_2(Id( 39532), "tst:TRICE64_2 %u %u\n", -1, -2);
        break;
        case 11:
					  TRICE( Id(60930), "sig:colored single letters and several TRICE macros in one line\n" );    
            TRICE(Id(63529), "e:A");
            TRICE(Id(45283), "w:B");
            TRICE(Id(57374), "a:c");
            TRICE(Id(58579), "wr:d");
            TRICE(Id(38080), "rd:e\n");
            TRICE(Id(39747), "diag:f");
            TRICE(Id(47386), "d:G");
            TRICE(Id(39124), "t:H");
            TRICE(Id(42786), "time:i");
            TRICE(Id(34322), "message:J");
            TRICE(Id(44757), "dbg:k\n");
            TRICE(Id(39634), "1");
            TRICE(Id(39930), "2");
            TRICE(Id(62444), "3");
            TRICE(Id(39908), "4");
            TRICE(Id(40865), "e:7");
            TRICE(Id(38389), "m:12");
            TRICE(Id(41936), "m:123\n");
            TRICE(Id(63288), "e:A"); TRICE(Id(53934), "w:B"); TRICE(Id(44438), "a:c");
            TRICE(Id(39663), "wr:d"); TRICE(Id(42027), "rd:e\n"); //TRICE(Id(54760), "diag:f");
        break;
        case 12:
					  TRICE( Id(55452), "sig:Runtime generated strings\n" );   
            s = "AAAAAAAAAAAA";
            TRICE32( Id( 47643), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id( 43140), "sig:%s\n", s ); 
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
            TRICE32( Id( 48152), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id( 43140), "sig:%s\n", s );
        break;
        case 13:
					  TRICE( Id(34226), "sig:Runtime generated strings\n" );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
            TRICE32( Id( 60956), "dbg:len=%u: ", strlen(s) );
            TRICE_S( Id( 43140), "sig:%s\n", s );
        break;
        case 14:
					  TRICE( Id(58038), "sig:Runtime generated strings\n" );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
";
            TRICE32( Id( 60956), "dbg:len=%u: ", strlen(s) );
            TRICE_S( Id( 43140), "sig:%s\n", s );
        break;
        case 15:
					  TRICE( Id(60657), "sig:Runtime generated strings\n" );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
          TRICE32( Id( 60956), "dbg:len=%u: ", strlen(s) );  
          TRICE_S( Id( 43140), "sig:%s\n", s );
        break;
				case 16:{ // https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/
						int i = 15;
						TRICE( Id(61126), "sig:Integer (indent, base, sign)\n" );
						TRICE( Id(42661), "rd: 15 	%d 	Base 10\n", i );
						TRICE( Id(62209), "rd: +15 	%+d 	Always show sign\n", i );
						TRICE( Id(37289), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", i );
						TRICE( Id(46405), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", i );
						TRICE( Id(57753), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", i );
						TRICE( Id(36970), "rd: 1111 	%b 	Base 2\n", i );
						TRICE( Id(35229), "rd: 17 	%o 	Base 8\n", i );
						TRICE( Id(44824), "rd: f 	%x 	Base 16, lowercase\n", i );
						TRICE( Id(47950), "rd: F 	%X 	Base 16, uppercase\n", i );
						TRICE( Id(38989), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", i );					
				}{
						char a = 'A';
						TRICE( Id(62284), "sig:Character (quoted, Unicode)\n" );
						TRICE( Id(65019), "rd: A 	%c 	Character\n", a );
						TRICE( Id(37273), "rd: 'A' 	%q 	Quoted character\n", a );
					//TRICE( Id(53839), "rd: U+0041 	%U 	Unicode\n", a ); // todo
					//TRICE( Id(33809), "rd: U+0041 'A' 	%#U 	Unicode with character\n", a ); // todo
						TRICE( Id(56603), "sig:Boolean (true/false)\n" );
						TRICE( Id(33621), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 );
						TRICE( Id(42327), "sig:Pointer (hex)\n" );
						TRICE8( Id(60388), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );
						TRICE16( Id(34786), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );
						TRICE32( Id(52807), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );
						TRICE64( Id(65477), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 );
						TRICE( Id(34647), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);
				}
				break;
				case 17:{
						float f = 123.456;
						TRICE( Id(39810), "sig:Float (indent, precision, scientific notation)\n" );
						TRICE( Id(33264), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(f) );
						TRICE( Id(53763), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(f) );
						TRICE( Id(41255), "rd: 123.46	 		%.2f		%%.2f Default width, precision 2\n", aFloat(f) );
						TRICE( Id(57868), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(f) );
						TRICE( Id(33347), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(f) );					
				}{
						double d = 123.456;
						TRICE( Id(55556), "sig:Double (indent, precision, scientific notation)\n" );
						TRICE64( Id(38168), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(d) );
						TRICE64( Id(49909), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(d) );
						TRICE64( Id(55026), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(d) );
						TRICE64( Id(51914), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(d) );
						TRICE64( Id(46200), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(d) );					
				}{
						char* s = "café";
						TRICE( Id(61291), "sig:String or byte slice (quote, indent, hex)\n" );
						TRICE_S( Id(35811), "rd: café 			%s		Plain string\n", s );
						TRICE_S( Id(49621), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );
						TRICE_S( Id(34432), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );
						TRICE_S( Id(37252), "rd: \"café\" 		%q	 	Quoted string\n", s );
						TRICE_S( Id(42861), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );
						TRICE_S( Id(54266), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );				
				}
				break;
				case 18:
						TRICE( Id(42971), "sig:TRICE32 with variable param count 1 to 12\n" );
            TRICE8(Id(51798), "tst:TRICE8 %d\n", -1 );
            TRICE8(Id(64111), "tst:TRICE8 %d %d\n", -1, -2 );
            TRICE8(Id(60307), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
            TRICE8(Id(34954), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8(Id(35057), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8(Id(57500), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8(Id(43074), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8(Id(48189), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
            TRICE8(Id(49394), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8(Id(62045), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE8(Id(61322), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8(Id(38798), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
						TRICE8(Id(47568), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
            TRICE8_1 (Id(33595), "tst:TRICE8_1  %d\n", -1 );
            TRICE8_2 (Id(51702), "tst:TRICE8_2  %d %d\n", -1, -2 );
            TRICE8_3 (Id(38514), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
            TRICE8_4 (Id(35075), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8_5 (Id(53312), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8_6 (Id(61056), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8_7 (Id(48867), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8_8 (Id(38249), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
            TRICE8_9 (Id(54744), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8_10(Id(36588), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE8_11(Id(35416), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8_12(Id(62757), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
						TRICE8  (Id(33649), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
						TRICE8_9(Id(40830), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
				break;
				case 19:
						TRICE( Id(44218), "sig:TRICE16 with variable param count 1 to 12\n" );
            TRICE16(Id(41097), "tst:TRICE16 %d\n", -1 );
            TRICE16(Id(53986), "tst:TRICE16 %d %d\n", -1, -2 );
            TRICE16(Id(53261), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
            TRICE16(Id(53192), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE16(Id(33517), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE16(Id(48502), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16(Id(40166), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE16(Id(43147), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
            TRICE16(Id(61280), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16(Id(50533), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE16(Id(40501), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16(Id(42841), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
						TRICE16(Id(56342), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
            TRICE16_1 (Id(38909), "tst:TRICE16_1  %d\n", -1 );
            TRICE16_2 (Id(57683), "tst:TRICE16_2  %d %d\n", -1, -2 );
            TRICE16_3 (Id(61529), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
            TRICE16_4 (Id(65232), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE16_5 (Id(62511), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE16_6 (Id(63882), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16_7 (Id(65195), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE16_8 (Id(59043), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
            TRICE16_9 (Id(43591), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16_10(Id(44251), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE16_11(Id(53872), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16_12(Id(36066), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
						TRICE16_9(Id(48121), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
				break;
				case 20:
						TRICE( Id(52002), "sig:TRICE32 with variable param count 1 to 12\n" );
            TRICE32(Id(56796), "tst:TRICE32 %d\n", -1 );
            TRICE32(Id(51979), "tst:TRICE32 %d %d\n", -1, -2 );
            TRICE32(Id(47271), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
            TRICE32(Id(58332), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE32(Id(65038), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE32(Id(39831), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32(Id(45933), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE32(Id(53478), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
            TRICE32(Id(52619), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32(Id(59625), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE32(Id(62841), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32(Id(49802), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
						TRICE32(Id(49002), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
            TRICE32_1 (Id(42633), "tst:TRICE32_1  %d\n", -1 );
            TRICE32_2 (Id(55780), "tst:TRICE32_2  %d %d\n", -1, -2 );
            TRICE32_3 (Id(60119), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
            TRICE32_4 (Id(45083), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE32_5 (Id(54005), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE32_6 (Id(61798), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32_7 (Id(64783), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE32_8 (Id(52811), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
            TRICE32_9 (Id(34075), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32_10(Id(55009), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE32_11(Id(59997), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32_12(Id(54670), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
						TRICE32   (Id(54164), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
						TRICE32_12(Id(36447), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
				break;
				case 21:
						TRICE( Id(56014), "sig:TRICE64 with variable param count 1 to 12\n" );
            TRICE64(Id(36891), "tst:TRICE64 %d\n", -1 );
            TRICE64(Id(37083), "tst:TRICE64 %d %d\n", -1, -2 );
            TRICE64(Id(59897), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
            TRICE64(Id(40278), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE64(Id(55518), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE64(Id(44341), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64(Id(51400), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE64(Id(43825), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
            TRICE64(Id(55782), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64(Id(43757), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE64(Id(34168), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64(Id(43774), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
						TRICE64(Id(49511), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
            TRICE64_1 (Id(65009), "tst:TRICE64_1  %d\n", -1 );
            TRICE64_2 (Id(37510), "tst:TRICE64_2  %d %d\n", -1, -2 );
            TRICE64_3 (Id(53064), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
            TRICE64_4 (Id(47446), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE64_5 (Id(51628), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE64_6 (Id(43988), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64_7 (Id(57819), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE64_8 (Id(52990), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
            TRICE64_9 (Id(33451), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64_10(Id(55341), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE64_11(Id(33951), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64_12(Id(58482), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
						TRICE64_12(Id(52778), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
				break;
        case 22:
					  TRICE( Id(36707), "att:positive and negative float in format variants\n");
            TRICE( Id(59484), "rd:TRICE float %f (%%f)\n", aFloat(x) ); 
            TRICE( Id(65344), "rd:TRICE float %9f (%%9f)\n", aFloat(x) ); 
            TRICE( Id(52386), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) ); 
            TRICE( Id(59439), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) ); 
            TRICE( Id(46307), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) ); 
            TRICE( Id(55596), "rd:TRICE float %f (%%f)\n", aFloat(-x) ); 
            TRICE( Id(39188), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) ); 
            TRICE( Id(51674), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) ); 
            TRICE( Id(42236), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) ); 
            TRICE( Id(48894), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) ); 
            TRICE( Id(63262), "rd:TRICE float %+f (%%f)\n", aFloat(x) ); 
            TRICE( Id(60391), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) ); 
            TRICE( Id(62646), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) ); 
            TRICE( Id(34620), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) ); 
            TRICE( Id(60302), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) ); 
            TRICE( Id(58004), "rd:TRICE float %+f (%%f)\n", aFloat(-x) ); 
            TRICE( Id(36493), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) ); 
            TRICE( Id(63276), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) ); 
            TRICE( Id(33348), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) ); 
            TRICE( Id(63568), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) ); 
            TRICE( Id(40275), "rd:TRICE float %-f (%%f)\n", aFloat(x) ); 
            TRICE( Id(62221), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) ); 
            TRICE( Id(42622), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) ); 
            TRICE( Id(50004), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) ); 
            TRICE( Id(47343), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) ); 
            TRICE( Id(43292), "rd:TRICE float %-f (%%f)\n", aFloat(-x) ); 
            TRICE( Id(48242), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) ); 
            TRICE( Id(36843), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) ); 
            TRICE( Id(50552), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) ); 
            TRICE( Id(63022), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) ); 
				break;
        case 23:
			      TRICE( Id(40634), "att:positive float & double in variants\n");
            TRICE32_1( Id(36805), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) ); 
            TRICE32_1( Id(56606), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) ); 
            TRICE32_1( Id(32885), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) ); 
            TRICE32_1( Id(60583), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) ); 
            TRICE32_1( Id(53319), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) ); 
            TRICE32_1( Id(45161), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) ); 
				
            TRICE32( Id(41717), "rd:TRICE32 float %e (%%e)\n", aFloat(x) ); 
            TRICE32( Id(37819), "rd:TRICE32 float %f (%%f)\n", aFloat(x) ); 
            TRICE32( Id(44411), "rd:TRICE32 float %g (%%g)\n", aFloat(x) ); 
            TRICE32( Id(37291), "rd:TRICE32 float %E (%%E)\n", aFloat(x) ); 
            TRICE32( Id(34873), "rd:TRICE32 float %F (%%F)\n", aFloat(x) ); 
            TRICE32( Id(47575), "rd:TRICE32 float %G (%%G)\n", aFloat(x) ); 

            TRICE( Id(55265), "rd:TRICE float %e (%%e)\n", aFloat(x) );
            TRICE( Id(60347), "rd:TRICE float %f (%%f)\n", aFloat(x) );
            TRICE( Id(52958), "rd:TRICE float %g (%%g)\n", aFloat(x) );
            TRICE( Id(37737), "rd:TRICE float %E (%%E)\n", aFloat(x) );
            TRICE( Id(64326), "rd:TRICE float %F (%%F)\n", aFloat(x) );
            TRICE( Id(61656), "rd:TRICE float %G (%%G)\n", aFloat(x) );

    		    TRICE64( Id( 61570), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) ); 
            TRICE64( Id( 48898), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) ); 
            TRICE64( Id( 45981), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) ); 
            TRICE64( Id( 48603), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) ); 
            TRICE64( Id( 58939), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) ); 
            TRICE64( Id( 63708), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
						
    		    TRICE64_1( Id( 46163), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) ); 
            TRICE64_1( Id( 60501), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) ); 
            TRICE64_1( Id( 60361), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) ); 
            TRICE64_1( Id( 50015), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) ); 
            TRICE64_1( Id( 56849), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) ); 
            TRICE64_1( Id( 62936), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) ); 
        break;
        case 24:                                                         				
            TRICE( Id(45826), "att:negative float & double\n" );
				    TRICE( Id(55265), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
            TRICE( Id(60347), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
            TRICE( Id(52958), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
            TRICE( Id(37737), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
            TRICE( Id(64326), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
            TRICE( Id(61656), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
				
    		    TRICE64( Id( 56469), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) ); 
            TRICE64( Id( 64681), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) ); 
            TRICE64( Id( 46262), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) ); 
            TRICE64( Id( 62561), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) ); 
            TRICE64( Id( 56422), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) ); 
            TRICE64( Id( 59170), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) ); 
                                                        
            TRICE( Id(49994), "att:formatted float & double\n" );
    				TRICE32( Id( 41779), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) ); 
            TRICE32( Id( 57468), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) ); 
            TRICE32( Id( 45198), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) ); 
            TRICE32( Id( 51059), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) ); 
            TRICE32( Id( 48951), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) ); 
            TRICE32( Id( 44698), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
				
    		    TRICE64( Id( 49771), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) ); 
            TRICE64( Id( 49955), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) ); 
            TRICE64( Id( 52071), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) ); 
            TRICE64( Id( 64451), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) ); 
            TRICE64( Id( 63365), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) ); 
            TRICE64( Id( 56912), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) ); 
        break;
        case 25:                                                         
            TRICE( Id(51863), "att:mixed int & float & double & bits\n" );
            TRICE32( Id(51737), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(x), aFloat(x), aFloat(x), aFloat(x) ); 
            TRICE32( Id(51737), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(x), aFloat(x), aFloat(x), aFloat(x) ); 
            TRICE32( Id(51737), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              x,  aFloat(x), aFloat(x), aFloat(x) ); 
            TRICE32( Id(51737), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(-x), aFloat(-x), aFloat(-x), aFloat(-x) ); 
            TRICE32( Id(51737), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(-x), aFloat(-x), aFloat(-x), aFloat(-x) ); 
            TRICE32( Id(51737), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              -x,  aFloat(-x), aFloat(-x), aFloat(-x) ); 
            TRICE64( Id(61610), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(y), aDouble(y), aDouble(y), aDouble(y) ); 
            TRICE64( Id(61610), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(y), aDouble(y), aDouble(y), aDouble(y) ); 
            TRICE64( Id(61610), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               y,  aDouble(y), aDouble(y), aDouble(y) ); 
            TRICE64( Id(40084), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); 
            TRICE64( Id(40084), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); 
            TRICE64( Id(40084), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               -y,  aDouble(-y), aDouble(-y), aDouble(-y) ); 				
        
				#ifdef TRICE_ENCRYPT
        TRICE0 (Id( 48705), "--------------------------------------------------\n" );
        {
            uint32_t by[8] = {1,2,3,4,5,6};
            TRICE32(  Id( 34721), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            TRICE32( Id( 42011), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TriceEncrypt(by, 6);
            TRICE32( Id( 62571), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TRICE32(  Id( 50134), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            #ifdef TRICE_DECRYPT
            TRICE16_1( Id( 45804), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
            TriceDecrypt(by, 6);
            TRICE16_1( Id( 42353), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
            #endif
            TRICE32(  Id( 57611), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
        }
        TRICE0 (Id( 48705), "--------------------------------------------------\n" );
				#endif
        break;
				default:
				break;
    }
}
