/*!\file treyferCrypto.h
\brief 64 bit Crypto Module Code
\details Contains 64 bit encrypt and decrypt routines dclarations
*/
#ifndef TreyferCrypto_h__
#define TreyferCrypto_h__  /*!< avoid multiple inclusion of this file */


#ifdef AUTOMATIC_HEADER_INLUSION_
// use this for doxygen, define AUTOMATIC_HEADER_INLUSION_ in
// .doxygen file section PREDEFINED
#include <stdint.h>
#endif /* #ifdef AUTOMATIC_HEADER_INLUSION_ */

/*!
* \startuml
* \startditaa
* /----------------------------------------------------------------------\
* |                                                                      |
* |                    /------------------------\                        |
* |                    |                        |                        |
* |                    |                        |                        |
* |             data ->*        encrypt         *-> enrypted data        |
* |                    |                        |                        |
* |                    |                   cYEL |                        |
* |                    \------------------------/                        |
* |Application Layer                                                 cDD0|
* \----------------------------------------------------------------------/
* \endditaa
* \enduml
* <EM>Figure: View of encryption routine</EM><BR><BR>
*/
void encrypt(uint8_t text[8], uint8_t const key[8]);

/*!
* \startuml
* \startditaa
* /----------------------------------------------------------------------\
* |                                                                      |
* |                    /------------------------\                        |
* |                    |                        |                        |
* |                    |                        |                        |
* |    enrypted data ->*        decrypt         *->  data                |
* |                    |                        |                        |
* |                    |                   cYEL |                        |
* |                    \------------------------/                        |
* |Application Layer                                                 cDD0|
* \----------------------------------------------------------------------/
* \endditaa
* \enduml
* <EM>Figure: View of decryption routine</EM><BR><BR>
*/
void decrypt(uint8_t text[8], uint8_t const key[8]);

#endif /* #ifndef TreyferCrypto_h__ */
