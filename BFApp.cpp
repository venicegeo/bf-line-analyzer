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

std::string loadFile(const char* filename);

int main(int argc, const char* argv[])
{
  std::vector<std::string> strings;
  if((argc == 2) && (strcmp(argv[1], "-test") == 0)){
    strings = polygonize(loadFile("../test/ls1.wkt"), loadFile("../test/ls2.wkt"));
  } else if(argc > 2){
    if(strcmp(argv[1], "-mlp") == 0) {
      strings.push_back(mlsToMlp(loadFile(argv[2])));
    } else {
      strings = polygonize(loadFile(argv[1]), loadFile(argv[2]));
    }
  } else {
    std::cout << "Usage:" << std::endl;
    std::cout << "bf_la [wkt] [wkt] to analyze two linestrings" << std::endl;
    std::cout << "bf_la -mlp [wkt] to transform a multilinestring to a multipolygon" << std::endl;
    std::cout << "bf_la -test" << std::endl;
    return 1;
  }
  for(int inx = 0; inx < strings.size(); inx++){
    std::cout << strings[inx];
  }
  std::cout << std::endl;
  return 0;
}

std::string loadFile(const char* filename){
  std::string result;
  std::ifstream ifs(filename);
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  result = buffer.str();
  ifs.close();
  return result;
}

