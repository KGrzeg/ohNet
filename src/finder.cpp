#include <OpenHome/OhNetTypes.h>
#include <OpenHome/Ascii.h>
#include <OpenHome/Thread.h>
#include <OpenHome/TestFramework/OptionParser.h>
#include <OpenHome/Debug.h>
#include <OpenHome/Net/OhNet.h>
#include <OpenHome/Net/Bindings/Cpp/ControlPoint/FunctorCpDevice.h>
#include <OpenHome/Net/Bindings/Cpp/ControlPoint/CpDevice.h>
#include <OpenHome/Net/Bindings/Cpp/ControlPoint/CpDeviceUpnp.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>

#include <OpenHome/Net/XmlParser.h>

#ifdef _WIN32

#define snprintf _snprintf

#pragma warning(disable:4355) // use of 'this' in ctor lists safe in this case

#define CDECL __cdecl

#include <windows.h>
#include <conio.h>

int mygetch()
{
  return (_getch());
}

void sleep(int seconds)
{
Sleep(seconds * 1000);
}

#else

#define CDECL

#include <termios.h>
#include <unistd.h>

int mygetch()
{
  struct termios oldt, newt;
  int ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}

#endif

using namespace std;

using namespace OpenHome;
using namespace OpenHome::Net;
using namespace OpenHome::TestFramework;

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

int CDECL main(int aArgc, char* aArgv[])
{
  OptionParser parser;

  OptionUint optionAdapter("-a", "--adapter", 0, "[adapter] index of network adapter to use");
  parser.AddOption(&optionAdapter);

  OptionUint optionTtl("-t", "--ttl", 1, "[ttl] ttl");
  parser.AddOption(&optionTtl);

  OptionString optionUri("-u", "--uri", Brn("mpus://0.0.0.0:0"), "[uri] uri of the sender");
  parser.AddOption(&optionUri);

  OptionString optionFilename("-f", "--filename", Brn("audio"), "[filename] desired output file name");
  parser.AddOption(&optionFilename);

  OptionBool optionFlac("-F", "--flac", "[flac] output to FLAC (default is WAV)");
  parser.AddOption(&optionFlac);

  if (!parser.Parse(aArgc, aArgv)) {
    return (1);
  }

  InitialisationParams* initParams = InitialisationParams::Create();

  Environment* env = UpnpLibrary::Initialise(initParams);

  std::vector<NetworkAdapter*>* subnetList = UpnpLibrary::CreateSubnetList();
int pos = 1;
for (auto iter = subnetList->begin(); iter != subnetList->end(); ++iter, ++pos)
{
  char *full_name = (*iter)->FullName();
cout << "Found adapter #" << pos << ": " << full_name << endl;
  free(full_name);
}
cout << "Which one to use?" << endl;
  int offset = mygetch()-'0'-1;
  TIpAddress subnet = (*subnetList)[offset]->Subnet();
  TIpAddress adapter = (*subnetList)[offset]->Address();
  Library::DestroySubnetList(subnetList);

  //printf("Using subnet %d.%d.%d.%d\n", subnet&0xff, (subnet>>8)&0xff, (subnet>>16)&0xff, (subnet>>24)&0xff);

  TUint ttl = optionTtl.Value();

  UpnpLibrary::StartCp(subnet);

  DeviceList* deviceList = new DeviceList;
  FunctorCpDeviceCpp added = MakeFunctorCpDeviceCpp(*deviceList, &DeviceList::Added);
  FunctorCpDeviceCpp removed = MakeFunctorCpDeviceCpp(*deviceList, &DeviceList::Removed);

  string domain("av.openhome.org");
  string service("Sender");
  CpDeviceListCppUpnpServiceType* list = new
    CpDeviceListCppUpnpServiceType(domain, service, 1, added,
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
  offset = mygetch()-'0'-1;

  return 0;
  }
