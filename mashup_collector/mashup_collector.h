#ifndef MASHUP_COLLECTOR_H
#define MASHUP_COLLECTOR_H

#include <iostream>
#include <string>
#include <vector>

#include <filesystem>
#include <direct.h>

#include <curl/curl.h>
#include <json/json.h>

#include "./mashup_collector_utils.h"

std::string encode_spaces(std::string str) {
  std::string result;
  for (char c : str) {
    if (c == ' ') {
      result += "%20";
    } else {
      result += c;
    }
  }
  return result;
}

int run_mashup_collector() {

  std::string url = "https://api.songfilehub.com/v2/songs/";

  std::cout << "Enter Level Name: ";
  std::string song_name;
  std::getline(std::cin, song_name);

  song_name = encode_spaces(song_name); //

  std::string full_url = url + "?name=" + song_name;

  std::cout << "Sending GET request to: " << full_url << std::endl;

  std::string response = perform_get_request(full_url);

  if (!response.empty() && response.size() > 2) {
    std::cout << "RESPONSE SIZE: " << response.size() << std::endl;
    int res = parse_json_response(response, song_name);

    if (res == -1) {
      std::cerr << "Failed To Process json\n";
      return -1;
    }

  } else {
    std::cout << "No Response Received" << std::endl;
  }

  return 0;
}

#endif //MASHUP_COLLECTOR_H