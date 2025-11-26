#include "trice.h"

// clang-format off

void TriceCheckSpecial( int n ) {

        static int8_t b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
        static int16_t b16[] = { 0, -1, -2, 0x3344 };
        static int32_t b32[] = { 0, -1, -2, 0x33445555};
        static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };

        switch(n){
                        case __LINE__: TRice("Hello World!\n");                                             //exp: Time:feed3322default: Hello World!
                break;  case __LINE__: TRICE8_F(ID(0), "info:FunctionNameWa", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: Time:feed3322default: info:FunctionNameWa(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
                break;  case __LINE__: TRice8F("info:FunctionNameWb", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: Time:feed3322default: info:FunctionNameWb(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
                break;  case __LINE__: Trice8F("info:FunctionNameWc", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: Time:    be16default: info:FunctionNameWc(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
                break;  case __LINE__: trice8F("info:FunctionNameWd", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: Time:        default: info:FunctionNameWd(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
                break;  case __LINE__: TRICE16_F(ID(0), "info:FunctionNameXa", b16, sizeof(b16)/sizeof(int16_t) ); //exp: Time:feed3322default: info:FunctionNameXa(0000)(ffff)(fffe)(3344) 
                break;  case __LINE__: TRice16F("info:FunctionNameXb", b16, sizeof(b16)/sizeof(int16_t) );  //exp: Time:feed3322default: info:FunctionNameXb(0000)(ffff)(fffe)(3344) 
                break;  case __LINE__: Trice16F("info:FunctionNameXc", b16, sizeof(b16)/sizeof(int16_t) );  //exp: Time:    be16default: info:FunctionNameXc(0000)(ffff)(fffe)(3344) 
                break;  case __LINE__: trice16F("info:FunctionNameXd", b16, sizeof(b16)/sizeof(int16_t) );  //exp: Time:        default: info:FunctionNameXd(0000)(ffff)(fffe)(3344) 
                break;  case __LINE__: TRICE32_F(ID(0), "info:FunctionNameYa", b32, sizeof(b32)/sizeof(int32_t) ); //exp: Time:feed3322default: info:FunctionNameYa(00000000)(ffffffff)(fffffffe)(33445555)
                break;  case __LINE__: TRice32F("rpc:FunctionNameYb",  b32, sizeof(b32)/sizeof(int32_t) );  //exp: Time:feed3322default: rpc:FunctionNameYb(00000000)(ffffffff)(fffffffe)(33445555)
                break;  case __LINE__: Trice32F("rpc:FunctionNameYc",  b32, sizeof(b32)/sizeof(int32_t) );  //exp: Time:    be16default: rpc:FunctionNameYc(00000000)(ffffffff)(fffffffe)(33445555)
                break;  case __LINE__: trice32F("call:FunctionNameYd", b32, sizeof(b32)/sizeof(int32_t) );  //exp: Time:        default: call:FunctionNameYd(00000000)(ffffffff)(fffffffe)(33445555)
                break;  case __LINE__: TRICE64_F(ID(0), "info:FunctionNameZa", b64, sizeof(b64)/sizeof(int64_t) ); //exp: Time:feed3322default: info:FunctionNameZa(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
                break;  case __LINE__: TRice64F("call:FunctionNameZb", b64, sizeof(b64)/sizeof(int64_t) );  //exp: Time:feed3322default: call:FunctionNameZb(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
                break;  case __LINE__: Trice64F("RPC:FunctionNameZc",  b64, sizeof(b64)/sizeof(int64_t) );  //exp: Time:    be16default: RPC:FunctionNameZc(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
                break;  case __LINE__: trice64F("Rpc:FunctionNameZd",  b64, sizeof(b64)/sizeof(int64_t) );  //exp: Time:        default: Rpc:FunctionNameZd(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        }
}

// clang-format on
