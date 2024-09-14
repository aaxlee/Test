#ifndef MASHUP_SWITCHER_H
#define MASHUP_SWITCHER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <windows.h>

#include "./mashup_switcher_utils.h"

namespace fs = std::filesystem;

int run_mashup_switcher() {

  const std::string cwd = get_cwd();
  const std::string gd_folder_path = get_gd_song_folder_path();

  fs::path gd_path = gd_folder_path;

  if (!fs::is_directory(gd_path)) {
    std::cout << "Unable To Locate GD Path.\n";
    return 1;
  } 

  const std::string songs_folder_path = cwd + "\\Mashups";
  std::cout << songs_folder_path << std::endl;
  std::vector<std::string> *song_paths = new std::vector<std::string>;

  iterate(songs_folder_path, song_paths);

  if (song_paths->size() == 0) {
    std::cout << "No Level(s) found\n";
    delete(song_paths);
    return 0;
  }

  std::cout << "Found (" << song_paths->size() << ")" << " Level(s):\n";

  int count = 0;
  for (auto it : *song_paths) {
    fs::path folder_path(it);
    std::string song_name = folder_path.filename().string();
    std::cout << count << "    " << song_name << '\n';
    count++;
  }

  std::cout << "Select Song (0 - " << count << "):\n";

  int song_choice;
  std::cin >> song_choice;

  std::vector<std::string> *mashups = new std::vector<std::string>;
  std::string song_path = song_paths->at(song_choice);
  iterate(song_path, mashups);

  if (mashups->size() == 0) {
    std::cout << "No Mashup(s) Found For The Song";
    delete(song_paths);
    delete(mashups);
    return 0;
  }

  std::cout << "Found (" << mashups->size() << ")" << " Mashup(s):\n";

  count = 0;
  for (auto it : *mashups) {
    fs::path folder_path(it);
    std::string mashup = folder_path.filename().string();
    std::cout << count << "    " << mashup << '\n';
    count++;
  }

  std::cout << "Select Mashup (0 - " << count << "):\n";

  int mashup_choice;
  std::cin >> mashup_choice;

  std::string mashup_folder_path = mashups->at(mashup_choice);
  std::string mashup_path = get_mashup_path(mashup_folder_path);

  enable_mashup(gd_folder_path, mashup_path);

  delete(song_paths);
  delete(mashups);

  return 0;
}

#endif //MASHUP_SWITCHER_H