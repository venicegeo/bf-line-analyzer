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

#include <istream>
#include <geos.h>
#include <geos/operation/polygonize/Polygonizer.h>
#include "BFAnalyzer.h"

Geometry* smallestSegment(Point *p11, Point *p12, Point *p21, Point *p22);

std::vector<std::string> polygonize(const std::string &input1, const std::string &input2)
{
  std::vector<std::string> result;
  WKTReader wktr;
  WKTWriter wktw;
  Geometry *line1 = wktr.read(input1);
  Geometry *line2 = wktr.read(input2);

  // Unioning the linestrings inserts nodes for polygonization
  Geometry *nodedLineStrings = line1->Union(line2);
  
  // Add additional lineStrings to each end to close them off
  LineString *ls1 = dynamic_cast<LineString*>(line1);
  LineString *ls2 = dynamic_cast<LineString*>(line2);
  Point *sp1 = ls1->getStartPoint();
  Point *sp2 = ls2->getStartPoint();
  Point *ep1 = ls1->getEndPoint();
  Point *ep2 = ls2->getEndPoint();
  Geometry *smallests = smallestSegment(sp1, ep1, sp2, ep2);
  nodedLineStrings = nodedLineStrings->Union(smallests);

  // Use GEOS to polygonize the strings
  geos::operation::polygonize::Polygonizer polygonizer;
  polygonizer.add(nodedLineStrings);
  std::vector<Polygon*> *polygons = polygonizer.getPolygons();
  
  for(int inx = 0; inx < polygons->size(); inx++){
    result.push_back(wktw.write((*polygons)[inx]));
  }

  return result;
}

// This is a very annoying thing - we want to generate line segments to turn
// the input pair of line segments (A1, A2), (B1, B2) into a closed ring. 
Geometry* smallestSegment(Point *p11, Point *p12, Point *p21, Point *p22){
  const GeometryFactory *gf = GeometryFactory::getDefaultInstance();
  const CoordinateSequenceFactory *csf = gf->getCoordinateSequenceFactory();

  // Create the two candidate line segments.
  CoordinateSequence *cs1 = csf->create();
  CoordinateSequence *cs2 = csf->create();
  cs1->add(*(p11->getCoordinate()), false);
  cs1->add(*(p21->getCoordinate()), false);
  cs2->add(*(p12->getCoordinate()), false);
  cs2->add(*(p22->getCoordinate()), false);
  LineString *l1 = gf->createLineString(cs1);
  LineString *l2 = gf->createLineString(cs2);
  
  // If their envelopes overlap each other it is wrong 
  // and we need to reverse the two segments.
  Geometry *env1 = l1->getEnvelope();
  Geometry *env2 = l2->getEnvelope();  
  if(env1->overlaps(env2)){
    delete env1;
    delete env2;
    delete l1;
    delete l2;
    cs1 = csf->create();
    cs2 = csf->create();
    cs1->add(*(p11->getCoordinate()), false);
    cs1->add(*(p22->getCoordinate()), false);
    cs2->add(*(p12->getCoordinate()), false);
    cs2->add(*(p21->getCoordinate()), false);
    LineString *l1 = gf->createLineString(cs1);
    LineString *l2 = gf->createLineString(cs2);
  }
  
  return l1->Union(l2);
}

std::string mlsToMlp(const std::string &input1){
  std::string result;
  WKTReader wktr;
  WKTWriter wktw;
  Geometry *mls = wktr.read(input1);

  // First we have to properly node the linestrings. Union makes this easy.
  Polygon *envelope = dynamic_cast<Polygon*>(mls->getEnvelope());
  Geometry *nodedLineStrings = envelope->getExteriorRing()->Union(mls);

  // The polygonizer creates polygons out of the linework
  geos::operation::polygonize::Polygonizer polygonizer;
  polygonizer.add(nodedLineStrings);
  std::vector<Polygon*> *polygons = polygonizer.getPolygons();

  // Annoyingly, we have to transform one vector type to another
  std::vector<Geometry*> geometries;
  for(int inx = 0; inx < polygons->size(); inx++){
    geometries.push_back((*polygons)[inx]);
  }

  Geometry *mlp = GeometryFactory::getDefaultInstance()->createMultiPolygon(geometries);
  result = wktw.write(mlp);
  return result;
}

