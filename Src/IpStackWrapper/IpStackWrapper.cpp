#include "IpStackWrapper.hpp"

#include <QDebug>
#include <nlohmann/json.hpp>
#include <utility>

using json = nlohmann::json;

IpStackWrapper::IpStackWrapper(std::unique_ptr<CurlWrapperIf> curl) : curl(std::move(curl)) {
    clearStr = [](std::string in) -> std::string {
        in.erase(
            std::remove(in.begin(), in.end(), '\"'),
            in.end()
        );
        return in;
    };
}
IpStackWrapper::IpStackWrapper(IpStackWrapper &&wrapper) noexcept {
    apiKey = std::move(wrapper.apiKey);
    clearStr = std::move(wrapper.clearStr);
    curl = std::move(wrapper.curl);
}
IpStackWrapper &IpStackWrapper::operator=(IpStackWrapper &&wrapper) noexcept {
    if (this != &wrapper) {
        apiKey = std::move(wrapper.apiKey);
        clearStr = std::move(wrapper.clearStr);
        curl = std::move(wrapper.curl);
    }
    return *this;
}

std::tuple<bool, std::string, std::string> IpStackWrapper::getGeoData(const std::string &ipv4) {
    std::string request{"http://api.ipstack.com/" + ipv4 + "?access_key=" + apiKey};

    auto [result, response] = curl->getHttpRequest(request);
    std::string longitude{}, latitude{};
    try {
        json jResp = json::parse(response);
        if (jResp["success"].dump(0) == "null") {
            qInfo() << "IP: " << clearStr(jResp["ip"].dump(0)) << "Long: " << clearStr(jResp["longitude"].dump(0))
                    << "Lat: " << clearStr(jResp["latitude"].dump(0));
            longitude = clearStr(jResp["longitude"].dump(0));
            latitude = clearStr(jResp["latitude"].dump(0));
        } else {
            qInfo() << "Failed ipstack request: " << clearStr(jResp["error"]["info"].dump(0));
            result = false;
        }
    } catch (json::exception &e) {
        qInfo() << "Response parsing exception:" << e.what();
    }
    return {result, longitude, latitude};
}
void IpStackWrapper::setApiKey(const std::string &key) {
    apiKey = key;
}
