#!/bin/bash
# Format all C++ source files using clang-format

set -e

GREEN='\033[0;32m'
NC='\033[0m'

echo -e "${GREEN}Formatting C++ files...${NC}"

# Find all .cpp and .h files
find include lib tools -name '*.cpp' -o -name '*.h' | while read -r file; do
  echo "Formatting: $file"
  clang-format -i "$file"
done

echo -e "${GREEN}Formatting completed!${NC}"
