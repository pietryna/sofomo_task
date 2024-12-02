#ifndef SOFOMO_IPSTACKWRAPPER_CURLWRAPPERIF_HPP_
#define SOFOMO_IPSTACKWRAPPER_CURLWRAPPERIF_HPP_

#include <string>
#include <tuple>

class CurlWrapperIf {
 public:
    virtual ~CurlWrapperIf() = default;
    virtual std::pair<bool, std::string> getHttpRequest(const std::string& request) = 0;
};
#endif //SOFOMO_IPSTACKWRAPPER_CURLWRAPPERIF_HPP_
