#include "SoundcardDriver.h"
#include "../Soundcard.h"
#include "Icon.h"

#include <Ascii.h>
#include <Maths.h>
#include <Arch.h>
#include <Debug.h>

#include <Setupapi.h>
#include <ks.h>
#include <ksmedia.h>
#include <initguid.h>
#include <Shellapi.h>

EXCEPTION(SoundcardError);

using namespace OpenHome;
using namespace OpenHome::Net;

// OhmSenderDriverWindows

DEFINE_GUID(OHSOUNDCARD_GUID, 0x2685C863, 0x5E57, 0x4D9A, 0x86, 0xEC, 0x2E, 0xC9, 0xB7, 0xBB, 0xBC, 0xFD);

static const TUint KSPROPERTY_OHSOUNDCARD_VERSION = 0;
static const TUint KSPROPERTY_OHSOUNDCARD_ENABLED = 1;
static const TUint KSPROPERTY_OHSOUNDCARD_ACTIVE = 2;
static const TUint KSPROPERTY_OHSOUNDCARD_ENDPOINT = 3;
static const TUint KSPROPERTY_OHSOUNDCARD_TTL = 4;

OhmSenderDriverWindows::OhmSenderDriverWindows()
{
	if (!FindDriver()) {
		if (!InstallDriver()) {
			THROW(SoundcardError);
		}
		if (!FindDriver()) {
			THROW(SoundcardError);
		}
	}
}

TBool OhmSenderDriverWindows::InstallDriver()
{
	SHELLEXECUTEINFO shellExecuteInfo;

    shellExecuteInfo.cbSize  = sizeof(SHELLEXECUTEINFO);
    shellExecuteInfo.lpFile = "Install.exe";
	shellExecuteInfo.fMask = SEE_MASK_NOCLOSEPROCESS;     
    shellExecuteInfo.hwnd = NULL;  
    shellExecuteInfo.lpVerb = "open";
    shellExecuteInfo.lpParameters = NULL;
    shellExecuteInfo.lpDirectory = NULL;   
    shellExecuteInfo.nShow = SW_HIDE;
    shellExecuteInfo.hInstApp = 0;

    ShellExecuteEx(&shellExecuteInfo);
     
	int ret = (int)shellExecuteInfo.hInstApp;

	if (ret <= 32) {
		printf("Error %d\n", ret);
		return (false);
	}

	if (shellExecuteInfo.hProcess ==NULL)
	{
		printf("No process\n", ret);
		return (false);
	}

	WaitForSingleObject(shellExecuteInfo.hProcess, INFINITE);

    CloseHandle(shellExecuteInfo.hProcess);

	return (true);
}

/*






	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInformation;

	startupInfo.cb = sizeof(STARTUPINFO);
	startupInfo.lpReserved = NULL;
	startupInfo.lpDesktop = NULL;
	startupInfo.lpTitle = NULL;
	startupInfo.dwX = 0;
	startupInfo.dwY = 0;
	startupInfo.dwXSize = 0;
	startupInfo.dwYSize = 0;
	startupInfo.dwXCountChars = 0;
	startupInfo.dwYCountChars = 0;
	startupInfo.dwFillAttribute = 0;
	startupInfo.dwFlags = 0;
	startupInfo.wShowWindow = false;
	startupInfo.cbReserved2 = 0;
	startupInfo.lpReserved2 = NULL;
	startupInfo.hStdInput = 0;
	startupInfo.hStdOutput = 0;
	startupInfo.hStdError = 0;

	if (!CreateProcess("Install.exe", NULL, NULL, NULL, false, CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInformation))
	{
		printf("create failed %d\n", GetLastError());
		return (false);
	}

	WaitForSingleObject(processInformation.hProcess, INFINITE);

	CloseHandle(processInformation.hProcess );
	CloseHandle(processInformation.hThread );

	return (true);
}
*/


TBool OhmSenderDriverWindows::FindDriver()
{
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&KSCATEGORY_AUDIO, 0, 0, DIGCF_DEVICEINTERFACE | DIGCF_PROFILE | DIGCF_PRESENT);

	if (deviceInfoSet == 0) {
		return (false);
	}

	SP_DEVICE_INTERFACE_DATA deviceInterfaceData;

	deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	// build a DevInfo Data structure

	SP_DEVINFO_DATA deviceInfoData;

	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		 
	// build a Device Interface Detail Data structure

	TByte* detail = new TByte[1000];

	PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)detail;

	deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	for (TUint i = 0; SetupDiEnumDeviceInterfaces(deviceInfoSet, 0, &KSCATEGORY_AUDIO, i, &deviceInterfaceData); i++)
	{
		// now we can get some more detailed information

		if (SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, deviceInterfaceDetailData, 1000, 0, &deviceInfoData))
		{
			try
			{
				iHandle = CreateFile(deviceInterfaceDetailData->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

                KSPROPERTY prop;
				
				prop.Set = OHSOUNDCARD_GUID;
                prop.Id = KSPROPERTY_OHSOUNDCARD_VERSION;
                prop.Flags = KSPROPERTY_TYPE_GET;

                TByte buffer[4];

                DWORD bytes;

                if (DeviceIoControl(iHandle, IOCTL_KS_PROPERTY, &prop, sizeof(KSPROPERTY), buffer, sizeof(buffer), &bytes, 0))
                {
					TUint version = *(TUint*)buffer;

					if (version == 1) {
						delete [] detail;
						SetupDiDestroyDeviceInfoList(deviceInfoSet);
						return (true);
					}
                }
			}
			catch (...)
			{
			}
		}
	}

	delete [] detail;

	SetupDiDestroyDeviceInfoList(deviceInfoSet);

	return (false);
}

void OhmSenderDriverWindows::SetEnabled(TBool aValue)
{
    KSPROPERTY prop;
				
	prop.Set = OHSOUNDCARD_GUID;
    prop.Id = KSPROPERTY_OHSOUNDCARD_ENABLED;
    prop.Flags = KSPROPERTY_TYPE_SET;

    DWORD bytes;

	DWORD value = aValue ? 1 : 0;

    DeviceIoControl(iHandle, IOCTL_KS_PROPERTY, &prop, sizeof(KSPROPERTY), &value, sizeof(value), &bytes, 0);
}

void OhmSenderDriverWindows::SetActive(TBool  aValue)
{
    KSPROPERTY prop;
				
	prop.Set = OHSOUNDCARD_GUID;
    prop.Id = KSPROPERTY_OHSOUNDCARD_ACTIVE;
    prop.Flags = KSPROPERTY_TYPE_SET;

    DWORD bytes;

	DWORD value = aValue ? 1 : 0;

    DeviceIoControl(iHandle, IOCTL_KS_PROPERTY, &prop, sizeof(KSPROPERTY), &value, sizeof(value), &bytes, 0);
}

void OhmSenderDriverWindows::SetEndpoint(const Endpoint& aEndpoint)
{
	KSPROPERTY prop;
				
	prop.Set = OHSOUNDCARD_GUID;
    prop.Id = KSPROPERTY_OHSOUNDCARD_ENDPOINT;
    prop.Flags = KSPROPERTY_TYPE_SET;

	TByte buffer[8];

	ULONG* ptr = (ULONG*)buffer;
	
	*ptr++ = Arch::BigEndian4(aEndpoint.Address());
	*ptr++ = aEndpoint.Port();

    DWORD bytes;

    DeviceIoControl(iHandle, IOCTL_KS_PROPERTY, &prop, sizeof(KSPROPERTY), &buffer, sizeof(buffer), &bytes, 0);
}


void OhmSenderDriverWindows::SetTtl(TUint aValue)
{
    KSPROPERTY prop;
				
	prop.Set = OHSOUNDCARD_GUID;
    prop.Id = KSPROPERTY_OHSOUNDCARD_TTL;
    prop.Flags = KSPROPERTY_TYPE_SET;

    DWORD bytes;

    DeviceIoControl(iHandle, IOCTL_KS_PROPERTY, &prop, sizeof(KSPROPERTY), &aValue, sizeof(aValue), &bytes, 0);
}

void OhmSenderDriverWindows::SetTrackPosition(TUint64 /*aSamplesTotal*/, TUint64 /*aSampleStart*/)
{
}


// Soundcard

Soundcard* Soundcard::Create(TIpAddress aSubnet, TUint aChannel, TUint aTtl, TBool aMulticast, TBool aEnabled, TUint aPreset, ReceiverCallback aReceiverCallback, void* aReceiverPtr, SubnetCallback aSubnetCallback, void* aSubnetPtr)
{
	try {
		Soundcard* soundcard = new Soundcard(aSubnet, aChannel, aTtl, aMulticast, aEnabled, aPreset, aReceiverCallback, aReceiverPtr, aSubnetCallback, aSubnetPtr);
		return (soundcard);
	}
	catch (SoundcardError) {
	}

	return (0);
}

Soundcard::Soundcard(TIpAddress aSubnet, TUint aChannel, TUint aTtl, TBool aMulticast, TBool aEnabled, TUint aPreset, ReceiverCallback aReceiverCallback, void* aReceiverPtr, SubnetCallback aSubnetCallback, void* aSubnetPtr)
	: iMutex("SCRD")
	, iClosing(false)
	, iSubnet(aSubnet)
	, iAdapter(0)
	, iSender(0)
	, iReceiverCallback(aReceiverCallback)
	, iReceiverPtr(aReceiverPtr)
	, iSubnetCallback(aSubnetCallback)
	, iSubnetPtr(aSubnetPtr)
{
	Debug::SetLevel(Debug::kMedia);
	

	// First do everything that might throw SoundcardError: get the computer name, and create the driver,
	// which links to the installed windows ohSoundcard audio driver and throws 
	Bws<kMaxUdnBytes> computer;
	Bws<kMaxUdnBytes> udn;
	Bws<kMaxUdnBytes + 1> friendly;

	TUint bytes = computer.MaxBytes();

	if (!GetComputerName((LPSTR)computer.Ptr(), (LPDWORD)&bytes)) {
		THROW(SoundcardError);
	}

	computer.SetBytes(bytes);
	udn.Replace("ohSoundcard-");
	udn.Append(computer);
	friendly.Replace(udn);
	friendly.Append('\0');

    iDriver = new OhmSenderDriverWindows();

	InitialisationParams* initParams = InitialisationParams::Create();

	Functor callback = MakeFunctor(*this, &Soundcard::SubnetListChanged);

	initParams->SetSubnetListChangedListener(callback);

	UpnpLibrary::Initialise(initParams);

	// Fixes bug in stack
	if (iSubnet == 0) {
		SubnetListChanged();
		iSubnet = iSubnetList[0]->Address();
		iAdapter = iSubnetList[0]->AdapterAddress();
	}
	/////////////////////

	UpnpLibrary::StartCombined(iSubnet);

	iDevice = new DvDeviceStandard(udn);
    
	iDevice->SetAttribute("Upnp.Domain", "av.openhome.org");
    iDevice->SetAttribute("Upnp.Type", "Soundcard");
    iDevice->SetAttribute("Upnp.Version", "1");
    iDevice->SetAttribute("Upnp.FriendlyName", (TChar*)friendly.Ptr());
    iDevice->SetAttribute("Upnp.Manufacturer", "Openhome");
    iDevice->SetAttribute("Upnp.ManufacturerUrl", "http://www.openhome.org");
    iDevice->SetAttribute("Upnp.ModelDescription", "OpenHome Soundcard");
    iDevice->SetAttribute("Upnp.ModelName", "ohSoundcard");
    iDevice->SetAttribute("Upnp.ModelNumber", "1");
    iDevice->SetAttribute("Upnp.ModelUrl", "http://www.openhome.org");
    iDevice->SetAttribute("Upnp.SerialNumber", "");
    iDevice->SetAttribute("Upnp.Upc", "");

	SubnetListChanged();

	Brn icon(icon_png, icon_png_len);

	iSender = new OhmSender(*iDevice, *iDriver, computer, aChannel, iAdapter, aTtl, aMulticast, aEnabled, icon, Brn("image/png"), aPreset);
	
	iDevice->SetEnabled();

	iReceiverManager = new ReceiverManager3(*this, iSender->SenderUri(), iSender->SenderMetadata());
}


