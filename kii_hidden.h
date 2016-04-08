#ifndef _KII_HIDDEN_
#define _KII_HIDDEN_

#ifdef __cplusplus
extern "C" {
#endif

/** Initializes Kii SDK
 *
 *  This method is for SDK developer. Application developers does not
 *  need to use this method.
 *
 *  \param [inout] kii sdk instance.
 *  \param [in] site the input of site name,
 *  should be one of "CN", "CN3", "JP", "US", "SG"
 *  \param [in] app_id the input of Application ID
 *  \param [in] app_key the input of Application Key
 *  \param [in] info the input of SDK info.
 *  \return  0:success, -1: failure
 */
kii_error_code_t _kii_init_with_info(
        kii_t* kii,
        const char* site,
        const char* app_id,
        const char* app_key,
        const char* info);

#ifdef __cplusplus
}
#endif

#endif
