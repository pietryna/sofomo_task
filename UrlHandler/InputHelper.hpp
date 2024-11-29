#ifndef SOFOMO_URLHANDLER_INPUTHELPER_HPP_
#define SOFOMO_URLHANDLER_INPUTHELPER_HPP_

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <tuple>

class InputHelper {
 public:
    static std::pair<bool,std::string> resolveUrl(const std::string& url);
    static std::string extractDomain(const std::string& url);

 private:
    static std::pair<bool,std::string> resolveDomain(const std::string& domain);

};

#endif //SOFOMO_URLHANDLER_INPUTHELPER_HPP_
