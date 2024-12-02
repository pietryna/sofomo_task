#include "CurlWrapper.hpp"
#include <curl/curl.h>
#include <QDebug>

std::pair<bool, std::string> CurlWrapper::getHttpRequest(const std::string& request) {
    bool result = true;
    std::string response{};

    CURL *curl;
    CURLcode curlResp;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrapper::curlWriteCallback);
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
    return {result, response};
}

size_t CurlWrapper::curlWriteCallback(void *contents, size_t size, size_t nmemb, std::string *userData) {
    size_t totalSize = size * nmemb;
    userData->append((char *) contents, totalSize);
    return totalSize;
}
