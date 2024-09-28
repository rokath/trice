/*! \file triceOff.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#define TRICE_ENTER
#define TRICE_LEAVE
#define TRICE_PUT(n)           // do{ ((void)(n)); }while(0)
#define TRICE_PUT16(n)         // do{ ((void)(n)); }while(0)
#define TRICE_PUT16_1616(x, n) // do{ ((void)(x)); ((void)(n)); }while(0)
#define PUT_BUFFER(b, l)       // do{ ((void)(b)); ((void)(l)); }while(0)
#define TRICE_S(id, p, s)      // do{ ((void)(id)); ((void)(p)); ((void)(s)); }while(0)
#define TRICE_N(id, p, s, n)   // do{ ((void)(id)); ((void)(p)); ((void)(s)); ((void)(n)); }while(0)

#define trice(fmt, ...)
#define Trice(fmt, ...)
#define TRice(fmt, ...)

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

#define trice16(fmt, ...)
#define Trice16(fmt, ...)
#define TRice16(fmt, ...)

#define trice16_0(fmt)                                                    //!< trice16_1 is a macro calling a function to reduce code size.
#define trice16_1(fmt, v0)                                                //!< trice16_1 is a macro calling a function to reduce code size.
#define trice16_2(fmt, v0, v1)                                            //!< trice16_2 is a macro calling a function to reduce code size.
#define trice16_3(fmt, v0, v1, v2)                                        //!< trice16_3 is a macro calling a function to reduce code size.
#define trice16_4(fmt, v0, v1, v2, v3)                                    //!< trice16_4 is a macro calling a function to reduce code size.
#define trice16_5(fmt, v0, v1, v2, v3, v4)                                //!< trice16_5 is a macro calling a function to reduce code size.
#define trice16_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< trice16_6 is a macro calling a function to reduce code size.
#define trice16_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< trice16_7 is a macro calling a function to reduce code size.
#define trice16_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< trice16_8 is a macro calling a function to reduce code size.
#define trice16_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< trice16_9 is a macro calling a function to reduce code size.
#define trice16_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< trice16_10 is a macro calling a function to reduce code size.
#define trice16_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< trice16_11 is a macro calling a function to reduce code size.
#define trice16_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< trice16_12 is a macro calling a function to reduce code size.

#define Trice16_0(fmt)                                                    //!< Trice16_1 is a macro calling a function to reduce code size.
#define Trice16_1(fmt, v0)                                                //!< Trice16_1 is a macro calling a function to reduce code size.
#define Trice16_2(fmt, v0, v1)                                            //!< Trice16_2 is a macro calling a function to reduce code size.
#define Trice16_3(fmt, v0, v1, v2)                                        //!< Trice16_3 is a macro calling a function to reduce code size.
#define Trice16_4(fmt, v0, v1, v2, v3)                                    //!< Trice16_4 is a macro calling a function to reduce code size.
#define Trice16_5(fmt, v0, v1, v2, v3, v4)                                //!< Trice16_5 is a macro calling a function to reduce code size.
#define Trice16_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< Trice16_6 is a macro calling a function to reduce code size.
#define Trice16_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< Trice16_7 is a macro calling a function to reduce code size.
#define Trice16_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< Trice16_8 is a macro calling a function to reduce code size.
#define Trice16_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< Trice16_9 is a macro calling a function to reduce code size.
#define Trice16_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< Trice16_10 is a macro calling a function to reduce code size.
#define Trice16_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< Trice16_11 is a macro calling a function to reduce code size.
#define Trice16_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< Trice16_12 is a macro calling a function to reduce code size.

#define TRice16_0(fmt)                                                    //!< TRice16_1 is a macro calling a function to reduce code size.
#define TRice16_1(fmt, v0)                                                //!< TRice16_1 is a macro calling a function to reduce code size.
#define TRice16_2(fmt, v0, v1)                                            //!< TRice16_2 is a macro calling a function to reduce code size.
#define TRice16_3(fmt, v0, v1, v2)                                        //!< TRice16_3 is a macro calling a function to reduce code size.
#define TRice16_4(fmt, v0, v1, v2, v3)                                    //!< TRice16_4 is a macro calling a function to reduce code size.
#define TRice16_5(fmt, v0, v1, v2, v3, v4)                                //!< TRice16_5 is a macro calling a function to reduce code size.
#define TRice16_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< TRice16_6 is a macro calling a function to reduce code size.
#define TRice16_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< TRice16_7 is a macro calling a function to reduce code size.
#define TRice16_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< TRice16_8 is a macro calling a function to reduce code size.
#define TRice16_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< TRice16_9 is a macro calling a function to reduce code size.
#define TRice16_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< TRice16_10 is a macro calling a function to reduce code size.
#define TRice16_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< TRice16_11 is a macro calling a function to reduce code size.
#define TRice16_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< TRice16_12 is a macro calling a function to reduce code size.

#define trice32(fmt, ...)
#define Trice32(fmt, ...)
#define TRice32(fmt, ...)

#define trice32_0(fmt)                                                    //!< trice32_1 is a macro calling a function to reduce code size.
#define trice32_1(fmt, v0)                                                //!< trice32_1 is a macro calling a function to reduce code size.
#define trice32_2(fmt, v0, v1)                                            //!< trice32_2 is a macro calling a function to reduce code size.
#define trice32_3(fmt, v0, v1, v2)                                        //!< trice32_3 is a macro calling a function to reduce code size.
#define trice32_4(fmt, v0, v1, v2, v3)                                    //!< trice32_4 is a macro calling a function to reduce code size.
#define trice32_5(fmt, v0, v1, v2, v3, v4)                                //!< trice32_5 is a macro calling a function to reduce code size.
#define trice32_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< trice32_6 is a macro calling a function to reduce code size.
#define trice32_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< trice32_7 is a macro calling a function to reduce code size.
#define trice32_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< trice32_8 is a macro calling a function to reduce code size.
#define trice32_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< trice32_9 is a macro calling a function to reduce code size.
#define trice32_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< trice32_10 is a macro calling a function to reduce code size.
#define trice32_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< trice32_11 is a macro calling a function to reduce code size.
#define trice32_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< trice32_12 is a macro calling a function to reduce code size.

#define Trice32_0(fmt)                                                    //!< Trice32_1 is a macro calling a function to reduce code size.
#define Trice32_1(fmt, v0)                                                //!< Trice32_1 is a macro calling a function to reduce code size.
#define Trice32_2(fmt, v0, v1)                                            //!< Trice32_2 is a macro calling a function to reduce code size.
#define Trice32_3(fmt, v0, v1, v2)                                        //!< Trice32_3 is a macro calling a function to reduce code size.
#define Trice32_4(fmt, v0, v1, v2, v3)                                    //!< Trice32_4 is a macro calling a function to reduce code size.
#define Trice32_5(fmt, v0, v1, v2, v3, v4)                                //!< Trice32_5 is a macro calling a function to reduce code size.
#define Trice32_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< Trice32_6 is a macro calling a function to reduce code size.
#define Trice32_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< Trice32_7 is a macro calling a function to reduce code size.
#define Trice32_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< Trice32_8 is a macro calling a function to reduce code size.
#define Trice32_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< Trice32_9 is a macro calling a function to reduce code size.
#define Trice32_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< Trice32_10 is a macro calling a function to reduce code size.
#define Trice32_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< Trice32_11 is a macro calling a function to reduce code size.
#define Trice32_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< Trice32_12 is a macro calling a function to reduce code size.

#define TRice32_0(fmt)                                                    //!< TRice32_1 is a macro calling a function to reduce code size.
#define TRice32_1(fmt, v0)                                                //!< TRice32_1 is a macro calling a function to reduce code size.
#define TRice32_2(fmt, v0, v1)                                            //!< TRice32_2 is a macro calling a function to reduce code size.
#define TRice32_3(fmt, v0, v1, v2)                                        //!< TRice32_3 is a macro calling a function to reduce code size.
#define TRice32_4(fmt, v0, v1, v2, v3)                                    //!< TRice32_4 is a macro calling a function to reduce code size.
#define TRice32_5(fmt, v0, v1, v2, v3, v4)                                //!< TRice32_5 is a macro calling a function to reduce code size.
#define TRice32_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< TRice32_6 is a macro calling a function to reduce code size.
#define TRice32_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< TRice32_7 is a macro calling a function to reduce code size.
#define TRice32_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< TRice32_8 is a macro calling a function to reduce code size.
#define TRice32_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< TRice32_9 is a macro calling a function to reduce code size.
#define TRice32_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< TRice32_10 is a macro calling a function to reduce code size.
#define TRice32_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< TRice32_11 is a macro calling a function to reduce code size.
#define TRice32_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< TRice32_12 is a macro calling a function to reduce code size.

#define trice64(fmt, ...)
#define Trice64(fmt, ...)
#define TRice64(fmt, ...)

#define trice64_0(fmt)                                                    //!< trice64_1 is a macro calling a function to reduce code size.
#define trice64_1(fmt, v0)                                                //!< trice64_1 is a macro calling a function to reduce code size.
#define trice64_2(fmt, v0, v1)                                            //!< trice64_2 is a macro calling a function to reduce code size.
#define trice64_3(fmt, v0, v1, v2)                                        //!< trice64_3 is a macro calling a function to reduce code size.
#define trice64_4(fmt, v0, v1, v2, v3)                                    //!< trice64_4 is a macro calling a function to reduce code size.
#define trice64_5(fmt, v0, v1, v2, v3, v4)                                //!< trice64_5 is a macro calling a function to reduce code size.
#define trice64_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< trice64_6 is a macro calling a function to reduce code size.
#define trice64_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< trice64_7 is a macro calling a function to reduce code size.
#define trice64_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< trice64_8 is a macro calling a function to reduce code size.
#define trice64_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< trice64_9 is a macro calling a function to reduce code size.
#define trice64_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< trice64_10 is a macro calling a function to reduce code size.
#define trice64_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< trice64_11 is a macro calling a function to reduce code size.
#define trice64_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< trice64_12 is a macro calling a function to reduce code size.

#define Trice64_0(fmt)                                                    //!< Trice64_1 is a macro calling a function to reduce code size.
#define Trice64_1(fmt, v0)                                                //!< Trice64_1 is a macro calling a function to reduce code size.
#define Trice64_2(fmt, v0, v1)                                            //!< Trice64_2 is a macro calling a function to reduce code size.
#define Trice64_3(fmt, v0, v1, v2)                                        //!< Trice64_3 is a macro calling a function to reduce code size.
#define Trice64_4(fmt, v0, v1, v2, v3)                                    //!< Trice64_4 is a macro calling a function to reduce code size.
#define Trice64_5(fmt, v0, v1, v2, v3, v4)                                //!< Trice64_5 is a macro calling a function to reduce code size.
#define Trice64_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< Trice64_6 is a macro calling a function to reduce code size.
#define Trice64_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< Trice64_7 is a macro calling a function to reduce code size.
#define Trice64_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< Trice64_8 is a macro calling a function to reduce code size.
#define Trice64_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< Trice64_9 is a macro calling a function to reduce code size.
#define Trice64_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< Trice64_10 is a macro calling a function to reduce code size.
#define Trice64_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< Trice64_11 is a macro calling a function to reduce code size.
#define Trice64_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< Trice64_12 is a macro calling a function to reduce code size.

#define TRice64_0(fmt)                                                    //!< TRice64_1 is a macro calling a function to reduce code size.
#define TRice64_1(fmt, v0)                                                //!< TRice64_1 is a macro calling a function to reduce code size.
#define TRice64_2(fmt, v0, v1)                                            //!< TRice64_2 is a macro calling a function to reduce code size.
#define TRice64_3(fmt, v0, v1, v2)                                        //!< TRice64_3 is a macro calling a function to reduce code size.
#define TRice64_4(fmt, v0, v1, v2, v3)                                    //!< TRice64_4 is a macro calling a function to reduce code size.
#define TRice64_5(fmt, v0, v1, v2, v3, v4)                                //!< TRice64_5 is a macro calling a function to reduce code size.
#define TRice64_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< TRice64_6 is a macro calling a function to reduce code size.
#define TRice64_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< TRice64_7 is a macro calling a function to reduce code size.
#define TRice64_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< TRice64_8 is a macro calling a function to reduce code size.
#define TRice64_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< TRice64_9 is a macro calling a function to reduce code size.
#define TRice64_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< TRice64_10 is a macro calling a function to reduce code size.
#define TRice64_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< TRice64_11 is a macro calling a function to reduce code size.
#define TRice64_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< TRice64_12 is a macro calling a function to reduce code size.
