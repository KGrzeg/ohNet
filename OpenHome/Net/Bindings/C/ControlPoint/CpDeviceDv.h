#ifndef HEADER_CPDEVICEDVC
#define HEADER_CPDEVICEDVC

#include <OpenHome/Defines.h>
#include <OpenHome/Net/Bindings/C/CpDevice.h>
#include <OpenHome/Net/Bindings/C/DvDevice.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CpDevice
 * @ingroup ControlPoint
 * @{
 */

/**
 * Create a Control point device which communicates directly with a device stack's device
 *
 * @param[in] aDevice  Handle returned by DvDeviceCreate[NoResources]
 *
 * @return  Device handle
 */
DllExport CpDeviceC STDCALL CpDeviceDvCreate(DvDeviceC aDevice);

/* @} */

#ifdef __cplusplus
} // extern "C"
#endif

#endif // HEADER_CPDEVICEDVC
