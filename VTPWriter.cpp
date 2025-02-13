#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <pugixml.hpp>
#include <sstream>

#include "DataArray.h"
#include "VTKWriter.h"
#include "VTPWriter.h"

VTPWriter::VTPWriter(int nump){
    this->size = nump;
}

void VTPWriter::Write(std::string fname)
{
  pugi::xml_document *doc = new pugi::xml_document;

  pugi::xml_node decl = doc->prepend_child(pugi::node_declaration);
  decl.append_attribute("version") = "1.0";
  decl.append_attribute("encoding") = "UTF-8";

  pugi::xml_node vtp  = doc->append_child("VTKFile");
  vtp.append_attribute("type") = "PolyData";
  vtp.append_attribute("byte_order") = "LittleEndian";
  vtp.append_attribute("compressor") = "vtkZLibDataCompressor";

  pugi::xml_node polyData  = vtp.append_child("PolyData");

  pugi::xml_node piece  = polyData.append_child("Piece");
  std::stringstream numparts;
  numparts << this->size;
  piece.append_attribute("NumberOfPoints") = numparts.str().c_str();
  piece.append_attribute("NumberOfVerts") = "0";
  piece.append_attribute("NumberOfLines") = "0";
  piece.append_attribute("NumberOfStrips") = "0";
  piece.append_attribute("NumberOfPolys") = "0";

  pugi::xml_node points  = piece.append_child("Points");
// Add positions here
  posarray.Write(&points);
 

// Now add all other data
  pugi::xml_node pointData  = piece.append_child("PointData");
  std::stringstream scalars;
  int lastIDX = 0;
  for(int i=0; i<arrays.size(); i++){
    if(arrays[i].NumComponents() == 1) lastIDX = i;
  }
  for(int i=0; i<arrays.size(); i++){
    if(arrays[i].NumComponents() == 1) 
    {
      scalars << arrays[i].Name();
      if(i!=lastIDX) scalars << " ";
    }
  }

  std::stringstream vectors;
  for(int i=0; i<arrays.size(); i++){
    if(arrays[i].NumComponents() != 1) lastIDX = i;
  }
  for(int i=0; i<arrays.size(); i++){
    if(arrays[i].NumComponents() != 1) {
      vectors << arrays[i].Name();
      if(i!=lastIDX) vectors << " ";
    }
  }

  pointData.append_attribute("Scalars") = scalars.str().c_str();
  pointData.append_attribute("Vectors") = vectors.str().c_str();

  for(int i=0; i<arrays.size(); i++){
    arrays[i].Write(&pointData);
  }

  doc->save_file(fname.c_str());
  delete doc;
}


void VTPWriter::AddPositions(std::string name, double *data, int numComponents, Order::order order){
        DataArray arr(name,(void *)data,numComponents,sizeof(double),0,order,size,twoD);
        this->posarray = arr;
    // this->posarray(name,(void *)data,numComponents,sizeof(double),0,order,size,twoD);
};