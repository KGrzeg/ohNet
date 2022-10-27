#include <OpenHome/OhNetTypes.h>
#include <OpenHome/Ascii.h>
#include <OpenHome/Thread.h>
// #include <OpenHome/OptionParser.h>
#include <OpenHome/Debug.h>
#include <OpenHome/Net/OhNet.h>
#include <OpenHome/Net/ControlPoint/CpDevice.h>
#include <OpenHome/Net/ControlPoint/CpDeviceUpnp.h>
#include <OpenHome/Net/ControlPoint/FunctorCpDevice.h>
#include <OpenHome/Net/ControlPoint/Proxies/CpAvOpenhomeOrgSender1.h>
#include <OpenHome/TIpAddressUtils.h>

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
  /* TUint ttl = 1;
  OhmReceiverDriver *driver = new OhmReceiverDriver();
  OhmReceiver *receiver = new OhmReceiver(*env, adapter, ttl, *driver);

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
  pos = 1;
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
  fflush(stdout); */

  return 0;
}
