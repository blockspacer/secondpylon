#include <secondpylon/diag/diag_log.h>
#include <unittest++/src/UnitTest++.h>

using namespace secondpylon;

namespace 
{
    class StoringLogListener : public diag::ILogListener
    {
        enum { kStorageSize = 80 };
    public:
        StoringLogListener()
        {
            m_Message[0] = '\0';
        }

        virtual void OnMessage(const char* message)
        {
            strncpy(m_Message, message, kStorageSize);
            m_Message[kStorageSize-1] = '\0';
        }

        const char* GetMessage() const { return m_Message; }
    private:
        char m_Message[kStorageSize];
    };
}

TEST(UncopyableSize)
{
    CHECK_EQUAL(sizeof(plat::uint32), sizeof(secondpylon::diag::ILogListener));
}

//TEST(LogInit)
//{
//    diag::Log testLog;
//    testLog.Print("test");
//}

TEST(LogAddListener)
{
    const char* msg = "test";

    diag::Log testLog;
    StoringLogListener listener;
    testLog.AddListener(listener);
    testLog.Message(msg);
    testLog.RemoveListener(listener);

    CHECK_EQUAL(msg, listener.GetMessage());
}