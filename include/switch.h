/**
 * @file switch.h
 * @brief Central Switch header. Includes all others.
 * @copyright libnx Authors
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "switch/types.h"
#include "switch/result.h"

#include "switch/arm/atomics.h"
#include "switch/arm/tls.h"

#include "switch/kernel/svc.h"
#include "switch/kernel/ipc.h"

#include "switch/services/sm.h"

#ifdef __cplusplus
}
#endif