/*! \file triceCheck.c
\brief trices for tool evaluation
\details The trices are dumped as 32bit values into a 32 bit wide fifo.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include <float.h>
//#define TRICE_OFF // enablae this line to disable trice code generation in this file object
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
				case 25:{ // https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/
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
				}
				break;
				case 26: {
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
				case 27: {
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
        case 29:
            TRICE32_1( Id(56606), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) ); 
            TRICE32_1( Id(57028), "rd:TRICE32_1 float %9f (%%9f)\n", aFloat(x) ); 
            TRICE32_1( Id(61734), "rd:TRICE32_1 float %.9f (%%.9f)\n", aFloat(x) ); 
            TRICE32_1( Id(44925), "rd:TRICE32_1 float %9.f (%%9.f)\n", aFloat(x) ); 
            TRICE32_1( Id(51264), "rd:TRICE32_1 float %9.6f (%%9.3f)\n", aFloat(x) ); 
            TRICE32_1( Id(56606), "rd:TRICE32_1 float %f (%%f)\n", aFloat(-x) ); 
            TRICE32_1( Id(57028), "rd:TRICE32_1 float %9f (%%9f)\n", aFloat(-x) ); 
            TRICE32_1( Id(61734), "rd:TRICE32_1 float %.9f (%%.9f)\n", aFloat(-x) ); 
            TRICE32_1( Id(35224), "rd:TRICE32_1 float %9.f (%%9.f)\n", aFloat(-x) ); 
            TRICE32_1( Id(51264), "rd:TRICE32_1 float %9.6f (%%9.3f)\n", aFloat(-x) ); 
            TRICE32_1( Id(55440), "rd:TRICE32_1 float %+f (%%f)\n", aFloat(x) ); 
            TRICE32_1( Id(54089), "rd:TRICE32_1 float %+9f (%%9f)\n", aFloat(x) ); 
            TRICE32_1( Id(44134), "rd:TRICE32_1 float %+.9f (%%.9f)\n", aFloat(x) ); 
            TRICE32_1( Id(56136), "rd:TRICE32_1 float %+9.f (%%9.f)\n", aFloat(x) ); 
            TRICE32_1( Id(36907), "rd:TRICE32_1 float %+9.6f (%%9.3f)\n", aFloat(x) ); 
            TRICE32_1( Id(55440), "rd:TRICE32_1 float %+f (%%f)\n", aFloat(-x) ); 
            TRICE32_1( Id(54089), "rd:TRICE32_1 float %+9f (%%9f)\n", aFloat(-x) ); 
            TRICE32_1( Id(44134), "rd:TRICE32_1 float %+.9f (%%.9f)\n", aFloat(-x) ); 
            TRICE32_1( Id(56136), "rd:TRICE32_1 float %+9.f (%%9.f)\n", aFloat(-x) ); 
            TRICE32_1( Id(36907), "rd:TRICE32_1 float %+9.6f (%%9.3f)\n", aFloat(-x) ); 
            TRICE32_1( Id(49405), "rd:TRICE32_1 float %-f (%%f)\n", aFloat(x) ); 
            TRICE32_1( Id(38959), "rd:TRICE32_1 float %-9f (%%9f)\n", aFloat(x) ); 
            TRICE32_1( Id(37554), "rd:TRICE32_1 float %-.9f (%%.9f)\n", aFloat(x) ); 
            TRICE32_1( Id(43752), "rd:TRICE32_1 float %-9.f (%%9.f)\n", aFloat(x) ); 
            TRICE32_1( Id(41134), "rd:TRICE32_1 float %-9.6f (%%9.3f)\n", aFloat(x) ); 
            TRICE32_1( Id(49405), "rd:TRICE32_1 float %-f (%%f)\n", aFloat(-x) ); 
            TRICE32_1( Id(38959), "rd:TRICE32_1 float %-9f (%%9f)\n", aFloat(-x) ); 
            TRICE32_1( Id(37554), "rd:TRICE32_1 float %-.9f (%%.9f)\n", aFloat(-x) ); 
            TRICE32_1( Id(43752), "rd:TRICE32_1 float %-9.f (%%9.f)\n", aFloat(-x) ); 
            TRICE32_1( Id(41134), "rd:TRICE32_1 float %-9.6f (%%9.3f)\n", aFloat(-x) ); 
				break;
        case 30:
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
        case 31:                                                         				
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
        break;
        case 32:                                                         
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
        case 33:                                                         
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
        case 1:                    
            TRICE( Id(50845), "att:\n" );
            TRICE16_1( Id( 35169), "rd:TRICE16_1 line %u (%%u -1)\n", -1 ); 
            TRICE16_1( Id( 35133), "rd:TRICE16_1 line %b (%%b -1)\n", -1 ); 
            TRICE16_1( Id( 51463), "rd:TRICE16_1 line %o (%%o -1)\n", -1 ); 
            TRICE16_1( Id( 53632), "rd:TRICE16_1 line %x (%%x -1)\n", -1 ); 
            TRICE16_1( Id( 37985), "rd:TRICE16_1 line %d (%%d -1)\n", -1 ); 
            TRICE( Id(48024), "rd:TRICE line %u (%%u)\n", -1 ); 
            TRICE( Id(58374), "rd:TRICE line %b (%%b)\n", -2 ); 
            TRICE( Id(45055), "rd:TRICE line %o (%%o)\n", -3 ); 
            TRICE( Id(37423), "rd:TRICE line %x (%%x)\n", -4 ); 
            TRICE( Id(53816), "rd:TRICE line %d (%%d)\n", -5 ); 
            TRICE32_1( Id(62543), "rd:TRICE32_1 line %u (%%u)\n", -1 ); 
            TRICE32_1( Id(45989), "rd:TRICE32_1 line %b (%%b)\n", -1 ); 
            TRICE32_1( Id(36491), "rd:TRICE32_1 line %o (%%o)\n", -1 ); 
            TRICE32_1( Id(35192), "rd:TRICE32_1 line %x (%%x)\n", -1 ); 
            TRICE32_1( Id(58755), "rd:TRICE32_1 line %d (%%d)\n", -1 ); 
            TRICE64_1( Id(50978), "rd:TRICE64_1 line %u (%%u)\n", -1 ); 
            TRICE64_1( Id(35922), "rd:TRICE64_1 line %b (%%b)\n", -1 ); 
            TRICE64_1( Id(36296), "rd:TRICE64_1 line %o (%%o)\n", -1 ); 
            TRICE64_1( Id(42819), "rd:TRICE64_1 line %x (%%x)\n", -1 ); 
            TRICE64_1( Id(52591), "rd:TRICE64_1 line %d (%%d)\n", -1 ); 
        break;
        case 2:
            TRICE( Id(44084), "msg:\n" );
            TRICE( Id(65259), "ATT:TRICE attention message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(34713), "ATT:TRICE attention message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(53919), "ATT:TRICE attention message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(36779), "ATT:TRICE attention message, SysTick is %6d\n", SYSTICKVAL);
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
        case 3:                    
            TRICE8( Id( 63495), "rd:TRICE8 %d\n", 1 );
            TRICE8( Id( 40220), "rd:TRICE8 %d, %d\n", 1, 2 );
            TRICE8( Id( 56315), "rd:TRICE8 %d, %d, %d\n", 1, 2, 3 );
            TRICE8( Id( 35199), "rd:TRICE8 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE8( Id( 64677), "rd:TRICE8 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE8( Id( 63697), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE8( Id( 62807), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8( Id( 59840), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            TRICE8( Id( 40569), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );      
            TRICE8( Id( 45470), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);      
            TRICE8( Id( 38100), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );      
            TRICE8( Id( 39583), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );      
        break;
        case 4:            
            TRICE16( Id( 61035), "rd:TRICE16 %d\n", 1 );
            TRICE16( Id( 48938), "rd:TRICE16 %d, %d\n", 1, 2 );
            TRICE16( Id( 50921), "rd:TRICE16 %d, %d, %d\n", 1, 2, 3 );
            TRICE16( Id( 38261), "rd:TRICE16 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE16( Id( 36236), "rd:TRICE16 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE16( Id( 43535), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE16( Id( 43174), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE16( Id( 34224), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            TRICE16( Id( 50005), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );      
            TRICE16( Id( 54783), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);      
            TRICE16( Id( 50159), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );      
            TRICE16( Id( 39046), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );  
        break;
        case 5:
            TRICE( Id( 33680), "rd:TRICE --------------------------------------------------\n");            
            TRICE( Id( 61886), "rd:TRICE %d\n", 1 );
            TRICE( Id( 37931), "rd:TRICE %d, %d\n", 1, 2 );
            TRICE( Id( 36583), "rd:TRICE %d, %d, %d\n", 1, 2, 3 );
            TRICE( Id( 52400), "rd:TRICE %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE( Id( 61348), "rd:TRICE %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE( Id( 44290), "rd:TRICE %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE( Id( 35857), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE( Id( 35028), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            TRICE( Id( 37374), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );      
            TRICE( Id( 65335), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);      
            TRICE( Id( 43075), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );      
            TRICE( Id( 35190), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 6:                    
            TRICE64( Id( 63433), "rd:TRICE64 %d\n", 1 );
            TRICE64( Id( 52183), "rd:TRICE64 %d, %d\n", 1, 2 );
            TRICE64( Id( 58343), "rd:TRICE64 %d, %d, %d\n", 1, 2, 3 );
            TRICE64( Id( 59098), "rd:TRICE64 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE64( Id( 36354), "rd:TRICE64 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE64( Id( 58425), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE64( Id( 63581), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE64( Id( 52575), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            TRICE64( Id( 65307), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );      
            TRICE64( Id( 33102), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);      
            TRICE64( Id( 51678), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );      
            TRICE64( Id( 39543), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );        
        break;
        case 7:
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
        case 8:            
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
        break;
        case 9:            
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
        case 10:            
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
        case 11:
            TRICE0(Id( 55247), "--------------------------------------------------\n");
            TRICE0(Id( 51804), "--------------------------------------------------\n");
            TRICE16_1(Id( 55922), "dbg:12345 as 16bit is %016b\n", 12345);
            TRICE0(Id( 45664), "--------------------------------------------------\n");
            TRICE0(Id( 48439), "sig:This ASSERT error is just a demo and no real error:\n");
            TRICE0(Id( 36621), "--------------------------------------------------\n");
            TRICE16_1(Id( 62486), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 50106), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 55534), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 39816), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 60853), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 64581), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 46649), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 45449), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 38091), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 56748), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 51343), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 59255), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
        break;
        case 12:            
            TRICE32_1(Id( 38012), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 33478), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 57446), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 51946), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 34576), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 42368), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 63633), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 53889), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 42207), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 38326), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 61267), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
        break;
        case 13:            
            TRICE8_4( Id( 43420), "tst:TRICE8_4  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4( Id( 46621), "tst:TRICE8_4   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4( Id( 50996), "tst:TRICE8_4   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            TRICE16_4(Id( 37343), "tst:TRICE16_4  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id( 58854), "tst:TRICE16_4   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id( 61023), "tst:TRICE16_4   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE32_4(Id( 47740), "tst:TRICE32_4 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            TRICE32_4(Id( 46761), "tst:TRICE32_4 %%10d ->     %10d     %10d     %10d    %10x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
        break;
        case 14:
            TRICE64_1(Id( 52189), "att:TRICE64_1 %b\n", 0x1122334455667788);
            TRICE8_1(Id( 52582), "tst:TRICE8_1 %02x\n", 0xA1);
            TRICE8_2(Id( 58020), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
            TRICE8_3(Id( 40122), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
            TRICE8_4(Id( 50551), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
            TRICE8_5(Id( 33857), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
            TRICE8_6(Id( 55004), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
            TRICE8_7(Id( 49025), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
            TRICE8_8(Id( 53622), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
            TRICE8_1(Id( 55503), "tst:TRICE8_1 %d\n", -111);
            TRICE8_2(Id( 64874), "tst:TRICE8_2 %d %d\n", -111, -222);
            TRICE8_3(Id( 49107), "tst:TRICE8_3 %d %d %d\n", -111, -222, -3);
            TRICE8_4(Id( 34673), "tst:TRICE8_4 %d %d %d %d\n", -111, -222, -3, -4);
            TRICE8_5(Id( 59637), "tst:TRICE8_5 %d %d %d %d %d\n", -111, -222, -3, -4, -5);
            TRICE8_6(Id( 60537), "tst:TRICE8_6 %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6);
            TRICE8_7(Id( 45261), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7);
            TRICE8_8(Id( 63789), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7, -8);
            break;
        case 15:
            TRICE16_1(Id( 35206), "tst:TRICE16_1 %d\n", -111); // 1
            TRICE16_2(Id( 45725), "tst:TRICE16_2 %d %d\n", -111, -222); // 2
            TRICE16_3(Id( 63897), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333); // 3
            TRICE16_4(Id( 55371), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            TRICE32_1(Id( 58781), "tst:TRICE32_1 %08x\n", 0x0123cafe); // 2
            TRICE32_1(Id( 54389), "tst:TRICE32_1 %d\n", -111); // 2
            TRICE32_2(Id( 59795), "tst:TRICE32_2 %x %x\n", -111, -222); // 4
            TRICE32_2(Id( 47183), "tst:TRICE32_2 %d %d\n", -111, -222); // 4
            TRICE32_3(Id( 39898), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333); // 4
            TRICE32_3(Id( 40901), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333); // 4
            TRICE32_4(Id( 58747), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444); // 4
            TRICE32_4(Id( 59716), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            TRICE64_1(Id( 59670), "tst:TRICE64_1 %d\n", -111); // 2
            TRICE64_2(Id( 46159), "tst:TRICE64_2 %d %d\n", -111, -222); // 4
        break;
        case 28: // colored single letters and several TRICE macros in one line        
            TRICE0(Id( 42984), "e:A");
            TRICE0(Id( 65475), "w:B");
            TRICE0(Id( 60278), "a:c");
            TRICE0(Id( 39056), "wr:d");
            TRICE0(Id( 57073), "rd:e\n");
            TRICE0(Id( 35315), "diag:f");
            TRICE0(Id( 53769), "d:G");
            TRICE0(Id( 38573), "t:H");
            TRICE0(Id( 37916), "time:i");
            TRICE0(Id( 52118), "message:J");
            TRICE0(Id( 49746), "dbg:k\n");
            TRICE0(Id( 40004), "1");
            TRICE0(Id( 61543), "2");
            TRICE0(Id( 51583), "3");
            TRICE0(Id( 35885), "4");
            TRICE0(Id( 40078), "e:7");
            TRICE0(Id( 44255), "m:12");
            TRICE0(Id( 51771), "m:123\n");
            TRICE(Id(63288), "e:A"); TRICE(Id(53934), "w:B"); TRICE(Id(44438), "a:c");
            TRICE(Id(39663), "wr:d"); TRICE(Id(42027), "rd:e\n"); //TRICE(Id(54760), "diag:f");
				    //TRICE(   Id(57680), "%d%d", 2, -2 ); TRICE8(   Id(41705), "%d%d", 2, -2 ); TRICE16(   Id(56897), "%d%d", 2, -2 ); TRICE32(   Id(42128), "%d%d", 2, -2 ); TRICE64(   Id( 41176), "%d%d", 2, -2 );
				    //TRICE_2( Id(40709), "%d%d", 2, -2 ); TRICE8_2( Id(48632), "%d%d", 2, -2 ); TRICE16_2( Id(42184), "%d%d", 2, -2 ); TRICE32_2( Id(56879), "%d%d", 2, -2 ); TRICE64_2( Id(61735), "%d%d", 2, -2 );
        break;
        case 17:            
            TRICE8_1(Id( 54804), "tst:TRICE8_1 %d\n", 1);
            TRICE8_2(Id( 42494), "tst:TRICE8_2 %d %d\n", 1, 2);
            TRICE8_3(Id( 44072), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
            TRICE8_4(Id( 51249), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
            TRICE8_5(Id( 56554), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            TRICE8_6(Id( 37866), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            TRICE8_7(Id( 35663), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8(Id( 53316), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
            break;
        case 18:
            TRICE8_8(Id( 39887), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
            TRICE16_1(Id( 45609), "tst:TRICE16_1 0x%04x\n", 0xa0);
            TRICE8_1(Id( 34438), "tst:TRICE8_1 0x%02x\n", 0xa0);
            TRICE8_1(Id( 40928), "tst:TRICE8_1 0x%02x\n", 0xa0);
            TRICE8_1(Id( 59290), "tst:TRICE8_1 %u\n", 201);
            TRICE8_2(Id( 53955), "tst:TRICE8_2 %u %u\n", 201, 202);
            TRICE8_3(Id( 33982), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
            TRICE8_4(Id( 64087), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
            TRICE8_5(Id( 56344), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
            TRICE8_6(Id( 64958), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
            TRICE8_7(Id( 37787), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
            TRICE8_8(Id( 44560), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
        break;
        case 19:            
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
        case 20:
            TRICE8_8( Id( 48310), "dbg:%d %d %d %d %d %d %d %d\n", 8, 8, 8, 8, 8, 8, 8, 8 );
            s = "AAAAAAAAAAAA";
            TRICE32( Id( 47643), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id( 43140), "sig:%s\n", s ); 
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
        TRICE32( Id( 48152), "dbg:len=%u:", strlen(s) );
        TRICE_S( Id( 43140), "sig:%s\n", s );
        break;
        case 21:
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
        case 22:
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
        case 23:
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
        case 24:
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
            TRICE8_8(Id( 33702), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
            TRICE16_1(Id( 34004), "tst:TRICE16_1 0x%04x\n", 0xa0);
            TRICE8_1(Id( 62342), "tst:TRICE8_1 0x%02x\n", 0xa0);
            TRICE8_1(Id( 34484), "tst:TRICE8_1 0x%02x\n", 0xa0);
            TRICE8_1(Id( 56229), "tst:TRICE8_1 %u\n", 201);
            TRICE8_2(Id( 49010), "tst:TRICE8_2 %u %u\n", 201, 202);
            TRICE8_3(Id( 52509), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
            TRICE8_4(Id( 47415), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
            TRICE8_5(Id( 56700), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
            TRICE8_6(Id( 58847), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
            TRICE8_7(Id( 51359), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
            TRICE8_8(Id( 52236), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
            TRICE16_1(Id( 55311), "tst:TRICE16_1 %u\n", 60001);
            TRICE16_2(Id( 63358), "tst:TRICE16_2 %u %u\n", 60001, 60002);
            TRICE16_3(Id( 58776), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
            TRICE16_4(Id( 51895), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
            TRICE32_1(Id( 33535), "tst:TRICE32_1 %u\n", 4000000001);
            TRICE32_2(Id( 42047), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
            TRICE32_3(Id( 42536), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
            TRICE32_4(Id( 59395), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
            TRICE64_1(Id( 48635), "tst:TRICE64_1 %u\n", -1);
            TRICE64_2(Id( 64317), "tst:TRICE64_2 %u %u\n", -1, -2);
        break;
        default:
        break;
    }
}
