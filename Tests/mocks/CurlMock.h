#ifndef SOFOMO_TESTS_MOCKS_CURL_H_
#define SOFOMO_TESTS_MOCKS_CURL_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../IpStackWrapper/CurlWrapperIf.hpp"

class CurlMock: public CurlWrapperIf {
 public:
    ~CurlMock() override = default;
    MOCK_METHOD((std::pair<bool, std::string>), getHttpRequest, (const std::string& request), (override));
};

#endif //SOFOMO_TESTS_MOCKS_CURL_H_
