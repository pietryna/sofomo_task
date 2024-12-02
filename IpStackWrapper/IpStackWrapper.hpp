#ifndef SOFOMO_IPSTACKWRAPPER_IPSTACKWRAPPER_HPP_
#define SOFOMO_IPSTACKWRAPPER_IPSTACKWRAPPER_HPP_

#include <string>
#include <tuple>
#include <memory>
#include <functional>

#include "CurlWrapperIf.hpp"


class IpStackWrapper {
 public:
    explicit IpStackWrapper(std::unique_ptr<CurlWrapperIf> curl);
    ~IpStackWrapper() = default;

    std::tuple<bool, std::string, std::string> getGeoData(const std::string &ipv4);

 private:
    const std::string apiKey;
    std::function<std::string(std::string)> clearStr;

    std::unique_ptr<CurlWrapperIf> curl;
};

#endif //SOFOMO_IPSTACKWRAPPER_IPSTACKWRAPPER_HPP_
