#include <OpenHome/OhNetTypes.h>
#include <OpenHome/Ascii.h>
#include <OpenHome/Thread.h>
// #include <OpenHome/OptionParser.h>
#include <OpenHome/Debug.h>
#include <OpenHome/Net/Core/OhNet.h>
#include <OpenHome/Net/Cpp/CpDevice.h>
#include <OpenHome/Net/Cpp/CpDeviceUpnp.h>
#include <OpenHome/Net/Cpp/FunctorCpDevice.h>
// #include <OpenHome/Net/Bindings/Cpp/ControlPoint/CpDeviceUpnp.h>
// #include <OpenHome/Net/Bindings/Cpp/ControlPoint/FunctorCpDevice.h>
// #include <OpenHome/Net/Bindings/Cpp/ControlPoint/Proxies/CpAvOpenhomeOrgSender1.h>
// #include <OpenHome/Net/Bindings/Cpp/ControlPoint/CpDevice.h>
#include <OpenHome/TIpAddressUtils.h>

#include <OpenHome/Buffer.h>
#include <OpenHome/Timer.h>
#include <OpenHome/Uri.h>
#include <OpenHome/Fifo.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <cstdio>
#include <string>
#include <cassert>

#include <termios.h>
#include <unistd.h>
#define CDECL

// #include "OhmReceiver.h"

#include <OpenHome/Net/XmlParser.h>

using namespace std;

using namespace OpenHome;
using namespace OpenHome::Net;
// using namespace OpenHome::TestFramework;
// using namespace OpenHome::Av;

template <typename... Args>
std::string string_sprintf(const char *format, Args... args)
{
  using namespace std;
  int length = snprintf(nullptr, 0, format, args...);
  assert(length >= 0);

  char *buf = new char[length + 1];
  snprintf(buf, length + 1, format, args...);

  string str(buf);
  delete[] buf;
  return move(str);
}

int mygetch()
{
  struct termios oldt, newt;
  int ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}

class DeviceList
{
  private:
    std::vector<CpDeviceCpp*> iVector;
  public:
    void Added(CpDeviceCpp& aDevice)
    {
      iVector.push_back(&aDevice);
      aDevice.AddRef();
    }
    void Removed(CpDeviceCpp& aDevice)
    {
      const std::string udn = aDevice.Udn();
      for (size_t i=0; i<iVector.size(); i++) {
	if (iVector[i]->Udn() == udn) {
	  iVector[i]->RemoveRef();
	  iVector.erase(iVector.begin() +i);
	}
      }
    }
    CpDeviceCpp& getDevice(size_t index)
    {
      return *iVector[index];
    }
vector<CpDeviceCpp*>& getDevices()
{
return iVector;
}
    ~DeviceList()
    {const TUint count = (TUint)iVector.size();
      for (TUint i=0; i<count; i++) {
	iVector[i]->RemoveRef();
      }
      iVector.clear();
    }
};

int CDECL main(int aArgc, char *aArgv[])
{
  InitialisationParams *initParams = InitialisationParams::Create();
  Environment *env = UpnpLibrary::Initialise(initParams);
  std::vector<NetworkAdapter *> *subnetList = UpnpLibrary::CreateSubnetList();

  cout << "Which one to use?" << endl;
  int offset = mygetch() - '0' - 1;
  TIpAddress subnet = (*subnetList)[offset]->Subnet();
  TIpAddress adapter = (*subnetList)[offset]->Address();
  Library::DestroySubnetList(subnetList);

  TIpAddressUtils::AddressBuf buf;
  TIpAddressUtils::ToString(subnet, buf);
  printf("Using subnet %.*s\n", PBUF(buf));

  // TUint ttl = optionTtl.Value();
  TUint ttl = 1;
  // OhmReceiverDriver *driver = new OhmReceiverDriver();
  // OhmReceiver *receiver = new OhmReceiver(*env, adapter, ttl, *driver);

  UpnpLibrary::StartCp(subnet);

  DeviceList *deviceList = new DeviceList;
  FunctorCpDeviceCpp added = MakeFunctorCpDeviceCpp(*deviceList, &DeviceList::Added);
  FunctorCpDeviceCpp removed = MakeFunctorCpDeviceCpp(*deviceList, &DeviceList::Removed);

  string domain("av.openhome.org");
  string service("Sender");
  CpDeviceListCppUpnpServiceType *list = new CpDeviceListCppUpnpServiceType(domain, service, 1, added,
                                                                            removed);

  for (int i = 0; i < 10; ++i)
  {
    sleep(1);
  }

  auto vec = deviceList->getDevices();
  auto pos = 1;
  for (auto iter = vec.begin(); iter != vec.end(); ++iter, ++pos)
  {
    std::string val;
    (*iter)->GetAttribute("Upnp.FriendlyName", val);
    cout << "Found device #" << pos << ": " << val << endl;
  }
  cout << "Which one to use?" << endl;
  offset = mygetch() - '0' - 1;

  CpProxyAvOpenhomeOrgSender1Cpp *sender = new CpProxyAvOpenhomeOrgSender1Cpp(deviceList->getDevice(offset));
  cout << "Please wait";
  fflush(stdout);

  return 0;
}
