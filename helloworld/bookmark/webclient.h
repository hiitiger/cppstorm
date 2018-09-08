#pragma once
#include "../common/WinHttpClient.h"

class WebClient
{
public:
    struct HttpResponse
    {
        HttpResponse(bool r = false, int s = 0, const std::string b = "") : res(r), status(s), body(b) { ; }
        bool res = false;
        int status = 0;
        std::string body;
    };

    HttpResponse post(const std::string& url, const std::string& postData, const std::string& type);
    HttpResponse get(const std::string& url);
};

inline WebClient::HttpResponse WebClient::get(const std::string& url)
{
    HttpResponse res;
    WinHttpClient client(Storm::Utils::fromUtf8(url));
    res.res = client.SendHttpRequest();
    if (res.res)
    {
        res.status = std::stoi(client.GetResponseStatusCode());
        res.body = Storm::Utils::toUtf8(client.GetResponseContent());
    }
    return res;
}



inline WebClient::HttpResponse WebClient::post(const std::string& url, const std::string& postData, const std::string& type)
{
    HttpResponse res;
    WinHttpClient client(Storm::Utils::fromUtf8(url));

    // Set post data.
    client.SetAdditionalDataToSend((BYTE *)postData.c_str(), postData.size());

    // Set request headers.
    std::wstring headers = L"Content-Length: ";
    headers += std::to_wstring(postData.size());
    headers += L"\r\nContent-Type: ";
    headers += Storm::Utils::fromUtf8(type);
    headers += L"\r\n";
    client.SetAdditionalRequestHeaders(headers);

    // Send HTTP post request.
    res.res = client.SendHttpRequest(L"POST");
    if (res.res)
    {
        res.status = std::stoi(client.GetResponseStatusCode());
        res.body = Storm::Utils::toUtf8(client.GetResponseContent());
    }
    return res;
}

