#include "InputHelper.hpp"
#include <netdb.h>

#include <QDebug>

std::pair<bool, std::string> InputHelper::resolveUrl(const std::string &url) {
    return resolveDomain(extractDomain(url));
}

std::pair<bool, std::string> InputHelper::resolveDomain(const std::string &domain) {
    struct addrinfo hints{}, *res, *p;
    int status;
    char ipv4str[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(domain.c_str(), nullptr, &hints, &res)) != 0) {
        qInfo() << "getaddrinfo failed: " << gai_strerror(status);
        return {false, ""};
    }

    qInfo() << "IP addresses for " << domain << ":";

    for (p = res; p != nullptr; p = p->ai_next) {
        void *ipv4;

        if (p->ai_family == AF_INET) {
            ipv4 = &((reinterpret_cast<struct sockaddr_in *>(p->ai_addr))->sin_addr);
            inet_ntop(p->ai_family, ipv4, ipv4str, sizeof ipv4str);
            qInfo() << ipv4str;
        }
    }
    freeaddrinfo(res);
    return {true, ipv4str};
}

std::string InputHelper::extractDomain(const std::string &url) {
    std::size_t schemeEnd = url.find("://");
    std::size_t start = (schemeEnd != std::string::npos) ? schemeEnd + 3 : 0;

    std::size_t end = url.find_first_of("/:", start);
    if (end == std::string::npos) {
        end = url.length();
    }

    return url.substr(start, end - start);
}