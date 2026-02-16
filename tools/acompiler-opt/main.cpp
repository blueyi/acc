#include <iostream>
#include <string>

int main(int argc, char** argv) {
  std::string pipeline = "default";
  if (argc > 1) {
    pipeline = argv[1];
  }

  std::cout << "acompiler-opt skeleton\n";
  std::cout << "selected pipeline: " << pipeline << "\n";
  return 0;
}
