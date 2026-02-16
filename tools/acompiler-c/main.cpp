#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "acompiler/Frontend/Parser.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "usage: acompiler-c <input.txt>\n";
    return 1;
  }

  std::ifstream in(argv[1]);
  if (!in) {
    std::cerr << "failed to open input file: " << argv[1] << "\n";
    return 1;
  }

  std::vector<std::string> lines;
  for (std::string line; std::getline(in, line);) {
    if (!line.empty()) {
      lines.push_back(line);
    }
  }

  acompiler::Parser parser;
  try {
    auto module = parser.parseLines(lines);
    std::cout << "parsed " << module.nodes().size() << " statements\n";
  } catch (const std::exception& ex) {
    std::cerr << "parse error: " << ex.what() << "\n";
    return 2;
  }

  return 0;
}
