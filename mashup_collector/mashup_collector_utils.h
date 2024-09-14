#ifndef MASHUP_COLLECTOR_UTILS_H
#define MASHUP_COLLECTOR_UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <direct.h>

#include <windows.h>

#include <curl/curl.h>
#include <json/json.h>
#include <direct.h>

#include "../mashup_switcher/mashup_switcher_utils.h" // to access "get_cwd()"
namespace fs = std::filesystem;

std::string decode_mashup_name(const std::string &input);

size_t callback(void *contents, size_t size, size_t nmemb, std::string *userp);
std::string perform_get_request(const std::string &url);
void create_dir(std::string &path);
int download_mashup(const std::string& url, const std::string& target_path);
int parse_json_response(const std::string &response, const std::string input_name);

std::string decode_mashup_name(const std::string &input) {
  std::string result = input;
  std::string to_replace = "%20";
  std::string replace_with = " ";

  size_t pos = result.find(to_replace);
  while (pos != std::string::npos) {
    result.replace(pos, to_replace.length(), replace_with);
    pos = result.find(to_replace, pos + replace_with.length());
  }

  return result;
}

size_t callback(void *contents, size_t size, size_t nmemb, std::string *userp) {
  size_t total_size = size * nmemb;
  userp->append(static_cast<char*>(contents), total_size);
  return total_size;
}

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
  size_t written = fwrite(ptr, size, nmemb, stream);
  return written;
}

std::string perform_get_request(const std::string &url) {
  CURL *curl;
  CURLcode res;
  std::string response;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();
  return response;
}

void create_dir(std::string &path) {
  fs::path p(path);
  try {
    if (fs::create_directory(p)) {
      std::cout << "Directory Created Successfully." << std::endl;
    } else {
      std::cerr << "Directory Already Exists. Proceeding." << std::endl;
    }
  } catch (const fs::filesystem_error &e) {
    std::cerr << "An Error Occurred When Trying To Create Directory (" + path + ")" << e.what() << std::endl;
  }
}

int download_mashup(const std::string& url, const std::string& target_path) {
  CURL *curl;
  FILE *file;
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    file = fopen(target_path.c_str(), "wb");
    if (!file) {
      std::cerr << "Could Not Open File To Write";
      return -1;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() Failed: " << curl_easy_strerror(res) << std::endl;
    }

    fclose(file);
    curl_easy_cleanup(curl);

  } else {
    std::cerr << "Failed To Initialize curl.\n";
    return -1;

  }

  return 0;
}

int parse_json_response(const std::string &response, const std::string input_name) {
  Json::Value jsonData;
  Json::CharReaderBuilder builder;
  std::string errs;

  std::istringstream s(response);
  if (Json::parseFromStream(builder, s, &jsonData, &errs)) {
    // std::cout << "Parsed JSON: " << std::endl;
    // std::cout << jsonData.toStyledString() << std::endl;
    
    for (int i = 0; i < jsonData.size(); i++) {
      std::cout << i << "    " << jsonData[i]["songName"] << std::endl;
    }
    int input;
    std::cout << "Select Song: ";
    std::cin >> input;
    std::string song_name = jsonData[input]["songName"].asString();
    std::string file_name = jsonData[input]["songID"].asString();
    std::string download_url = jsonData[input]["downloadUrl"].asString();
    std::cout << "SONG NAME: " << song_name << std::endl << "DOWNLOAD URL: " << download_url << std::endl;

    std::string illegal_chars = "|:*?<>\"\\";
    for (char &c : song_name) {
      for (char i_c : illegal_chars) {
        if (c == i_c) {
          c = ' ';
        }
      }
    }

    std::string decoded_input_name = decode_mashup_name(input_name);

    std::string cwd = get_cwd();

    std::string folder_dir = cwd + "\\Mashups\\" + decoded_input_name;
    create_dir(folder_dir);

    std::string target_dir = cwd + "\\Mashups\\" + decoded_input_name + "\\" + song_name;
    create_dir(target_dir);
    std::cout << "Will Download File At: " << target_dir << std::endl;

    std::string target_file = target_dir + "\\" + file_name + ".mp3";

    int res = download_mashup(download_url, target_file);
    if (!res) {
      std::cout << "Successfully Downloaded " + song_name << std::endl;
    } else {
      std::cerr << "Failed To Download " + song_name << std::endl;
      return -1;
    }

  } else {
    std::cerr << "Failed To Parse json\n";
    return -1;

  }

  std::cin.clear();

  return 0;
}

#endif //MASHUP_COLLECTOR_UTILS_H