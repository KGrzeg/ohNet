#include <iostream>
#include <memory>

#include <OpenHome/Net/OhNet.h>
#include <OpenHome/Net/ControlPoint/CpDevice.h>
#include <OpenHome/Net/ControlPoint/CpDeviceUpnp.h>
#include <OpenHome/Net/ControlPoint/FunctorCpDevice.h>
#include <OpenHome/Net/ControlPoint/Proxies/CpAvOpenhomeOrgSender1.h>

#include <OpenHome/Net/XmlParser.h>

using namespace std;
using namespace OpenHome;
using namespace OpenHome::Net;

class DeviceList
{
  private:
    std::vector<CpDeviceCpp*> iVector;
  public:
    void Added(CpDeviceCpp& aDevice)
    {
      cout << aDevice.Udn() << endl;
      iVector.push_back(&aDevice);
      aDevice.AddRef();
    }
    void Removed(CpDeviceCpp& aDevice)
    {
      std::string udn = aDevice.Udn();
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
    ~DeviceList()
    {const TUint count = (TUint)iVector.size();
      for (TUint i=0; i<count; i++) {
	iVector[i]->RemoveRef();
      }
      iVector.clear();
    }
};

int main()
{
  InitialisationParams* initParams = InitialisationParams::Create();

  UpnpLibrary::Initialise(initParams);
  std::vector<NetworkAdapter*>* subnetList = UpnpLibrary::CreateSubnetList();
  TIpAddress subnet = (*subnetList)[0]->Subnet();
  UpnpLibrary::DestroySubnetList(subnetList);
  UpnpLibrary::StartCp(subnet);

  DeviceList* deviceList = new DeviceList;
  FunctorCpDeviceCpp added = MakeFunctorCpDeviceCpp(*deviceList, &DeviceList::Added);
  FunctorCpDeviceCpp removed = MakeFunctorCpDeviceCpp(*deviceList, &DeviceList::Removed);

  string domain("av.openhome.org");
  string service("Sender");
  CpDeviceListCppUpnpServiceType* list = new
    CpDeviceListCppUpnpServiceType(domain, service, 1, added,
	removed);

  for (int i = 0; i < 5; ++i)
  {
    sleep(1);
  }

  CpProxyAvOpenhomeOrgSender1Cpp* sender = new
    CpProxyAvOpenhomeOrgSender1Cpp(deviceList->getDevice(0));

  sender->Subscribe();

  sleep(3);

  string metadata;

  sender->PropertyMetadata(metadata);
  sender->Unsubscribe();

  cout << "Metadata" << endl << metadata.c_str() << endl;

  Brn xmlBuffer(metadata.c_str());
  Brn res = XmlParserBasic::Find("res", xmlBuffer);

  string uri((const char*)res.Ptr(), res.Bytes());

  cout << "Uri: " << uri << endl;

  delete sender;
  delete list;
  delete deviceList;

  UpnpLibrary::Close();

  return 0;
}
