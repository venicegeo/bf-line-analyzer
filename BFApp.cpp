/*
Copyright 2016, RadiantBlue Technologies, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include "BFAnalyzer.h"

int main(int argc, const char* argv[])
{
  std::vector<std::string> polygons;
  if((argc == 2) && (strcmp(argv[1], "-test") == 0)){
    std::ifstream is1 ("../test/ls1.wkt");
    std::ifstream is2 ("../test/ls2.wkt");
    std::stringstream buffer1;
    buffer1 << is1.rdbuf();
    std::string str1 = buffer1.str();
    const char *cc1 = str1.c_str();
    std::stringstream buffer2;
    buffer2 << is2.rdbuf();
    std::string str2 = buffer2.str();
    is1.close();
    is2.close();
    polygons = polygonize(str1, str2);
  } else if(argc > 2){
    polygons = polygonize(std::string(argv[1]), std::string(argv[2]));
  } else {
    std::cout << "Usage: bf_la [wkt] [wkt]" << std::endl;
    return 1;
  }
  for(int inx = 0; inx < polygons.size(); inx++){
    std::cout << polygons[inx];
  }
  std::cout << std::endl;
  return 0;
}
