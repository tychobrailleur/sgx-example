#include "app.h"
#include "Enclave_u.h"

#include <stdio.h>
#include "sgx_urts.h"


// Code from
//   https://github.com/intel/linux-sgx/blob/master/SampleCode/SampleEnclave/App/App.cpp
/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

typedef struct _sgx_errlist_t {
  sgx_status_t err;
  const char *msg;
  const char *sug; /* Suggestion */
} sgx_errlist_t;


/* Error code returned by sgx_create_enclave */
static sgx_errlist_t sgx_errlist[] = {
  {
    SGX_ERROR_UNEXPECTED,
    "Unexpected error occurred.",
    NULL
  },
  {
    SGX_ERROR_INVALID_PARAMETER,
    "Invalid parameter.",
    NULL
  },
  {
    SGX_ERROR_OUT_OF_MEMORY,
    "Out of memory.",
    NULL
  },
  {
    SGX_ERROR_ENCLAVE_LOST,
    "Power transition occurred.",
    "Please refer to the sample \"PowerTransition\" for details."
  },
  {
    SGX_ERROR_INVALID_ENCLAVE,
    "Invalid enclave image.",
    NULL
  },
  {
    SGX_ERROR_INVALID_ENCLAVE_ID,
    "Invalid enclave identification.",
    NULL
  },
  {
    SGX_ERROR_INVALID_SIGNATURE,
    "Invalid enclave signature.",
    NULL
  },
  {
    SGX_ERROR_OUT_OF_EPC,
    "Out of EPC memory.",
    NULL
  },
  {
    SGX_ERROR_NO_DEVICE,
    "Invalid SGX device.",
    "Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards."
  },
  {
    SGX_ERROR_MEMORY_MAP_CONFLICT,
    "Memory map conflicted.",
    NULL
  },
  {
    SGX_ERROR_INVALID_METADATA,
    "Invalid enclave metadata.",
    NULL
  },
  {
    SGX_ERROR_DEVICE_BUSY,
    "SGX device was busy.",
    NULL
  },
  {
    SGX_ERROR_INVALID_VERSION,
    "Enclave version was invalid.",
    NULL
  },
  {
    SGX_ERROR_INVALID_ATTRIBUTE,
    "Enclave was not authorized.",
    NULL
  },
  {
    SGX_ERROR_ENCLAVE_FILE_ACCESS,
    "Can't open enclave file.",
    NULL
  },
  {
    SGX_ERROR_MEMORY_MAP_FAILURE,
    "Failed to reserve memory for the enclave.",
    NULL
  },
};

/* Check error conditions for loading enclave */
void print_error_message(sgx_status_t ret)
{
  size_t idx = 0;
  size_t ttl = sizeof sgx_errlist/sizeof sgx_errlist[0];

  for (idx = 0; idx < ttl; idx++) {
    if (ret == sgx_errlist[idx].err) {
      if (NULL != sgx_errlist[idx].sug) {
        printf("Info: %s\n", sgx_errlist[idx].sug);
      }
      printf("Error: %s\n", sgx_errlist[idx].msg);
      break;
    }
  }

  if (idx == ttl)
    printf("Error code is 0x%X. Please refer to the \"Intel SGX SDK Developer Reference\" for more details.\n", ret);
}

//  End code from https://github.com/intel/linux-sgx/blob/master/SampleCode/SampleEnclave/App/App.cpp

int initialize_enclave() {
  sgx_status_t ret = SGX_ERROR_UNEXPECTED;

  /* Call sgx_create_enclave to initialize an enclave instance */
  /* Debug Support: set 2nd parameter to 1 */
  ret = sgx_create_enclave("enclave.signed.so", SGX_DEBUG_FLAG, NULL, NULL, &global_eid, NULL);
  if (ret != SGX_SUCCESS) {
    print_error_message(ret);
    return -1;
  }

  return 0;
}


int main(int argc, char *argv[]) {
  int init;


  // initialise enclave
  init = initialize_enclave();
  if (init < 0) {
    printf("Error initialising enclave.\n");
    return -1;
  }

  display_message("Hello, World!\n", global_eid);

  // destroy enclave
  sgx_destroy_enclave(global_eid);

  return 0;
}
