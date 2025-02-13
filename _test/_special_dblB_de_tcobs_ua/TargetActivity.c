#include "trice.h"

void TriceCheckSpecial( int n ) {

        static int8_t b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
        static int16_t b16[] = { 0, -1, -2, 0x3344 };
        static int32_t b32[] = { 0, -1, -2, 0x33445555};
        static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };

        switch(n){
                        case __LINE__: TRice(iD(15197), "Hello World!\n");                                                    //exp: feed3322 Hello World!
                break;  case __LINE__: TRICE8_F(ID(15198), "info:FunctionNameWa", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: feed3322 FunctionNameWa(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
                break;  case __LINE__: TRice8F(iD(15199), "info:FunctionNameWb", b8,  sizeof(b8) /sizeof(int8_t) );           //exp: feed3322 FunctionNameWb(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
                break;  case __LINE__: Trice8F(iD(15200), "info:FunctionNameWc", b8,  sizeof(b8) /sizeof(int8_t) );           //exp:     be16 FunctionNameWc(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
                break;  case __LINE__: trice8F(iD(15201), "info:FunctionNameWd", b8,  sizeof(b8) /sizeof(int8_t) );           //exp:          FunctionNameWd(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
                break;  case __LINE__: TRICE16_F(ID(15202), "info:FunctionNameXa", b16, sizeof(b16)/sizeof(int16_t) ); //exp: feed3322 FunctionNameXa(0000)(ffff)(fffe)(3344) 
                break;  case __LINE__: TRice16F(iD(15203), "info:FunctionNameXb", b16, sizeof(b16)/sizeof(int16_t) );         //exp: feed3322 FunctionNameXb(0000)(ffff)(fffe)(3344) 
                break;  case __LINE__: Trice16F(iD(15204), "info:FunctionNameXc", b16, sizeof(b16)/sizeof(int16_t) );         //exp:     be16 FunctionNameXc(0000)(ffff)(fffe)(3344) 
                break;  case __LINE__: trice16F(iD(15205), "info:FunctionNameXd", b16, sizeof(b16)/sizeof(int16_t) );         //exp:          FunctionNameXd(0000)(ffff)(fffe)(3344) 
                break;  case __LINE__: TRICE32_F(ID(15206), "info:FunctionNameYa", b32, sizeof(b32)/sizeof(int32_t) ); //exp: feed3322 FunctionNameYa(00000000)(ffffffff)(fffffffe)(33445555)
                break;  case __LINE__: TRice32F(iD(15207), "rpc:FunctionNameYb",  b32, sizeof(b32)/sizeof(int32_t) );         //exp: feed3322 FunctionNameYb(00000000)(ffffffff)(fffffffe)(33445555)
                break;  case __LINE__: Trice32F(iD(15208), "rpc:FunctionNameYc",  b32, sizeof(b32)/sizeof(int32_t) );         //exp:     be16 FunctionNameYc(00000000)(ffffffff)(fffffffe)(33445555)
                break;  case __LINE__: trice32F(iD(15209), "call:FunctionNameYd", b32, sizeof(b32)/sizeof(int32_t) );         //exp:          call:FunctionNameYd(00000000)(ffffffff)(fffffffe)(33445555)
                break;  case __LINE__: TRICE64_F(ID(15210), "info:FunctionNameZa", b64, sizeof(b64)/sizeof(int64_t) ); //exp: feed3322 FunctionNameZa(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
                break;  case __LINE__: TRice64F(iD(15211), "call:FunctionNameZb", b64, sizeof(b64)/sizeof(int64_t) );         //exp: feed3322 call FunctionNameZb(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
                break;  case __LINE__: Trice64F(iD(15212), "RPC:FunctionNameZc",  b64, sizeof(b64)/sizeof(int64_t) );         //exp:     be16 RPC:FunctionNameZc(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
                break;  case __LINE__: trice64F(iD(15213), "Rpc:FunctionNameZd",  b64, sizeof(b64)/sizeof(int64_t) );         //exp:          Rpc:FunctionNameZd(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        }
}
