/*! \file TriceOff.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICEOFF
#define TRICEOFF

#define TRICE_ENTER
#define TRICE_LEAVE
#define TRICE_PUT(n)           // do{ ((void)(n)); }while(0)
#define TRICE_PUT16(n)         // do{ ((void)(n)); }while(0)
#define TRICE_PUT16_1616(x, n) // do{ ((void)(x)); ((void)(n)); }while(0)
#define PUT_BUFFER(b, l)       // do{ ((void)(b)); ((void)(l)); }while(0)
#define TRICE_S(id, p, s)      // do{ ((void)(id)); ((void)(p)); ((void)(s)); }while(0)
#define TRICE_N(id, p, s, n)   // do{ ((void)(id)); ((void)(p)); ((void)(s)); ((void)(n)); }while(0)

#define trice8(fmt, ...)
#define Trice8(fmt, ...)
#define TRice8(fmt, ...)

#define trice8_0(fmt)                                                    //!< trice8_1 is a macro calling a function to reduce code size.
#define trice8_1(fmt, v0)                                                //!< trice8_1 is a macro calling a function to reduce code size.
#define trice8_2(fmt, v0, v1)                                            //!< trice8_2 is a macro calling a function to reduce code size.
#define trice8_3(fmt, v0, v1, v2)                                        //!< trice8_3 is a macro calling a function to reduce code size.
#define trice8_4(fmt, v0, v1, v2, v3)                                    //!< trice8_4 is a macro calling a function to reduce code size.
#define trice8_5(fmt, v0, v1, v2, v3, v4)                                //!< trice8_5 is a macro calling a function to reduce code size.
#define trice8_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< trice8_6 is a macro calling a function to reduce code size.
#define trice8_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< trice8_7 is a macro calling a function to reduce code size.
#define trice8_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< trice8_8 is a macro calling a function to reduce code size.
#define trice8_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< trice8_9 is a macro calling a function to reduce code size.
#define trice8_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< trice8_10 is a macro calling a function to reduce code size.
#define trice8_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< trice8_11 is a macro calling a function to reduce code size.
#define trice8_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< trice8_12 is a macro calling a function to reduce code size.

#define Trice8_0(fmt)                                                    //!< Trice8_1 is a macro calling a function to reduce code size.
#define Trice8_1(fmt, v0)                                                //!< Trice8_1 is a macro calling a function to reduce code size.
#define Trice8_2(fmt, v0, v1)                                            //!< Trice8_2 is a macro calling a function to reduce code size.
#define Trice8_3(fmt, v0, v1, v2)                                        //!< Trice8_3 is a macro calling a function to reduce code size.
#define Trice8_4(fmt, v0, v1, v2, v3)                                    //!< Trice8_4 is a macro calling a function to reduce code size.
#define Trice8_5(fmt, v0, v1, v2, v3, v4)                                //!< Trice8_5 is a macro calling a function to reduce code size.
#define Trice8_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< Trice8_6 is a macro calling a function to reduce code size.
#define Trice8_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< Trice8_7 is a macro calling a function to reduce code size.
#define Trice8_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< Trice8_8 is a macro calling a function to reduce code size.
#define Trice8_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< Trice8_9 is a macro calling a function to reduce code size.
#define Trice8_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< Trice8_10 is a macro calling a function to reduce code size.
#define Trice8_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< Trice8_11 is a macro calling a function to reduce code size.
#define Trice8_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< Trice8_12 is a macro calling a function to reduce code size.

#define TRice8_0(fmt)                                                    //!< TRice8_1 is a macro calling a function to reduce code size.
#define TRice8_1(fmt, v0)                                                //!< TRice8_1 is a macro calling a function to reduce code size.
#define TRice8_2(fmt, v0, v1)                                            //!< TRice8_2 is a macro calling a function to reduce code size.
#define TRice8_3(fmt, v0, v1, v2)                                        //!< TRice8_3 is a macro calling a function to reduce code size.
#define TRice8_4(fmt, v0, v1, v2, v3)                                    //!< TRice8_4 is a macro calling a function to reduce code size.
#define TRice8_5(fmt, v0, v1, v2, v3, v4)                                //!< TRice8_5 is a macro calling a function to reduce code size.
#define TRice8_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< TRice8_6 is a macro calling a function to reduce code size.
#define TRice8_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< TRice8_7 is a macro calling a function to reduce code size.
#define TRice8_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< TRice8_8 is a macro calling a function to reduce code size.
#define TRice8_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< TRice8_9 is a macro calling a function to reduce code size.
#define TRice8_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< TRice8_10 is a macro calling a function to reduce code size.
#define TRice8_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< TRice8_11 is a macro calling a function to reduce code size.
#define TRice8_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< TRice8_12 is a macro calling a function to reduce code size.

#endif // #ifndef TRICEOFF
