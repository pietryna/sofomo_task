#include "DatabaseItem.hpp"

#include <utility>

DatabaseItem::DatabaseItem(std::string url, std::string ip, std::string longitude, std::string latitude)
    : url(std::move(url)), ip(std::move(ip)), longitude(std::move(longitude)),
      latitude(std::move(latitude)) {}

std::string &
DatabaseItem::getUrl() {
    return url;
}
std::string &
DatabaseItem::getIp() {
    return ip;
}
std::string &
DatabaseItem::getLongitude() {
    return longitude;
}
std::string &
DatabaseItem::getLatitude() {
    return latitude;
}