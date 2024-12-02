#ifndef SOFOMO_DATABASEITEM_HPP
#define SOFOMO_DATABASEITEM_HPP

#include <string>
#include <utility>

class DatabaseItem {
 public:
    DatabaseItem(std::string url, std::string ip, std::string longitude, std::string latitude);
    ~DatabaseItem() = default;

    std::string &
    getUrl();
    std::string &
    getIp();
    std::string &
    getLongitude();
    std::string &
    getLatitude();
 private:
    std::string url;
    std::string ip;
    std::string longitude;
    std::string latitude;
};

#endif //SOFOMO_DATABASEITEM_HPP
