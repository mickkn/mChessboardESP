#include "lichess_client.hpp"
#include <ArduinoJson.h>

// Constructor
LichessClient::LichessClient(const std::string& token) : token(token) {
    client.setInsecure(); // Use this for testing purposes only, for production use proper certificates
}

// Destructor
LichessClient::~LichessClient() {
    // No specific cleanup required for WiFiClientSecure
}

// Helper method to perform HTTP GET request
std::string LichessClient::httpGet(const std::string& url) {
    if (!client.connect("lichess.org", 443)) {
        return "";
    }

    client.print(String("GET ") + url.c_str() + " HTTP/1.1\r\n" +
                 "Host: lichess.org\r\n" +
                 "Authorization: Bearer " + token.c_str() + "\r\n" +
                 "Connection: close\r\n\r\n");

    std::string response;
    while (client.connected() || client.available()) {
        if (client.available()) {
            response += client.readString().c_str();
        }
    }
    client.stop();

    // Extract the body from the response
    size_t bodyPos = response.find("\r\n\r\n");
    if (bodyPos != std::string::npos) {
        return response.substr(bodyPos + 4);
    }
    return "";
}

// Method to get user profile
std::string LichessClient::getUserProfile(const std::string& username) {
    std::string url = "/api/user/" + username;
    return httpGet(url);
}

// Method to get current games
std::string LichessClient::getCurrentGames() {
    std::string url = "/api/account/playing";
    return httpGet(url);
}

// Method to make a move
bool LichessClient::makeMove(const std::string& gameId, const std::string& move) {
    std::string url = "/api/board/game/" + gameId + "/move/" + move;
    std::string response = httpGet(url);
    return response.find("\"ok\":true") != std::string::npos;
}