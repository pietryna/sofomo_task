#include "IpStackWrapper.hpp"

#include <QDebug>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

IpStackWrapper::IpStackWrapper() : apiKey("XXX") {
    clearStr = [](std::string in) -> std::string {
        in.erase(
            std::remove(in.begin(), in.end(), '\"'),
            in.end()
        );
        return in;
    };
}

std::tuple<bool, std::string, std::string> IpStackWrapper::getGeoData(const std::string &ipv4) {
    bool result = true;

    const char *testOkVal{"{\"ip\": \"162.158.103.108\", \"type\": \"ipv4\", \"continent_code\": \"EU\", "
                          "\"continent_name\": \"Europe\", \"country_code\": \"PL\", \"country_name\": \"Poland\", "
                          "\"region_code\": \"MZ\", \"region_name\": \"Mazovia\", \"city\": \"Warsaw\", \"zip\": "
                          "\"00-025\", \"latitude\": 52.229698181152344, \"longitude\": 21.01219940185547, \"msa\": null, "
                          "\"dma\": null, \"radius\": null, \"ip_routing_type\": \"fixed\", \"connection_type\": \"tx\", "
                          "\"location\": {\"geoname_id\": 756135, \"capital\": \"Warsaw\", \"languages\": [{\"code\": "
                          "\"pl\", \"name\": \"Polish\", \"native\": \"Polski\"}], \"country_flag\": "
                          "\"https://assets.ipstack.com/flags/pl.svg\", \"country_flag_emoji\": "
                          "\"\\ud83c\\uddf5\\ud83c\\uddf1\", \"country_flag_emoji_unicode\": \"U+1F1F5 U+1F1F1\", "
                          "\"calling_code\": \"48\", \"is_eu\": true}}"};
//    std::string testErrorVAl
//        {"{\n  \"success\": false,\n  \"error\": {\n    \"code\": 101,\n    \"type\": \"invalid_access_key\",\n    "
//         "\"info\": \"You have not supplied a valid API Access Key. [Technical Support: support@apilayer.com]\"\n  "
//         "}\n}\n"};
//
//    json ok = json::parse(testOkVal);
//    json nok = json::parse(testErrorVAl);
//    qInfo() << "IP: " << std::string(ok["ip"].dump(0)) << "Long: " << ok["longitude"].dump(0) << "Lat: " <<
//    ok["latitude"].dump(0);
//    bool success = ("true" == nok["success"].dump(0));
//    qInfo() << "Success: " << success;


    CURL *curl;
    CURLcode curlResp;
    std::string request{"http://api.ipstack.com/" + ipv4 + "?access_key=" + apiKey};
    std::string response{};

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, IpStackWrapper::curlWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curlResp = curl_easy_perform(curl);
        if (curlResp != CURLE_OK) {
            qInfo() << "CURL error: " << curl_easy_strerror(curlResp);
            result = false;
        } else {
            // Print the response
            qInfo() << "Raw response: " << response;
        }
        curl_easy_cleanup(curl);
    }
//    json jResp = json::parse(response);
    json jResp = json::parse(testOkVal);
    std::string longitude{}, latitude{};
    if (jResp["success"].dump(0) == "null") {
        qInfo() << "IP: " << clearStr(jResp["ip"].dump(0)) << "Long: " << clearStr(jResp["longitude"].dump(0))
                << "Lat: " << clearStr(jResp["latitude"].dump(0));
        longitude = clearStr(jResp["longitude"].dump(0));
        latitude = clearStr(jResp["latitude"].dump(0));
    } else {
        qInfo() << "Failed ipstack request: " << clearStr(jResp["error"]["info"].dump(0));
        result = false;
    }
    return {result, longitude, latitude};
}
size_t IpStackWrapper::curlWriteCallback(void *contents, size_t size, size_t nmemb, std::string *userData) {
    size_t totalSize = size * nmemb;
    userData->append((char *) contents, totalSize);
    return totalSize;
}
