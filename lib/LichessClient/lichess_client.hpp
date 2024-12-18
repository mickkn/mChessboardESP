#ifndef LICHESS_CLIENT_H
#define LICHESS_CLIENT_H

#include <string>
#include <WiFiClientSecure.h>

class LichessClient {
public:
    LichessClient(const std::string& token);
    ~LichessClient();

    // Method to get user profile
    std::string getUserProfile(const std::string& username);

    // Method to get current games
    std::string getCurrentGames();

    // Method to make a move
    bool makeMove(const std::string& gameId, const std::string& move);

private:
    std::string token;
    WiFiClientSecure client;

    // Helper method to perform HTTP GET request
    std::string httpGet(const std::string& url);
};

#endif // LICHESS_CLIENT_H