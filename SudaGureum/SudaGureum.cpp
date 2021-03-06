#include "Common.h"

#include "SudaGureum.h"

#include "IrcClient.h"
#include "Utility.h"

namespace SudaGureum
{
    void run()
    {
#ifndef _WIN32
        boost::locale::generator gen;
        std::locale loc = gen("");
        std::locale::global(loc);
#endif

        IrcClientPool pool;

        auto client1 = pool.connect("altirc.ozinger.org", 80, "UTF-8",
            boost::assign::list_of("SudaGureum1")("SudaGureum2"));

        pool.run(4);

        // Sleep(100);

        // auto client2 = pool.connect("irc.ozinger.org", 6667, boost::assign::list_of("SudaGureum1")("SudaGureum2"));

        // pool.join();

        while(std::cin)
        {
            std::string line;
            std::getline(std::cin, line);

            boost::algorithm::trim(line);

            if(line.empty())
            {
                break;
            }

#ifdef _WIN32
            size_t wlen = static_cast<size_t>(MultiByteToWideChar(CP_ACP, 0, line.c_str(), -1, nullptr, 0));
            std::vector<wchar_t> wbuf(wlen);
            MultiByteToWideChar(CP_ACP, 0, line.c_str(), -1, wbuf.data(), static_cast<int>(wlen));

            client1.lock()->privmsg("#HNO3", encodeUtf8(wbuf.data()));
#else
            client1.lock()->privmsg("#HNO3", boost::locale::conv::to_utf<char>(line, std::locale()));
#endif
        }

        pool.closeAll();
        pool.join();
    }
}

int main()
{
    SudaGureum::run();

    return 0;
}
