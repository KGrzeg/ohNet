/**
 * UPnP library internal APIs
 *
 * NOT intended for use by clients of the library
 */

#ifndef HEADER_DVI_STACK
#define HEADER_DVI_STACK

#include <OpenHome/Types.h>
#include <OpenHome/Env.h>
#include <OpenHome/Thread.h>
#include <OpenHome/Net/Device/Upnp/DviServerUpnp.h>
#include <OpenHome/Net/Device/DviDevice.h>
#include <OpenHome/Net/Device/DviSubscription.h>
#include <OpenHome/Net/Device/Upnp/DviServerWebSocket.h>
#include <OpenHome/Net/Device/DviService.h>
#include <OpenHome/Net/Device/Bonjour/Bonjour.h>
#include <OpenHome/Net/Device/DviPropertyUpdateCollection.h>
#include <OpenHome/Net/Device/Upnp/DviSsdpNotifier.h>
#include <OpenHome/Standard.h>

#include <vector>

namespace OpenHome {
namespace Net {

class IControlPointObserver
{
public:
    virtual void NotifyControlPoint(const OpenHome::Brx& aControlPoint) = 0; // not allowed to throw
};

class IDvProtocolFactory;

class DvStack : private IStack, private INonCopyable
{
    static const TUint kMaxControlPointBytes = 1024;
public:
    DvStack(Environment& aEnv);
    Environment& Env() { return iEnv; }
    void Start();
    TUint BootId();
    TUint NextBootId();
    void UpdateBootId();
    DviServerUpnp& ServerUpnp();
    DviDeviceMap& DeviceMap();
    DviSubscriptionManager& SubscriptionManager();
    DviPropertyUpdateCollection& PropertyUpdateCollection();
    DviSsdpNotifierManager& SsdpNotifierManager();
    void AddProtocolFactory(IDvProtocolFactory* aProtocolFactory);
    std::vector<IDvProtocolFactory*>& ProtocolFactories();
    void AddControlPointObserver(IControlPointObserver& aObserver);
    void RemoveControlPointObserver(IControlPointObserver& aObserver);
    void NotifyControlPointUsed(const OpenHome::Brx& aControlPoint);
private:
    ~DvStack();
private:
    OpenHome::Environment& iEnv;
    Mutex iLock;
    TUint iBootId;
    TUint iNextBootId;
    DviServerUpnp* iDviServerUpnp;
    DviDeviceMap* iDviDeviceMap;
    DviSubscriptionManager* iSubscriptionManager;
    DviServerWebSocket* iDviServerWebSocket;
    DviPropertyUpdateCollection* iPropertyUpdateCollection;
    DviSsdpNotifierManager* iSsdpNotifierManager;
    std::vector<IDvProtocolFactory*> iProtocolFactories;
    std::vector<IControlPointObserver*> iControlPointObservers;
    Bws<kMaxControlPointBytes> iControlPoint;
};

} // namespace Net
} // namespace OpenHome

#endif // HEADER_DVI_STACK
