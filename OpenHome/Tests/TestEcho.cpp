#include <OpenHome/Standard.h>
#include <OpenHome/Thread.h>
#include <OpenHome/Network.h>
#include <OpenHome/TestFramework/TestFramework.h>
#include <OpenHome/Debug.h>
#include <OpenHome/Stream.h>
#include <Os/OsWrapper.h>
#include <OpenHome/Net/Globals.h>

using namespace OpenHome;
using namespace OpenHome::TestFramework;

class EchoSession : public SocketTcpSession
{
public:
    EchoSession(Semaphore& aSem)
        : iSem(aSem)
    {
        iStream = new Srs<200>(*this);
        iReaderUntil = new ReaderUntilS<200>(*iStream);
    }
    
    ~EchoSession()
    {
        delete iReaderUntil;
        delete iStream;
    }

    void Run()
    {
        const Brn exitUnix("exit");
        const Brn exitDos("exit\r");
        TBool done = false;
        while (!done) {
            try {
                Brn buf = iReaderUntil->ReadUntil('\n');
                Write(buf);
                Write('\n');
                if (buf.Equals(exitUnix) || buf.Equals(exitDos))
                    done = true;
            }
            catch (ReaderError&) {
                break;
            }
        }
        iSem.Signal();
    }

private:
    Srx* iStream;
    ReaderUntil* iReaderUntil;
    Semaphore& iSem;
};


void OpenHome::TestFramework::Runner::Main(TInt /*aArgc*/, TChar* /*aArgv*/[], Net::InitialisationParams* aInitParams)
{
    Net::UpnpLibrary::Initialise(aInitParams);
//    Debug::SetLevel(Debug::kNetwork);

    std::vector<NetworkAdapter*>* ifs = Os::NetworkListAdapters(*gEnv, Environment::ELoopbackExclude, false/* no ipv6*/, "TestEcho");
    ASSERT(ifs->size() > 0);
    TIpAddress addr = (*ifs)[0]->Address();
    Endpoint endpt(0, addr);
    Endpoint::AddressBuf buf;
    endpt.AppendAddress(buf);
    for (TUint i=0; i<ifs->size(); i++) {
        (*ifs)[i]->RemoveRef("TestEcho");
    }
    Print("Using network interface %s\n\n", buf.Ptr());
    Semaphore sem("", 0);
    SocketTcpServer* server = new SocketTcpServer(*gEnv, "ECHO", 1025, addr);
    server->Add("ECHO", new EchoSession(sem));

    sem.Wait();
    delete ifs;

    Net::UpnpLibrary::Close();
}

