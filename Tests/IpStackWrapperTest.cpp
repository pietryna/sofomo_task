#include <gtest/gtest.h>

#include "../IpStackWrapper/IpStackWrapper.hpp"
#include "CurlMock.h"

using testing::Return;

class IpStackWrapperTest : public testing::Test {
 protected:
    IpStackWrapperTest() {
        auto tmpMock = std::make_unique<CurlMock>();
        curlMock = tmpMock.get();
        ipStackWrapper = std::make_unique<IpStackWrapper>(std::move(tmpMock));
    }
    std::unique_ptr<IpStackWrapper> ipStackWrapper;
    CurlMock *curlMock;

    std::string ip{"1.2.3.4"};
    std::string m_longitude{"21.01219940185547"}, m_latitude{"52.229698181152344"};
    const char *testOkVal{"{\"ip\": \"1.2.3.4\", \"type\": \"ipv4\", \"continent_code\": \"EU\", "
                          "\"continent_name\": \"Europe\", \"country_code\": \"PL\", \"country_name\": \"Poland\", "
                          "\"region_code\": \"MZ\", \"region_name\": \"Mazovia\", \"city\": \"Warsaw\", \"zip\": "
                          "\"00-025\", \"latitude\": 52.229698181152344, \"longitude\": 21.01219940185547, \"msa\": null, "
                          "\"dma\": null, \"radius\": null, \"ip_routing_type\": \"fixed\", \"connection_type\": \"tx\", "
                          "\"location\": {\"geoname_id\": 756135, \"capital\": \"Warsaw\", \"languages\": [{\"code\": "
                          "\"pl\", \"name\": \"Polish\", \"native\": \"Polski\"}], \"country_flag\": "
                          "\"https://assets.ipstack.com/flags/pl.svg\", \"country_flag_emoji\": "
                          "\"\\ud83c\\uddf5\\ud83c\\uddf1\", \"country_flag_emoji_unicode\": \"U+1F1F5 U+1F1F1\", "
                          "\"calling_code\": \"48\", \"is_eu\": true}}"};
    std::string testErrorVal
        {"{\n  \"success\": false,\n  \"error\": {\n    \"code\": 101,\n    \"type\": \"invalid_access_key\",\n    "
         "\"info\": \"You have not supplied a valid API Access Key. [Technical Support: support@apilayer.com]\"\n  "
         "}\n}\n"};
};

TEST_F(IpStackWrapperTest, incorrectCurlResponse) {
    std::string request{"http://api.ipstack.com/" + ip + "?access_key=XXX"};
    EXPECT_CALL(*curlMock, getHttpRequest(request)).WillOnce(Return(std::pair<bool, std::string>{false, ""}));
    auto [result, longitude, latitude] = ipStackWrapper->getGeoData(ip);
    ASSERT_EQ(false, result);
    ASSERT_EQ("", longitude);
    ASSERT_EQ("", latitude);
}

TEST_F(IpStackWrapperTest, correctIpStackResponse) {
    std::string request{"http://api.ipstack.com/" + ip + "?access_key=XXX"};
    EXPECT_CALL(*curlMock, getHttpRequest(request)).WillOnce(Return(std::pair<bool, std::string>{true, testOkVal}));
    auto [result, longitude, latitude] = ipStackWrapper->getGeoData(ip);
    ASSERT_EQ(true, result);
    ASSERT_EQ(m_longitude, longitude);
    ASSERT_EQ(m_latitude, latitude);
}

TEST_F(IpStackWrapperTest, incorrectIpStackResponse) {
    std::string request{"http://api.ipstack.com/" + ip + "?access_key=XXX"};
    EXPECT_CALL(*curlMock, getHttpRequest(request)).WillOnce(Return(std::pair<bool, std::string>{true, testErrorVal}));
    auto [result, longitude, latitude] = ipStackWrapper->getGeoData(ip);
    ASSERT_EQ(false, result);
    ASSERT_EQ("", longitude);
    ASSERT_EQ("", latitude);
}