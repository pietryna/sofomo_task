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

    IpStackWrapper(const IpStackWrapper& wrapper) = delete;
    IpStackWrapper& operator=(const IpStackWrapper& wrapper) = delete;

    IpStackWrapper(IpStackWrapper&& wrapper) noexcept;
    IpStackWrapper& operator=(IpStackWrapper&& wrapper) noexcept ;

    std::tuple<bool, std::string, std::string> getGeoData(const std::string &ipv4);
    void setApiKey(const std::string& key);

 private:
    std::string apiKey{"empty"};
    std::function<std::string(std::string)> clearStr;

    std::unique_ptr<CurlWrapperIf> curl;
};

#endif //SOFOMO_IPSTACKWRAPPER_IPSTACKWRAPPER_HPP_
