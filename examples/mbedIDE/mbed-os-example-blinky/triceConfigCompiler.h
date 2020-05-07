/*! \file triceStm32CompilerConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_COMPILER_CONFIG_H_
#define TRICE_COMPILER_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif


#if defined( __GNUC__ ) /* gnu compiler ###################################### */ \
 || defined (__IAR_SYSTEMS_ICC__) /* IAR compiler ############################ */

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4                                  //!< align to 4 byte boundary preamble
#define ALIGN4_END __attribute__ ((aligned(4))) //!< align to 4 byte boundary post declaration
#define PACKED                                  //!< pack data preamble
#define PACKED_END __attribute__ ((packed))      //!< pack data post declaration

#elif defined(__arm__) // ARMkeil IDE #########################################

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4 __align(4) //!< align to 4 byte boundary preamble
#define ALIGN4_END        //!< align to 4 byte boundary post declaration
#define PACKED __packed   //!< pack data preamble
#define PACKED_END        //!< pack data post declaration

#else // ######################################################################
#error unknown compliler
#endif // compiler adaptions ##################################################


#ifdef __cplusplus
}
#endif

#endif /* TRICE_COMPILER_CONFIG_H_ */
