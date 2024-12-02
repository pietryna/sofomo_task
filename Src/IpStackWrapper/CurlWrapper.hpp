#ifndef SOFOMO_IPSTACKWRAPPER_CURLWRAPPER_HPP_
#define SOFOMO_IPSTACKWRAPPER_CURLWRAPPER_HPP_

#include <string>
#include <tuple>
#include "CurlWrapperIf.hpp"

class CurlWrapper : public CurlWrapperIf {
 public:
    ~CurlWrapper() override = default;
    std::pair<bool, std::string> getHttpRequest(const std::string &request) override;

 private:
    static size_t curlWriteCallback(void *contents, size_t size, size_t nmemb, std::string *userData);
};

#endif //SOFOMO_IPSTACKWRAPPER_CURLWRAPPER_HPP_
