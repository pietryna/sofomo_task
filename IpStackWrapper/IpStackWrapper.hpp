#ifndef SOFOMO_IPSTACKWRAPPER_IPSTACKWRAPPER_HPP_
#define SOFOMO_IPSTACKWRAPPER_IPSTACKWRAPPER_HPP_

#include <string>
#include <tuple>
#include <memory>
#include <functional>

#include <curl/curl.h>


class IpStackWrapper {
 public:
    IpStackWrapper();
    ~IpStackWrapper() = default;

    std::tuple<bool, std::string, std::string> getGeoData(const std::string &ipv4);

 private:
    static size_t curlWriteCallback(void *contents, size_t size, size_t nmemb, std::string *userData);
    const std::string apiKey;
    std::function<std::string(std::string)> clearStr;
};

#endif //SOFOMO_IPSTACKWRAPPER_IPSTACKWRAPPER_HPP_
