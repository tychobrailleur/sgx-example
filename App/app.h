#ifndef _APP_H_
#define _APP_H_

#include "sgx_error.h"       /* sgx_status_t */
#include "sgx_eid.h"     /* sgx_enclave_id_t */

extern sgx_enclave_id_t global_eid;    /* global enclave id */

#ifndef TRUE
# define TRUE 1
#endif

#ifndef FALSE
# define FALSE 0
#endif

#endif /* !_APP_H_ */
