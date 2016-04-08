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

#ifndef BFANALYZER_H
#define BFANALYZER_H

#include <vector>

// Finds the polygons between two intersecting linestrings
// input1, input2: two linestrings as WKT
// returns an array of polygons as WKT 
std::vector<std::string> polygonize(const std::string &input1, const std::string &input2);

// Transforms a MultiLineString into a MultiPolygon
// given that the LineStrings all touch the envelope
// input1: a MultiLineString as WKT
// a MultiPolygon as WKT 
std::string mlsToMlp(const std::string &input1);
#endif
