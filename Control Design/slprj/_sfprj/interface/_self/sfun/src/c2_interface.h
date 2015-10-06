#ifndef __c2_interface_h__
#define __c2_interface_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc2_interfaceInstanceStruct
#define typedef_SFc2_interfaceInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c2_sfEvent;
  boolean_T c2_isStable;
  boolean_T c2_doneDoubleBufferReInit;
  uint8_T c2_is_active_c2_interface;
  real_T *c2_lift;
  real_T *c2_ae1;
  real_T *c2_pitch;
  real_T *c2_roll;
  real_T *c2_yaw;
  real_T *c2_ae2;
  real_T *c2_ae3;
  real_T *c2_ae4;
} SFc2_interfaceInstanceStruct;

#endif                                 /*typedef_SFc2_interfaceInstanceStruct*/

/* Named Constants */

/* Variable Declarations */
extern struct SfDebugInstanceStruct *sfGlobalDebugInstanceStruct;

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c2_interface_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c2_interface_get_check_sum(mxArray *plhs[]);
extern void c2_interface_method_dispatcher(SimStruct *S, int_T method, void
  *data);

#endif
