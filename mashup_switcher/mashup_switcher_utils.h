#ifndef MASHUP_SWITCHER_UTILS_H
#define MASHUP_SWITCHER_UTILS_H

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

namespace fs = std::filesystem;

std::string get_gd_song_folder_path();
std::string get_cwd(); // get current working directory
void iterate(std::string mashups_path, std::vector<std::string> *paths); // iterates through the mashups folder and adds the songs to a pointer vector
std::string get_mashup_path(std::string mashup_folder_path); // gets the path of the mashup file from the parent folder path
void enable_mashup(std::string gd_path, std::string mashup_path);

std::string get_gd_song_folder_path() {
  std::string local_appdata = getenv("LOCALAPPDATA");
  std::string gd_path = local_appdata + "\\GeometryDash";
  return gd_path;
}

std::string get_cwd() {
  TCHAR buffer[MAX_PATH] = { 0 };
  GetModuleFileName(NULL, buffer, MAX_PATH);
  std::string cwd = fs::path(buffer).parent_path().string();
  return cwd;
}

void iterate(std::string mashups_path, std::vector<std::string> *paths) {
  for (const auto &entry : fs::directory_iterator(mashups_path)) {
    std::string file_path = entry.path().string();

    paths->push_back(file_path);
  }
}

std::string get_mashup_path(std::string mashup_folder_path) {
  for (const auto &entry : fs::directory_iterator(mashup_folder_path)) {
    if (entry.is_regular_file() && entry.path().extension() == ".mp3") {
      std::string mashup_path = entry.path().string();
      return mashup_path;
    }
  }
  return "file not found";
}

void enable_mashup(std::string gd_path, std::string mashup_path) {
  fs::path destination_mashup = fs::path(gd_path) / fs::path(mashup_path).filename();

  try {

    if (fs::exists(destination_mashup)) {
      fs::remove(destination_mashup);
    }

    fs::copy(mashup_path, destination_mashup);
    std::cout << "Successfully Enabled Mashup\n";
  } catch (const fs::filesystem_error &e) {
    std::cerr << "Error copying file: " << e.what() << std::endl;
  }
}

#endif //MASHUP_SWITCHER_UTILS_H