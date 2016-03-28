#ifndef BFANALYZER_H
#define BFANALYZER_H

#include <vector>

// Finds the polygons between two intersecting linestrings
// input1, input2: two linestrings as WKT
// returns an array of polygons as WKT 
std::vector<std::string> polygonize(const std::string &input1, const std::string &input2);
#endif
