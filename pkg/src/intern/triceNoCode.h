/*! \file triceNoCode.h
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_NO_CODE_H_
#define TRICE_NO_CODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define Trice0i( id, pFmt )
#define Trice8_1i( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)
#define Trice8_2i( id, pFmt, v0, v1                         ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define Trice16_1i( id, pFmt, v0                            ) do{ ((void)(v0)); }while(0)

#define Trice0( id, pFmt )
#define Trice8_1( id, pFmt, v0                              ) do{ ((void)(v0)); }while(0)
#define Trice8_2( id, pFmt, v0, v1                          ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define Trice16_1( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)

#define TRICE0( id, pFmt )
#define TRICE8_1( id, pFmt, v0                              ) do{ ((void)(v0)); }while(0)
#define TRICE8_2( id, pFmt, v0, v1                          ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define TRICE8_3( id, pFmt, v0, v1, v2                      ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define TRICE8_4( id, pFmt, v0, v1, v2, v3                  ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define TRICE8_5( id, pFmt, v0, v1, v2, v3, v4              ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); }while(0)
#define TRICE8_6( id, pFmt, v0, v1, v2, v3, v4, v5          ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); }while(0)
#define TRICE8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6      ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); ((void)(v6)); }while(0)
#define TRICE8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7  ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); ((void)(v6)); ((void)(v7)); }while(0)
#define TRICE16_1( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)
#define TRICE16_2( id, pFmt, v0, v1                         ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define TRICE16_3( id, pFmt, v0, v1, v2                     ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define TRICE16_4( id, pFmt, v0, v1, v2, v3                 ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define TRICE32_1( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)
#define TRICE32_2( id, pFmt, v0, v1                         ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define TRICE32_3( id, pFmt, v0, v1, v2                     ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define TRICE32_4( id, pFmt, v0, v1, v2, v3                 ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define TRICE64_1( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)
#define TRICE64_2( id, pFmt, v0, v1                         ) do{ ((void)(v0)); ((void)(v1)); }while(0)

#define TRICE0i( id, pFmt )
#define TRICE8_1i( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)
#define TRICE8_2i( id, pFmt, v0, v1                         ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define TRICE8_3i( id, pFmt, v0, v1, v2                     ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define TRICE8_4i( id, pFmt, v0, v1, v2, v3                 ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define TRICE8_5i( id, pFmt, v0, v1, v2, v3, v4             ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); }while(0)
#define TRICE8_6i( id, pFmt, v0, v1, v2, v3, v4, v5         ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); }while(0)
#define TRICE8_7i( id, pFmt, v0, v1, v2, v3, v4, v5, v6     ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); ((void)(v6)); }while(0)
#define TRICE8_8i( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); ((void)(v6)); ((void)(v7)); }while(0)
#define TRICE16_1i( id, pFmt, v0                            ) do{ ((void)(v0)); }while(0)
#define TRICE16_2i( id, pFmt, v0, v1                        ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define TRICE16_3i( id, pFmt, v0, v1, v2                    ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define TRICE16_4i( id, pFmt, v0, v1, v2, v3                ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define TRICE32_1i( id, pFmt, v0                            ) do{ ((void)(v0)); }while(0)
#define TRICE32_2i( id, pFmt, v0, v1                        ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define TRICE32_3i( id, pFmt, v0, v1, v2                    ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define TRICE32_4i( id, pFmt, v0, v1, v2, v3                ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define TRICE64_1i( id, pFmt, v0                            ) do{ ((void)(v0)); }while(0)
#define TRICE64_2i( id, pFmt, v0, v1                        ) do{ ((void)(v0)); ((void)(v1)); }while(0)

#define trice0( id, pFmt )
#define trice8_1( id, pFmt, v0                              ) do{ ((void)(v0)); }while(0)
#define trice8_2( id, pFmt, v0, v1                          ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define trice8_3( id, pFmt, v0, v1, v2                      ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define trice8_4( id, pFmt, v0, v1, v2, v3                  ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define trice8_5( id, pFmt, v0, v1, v2, v3, v4              ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); }while(0)
#define trice8_6( id, pFmt, v0, v1, v2, v3, v4, v5          ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); }while(0)
#define trice8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6      ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); ((void)(v6)); }while(0)
#define trice8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7  ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); ((void)(v6)); ((void)(v7)); }while(0)
#define trice16_1( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)
#define trice16_2( id, pFmt, v0, v1                         ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define trice16_3( id, pFmt, v0, v1, v2                     ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define trice16_4( id, pFmt, v0, v1, v2, v3                 ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define trice32_1( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)
#define trice32_2( id, pFmt, v0, v1                         ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define trice32_3( id, pFmt, v0, v1, v2                     ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define trice32_4( id, pFmt, v0, v1, v2, v3                 ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define trice64_1( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)
#define trice64_2( id, pFmt, v0, v1                         ) do{ ((void)(v0)); ((void)(v1)); }while(0)

#define trice0i( id, pFmt )
#define trice8_1i( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)
#define trice8_2i( id, pFmt, v0, v1                         ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define trice8_3i( id, pFmt, v0, v1, v2                     ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define trice8_4i( id, pFmt, v0, v1, v2, v3                 ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define trice8_5i( id, pFmt, v0, v1, v2, v3, v4             ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); }while(0)
#define trice8_6i( id, pFmt, v0, v1, v2, v3, v4, v5         ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); }while(0)
#define trice8_7i( id, pFmt, v0, v1, v2, v3, v4, v5, v6     ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); ((void)(v6)); }while(0)
#define trice8_8i( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); ((void)(v6)); ((void)(v7)); }while(0)
#define trice16_1i( id, pFmt, v0                            ) do{ ((void)(v0)); }while(0)
#define trice16_2i( id, pFmt, v0, v1                        ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define trice16_3i( id, pFmt, v0, v1, v2                    ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define trice16_4i( id, pFmt, v0, v1, v2, v3                ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define trice32_1i( id, pFmt, v0                            ) do{ ((void)(v0)); }while(0)
#define trice32_2i( id, pFmt, v0, v1                        ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define trice32_3i( id, pFmt, v0, v1, v2                    ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define trice32_4i( id, pFmt, v0, v1, v2, v3                ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define trice64_1i( id, pFmt, v0                            ) do{ ((void)(v0)); }while(0)
#define trice64_2i( id, pFmt, v0, v1                        ) do{ ((void)(v0)); ((void)(v1)); }while(0)

#ifdef __cplusplus
}
#endif

#endif // TRICE_NO_CODE_H_
