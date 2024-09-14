#include <iostream>

#include "./mashup_collector/mashup_collector.h"
#include "./mashup_switcher/mashup_switcher.h"

int main() {

  while (1) {
    std::cin.clear();
    std::string input;
    std::cout << "\n_______________________\n";
    std::cout << "What Do You Want To Do?\n";
    std::cout << "0 - Collect Mashups\n";
    std::cout << "1 - Switch Mashups\n";
    std::cout << "5 - Exit\n";
    std::cout << "_______________________\n";
    std::cout << "->";
    std::getline(std::cin, input);
    
    if (input == "0") {
      int mashup_collector = run_mashup_collector();
    } else if (input == "1") {
      int mashup_switcher = run_mashup_switcher();
    } else if (input == "5") {
      return 0;
    }
    
  }

  return 0;
}