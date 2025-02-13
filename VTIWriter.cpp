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
#include "VTIWriter.h"

VTIWriter::VTIWriter(std::vector<int> L, DTYPE h)
{
  this->L = L;
  this->h = h;
  this->size = 1;
  for(int i=0; i<3; i++) this->size*=L[i];
  this->twoD = L[2] <= 1;
  this->origin.push_back(0.0);
  this->origin.push_back(0.0);
  this->origin.push_back(0.0);
}

VTIWriter::VTIWriter(std::vector<int> L, std::vector<float> origin, DTYPE h)
{
  this->L = L;
  this->h = h;
  this->size = 1;
  for(int i=0; i<3; i++) this->size*=L[i];
  this->twoD = L[2] <= 1;
  this->origin = origin;
}

void VTIWriter::Write(std::string fname)
{
  pugi::xml_document *doc = new pugi::xml_document;

  pugi::xml_node decl = doc->prepend_child(pugi::node_declaration);
  decl.append_attribute("version") = "1.0";
  decl.append_attribute("encoding") = "UTF-8";

  pugi::xml_node vti  = doc->append_child("VTKFile");
  vti.append_attribute("type") = "ImageData";
  vti.append_attribute("byte_order") = "LittleEndian";
  vti.append_attribute("compressor") = "vtkZLibDataCompressor";

  pugi::xml_node imageData  = vti.append_child("ImageData");

  std::stringstream extent;
  extent << "0 " << L[0] << " 0 " << L[1] << " 0 ";
  if(L[2] > 1)  extent << L[2];
  else extent << 0;

  std::stringstream spacing;
  spacing << h << " " << h << " " << h;

  imageData.append_attribute("WholeExtent") = extent.str().c_str();


  std::stringstream originss;
  originss << origin[0] << " " << origin[0];
  if(origin.size()>2) originss << " " << origin[0];
  else originss << " 0";
  imageData.append_attribute("Origin") = originss.str().c_str();

  imageData.append_attribute("Spacing") = spacing.str().c_str();

  pugi::xml_node piece  = imageData.append_child("Piece");
  piece.append_attribute("Extent") = extent.str().c_str();

  pugi::xml_node cellData  = piece.append_child("CellData");

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

  cellData.append_attribute("Scalars") = scalars.str().c_str();
  cellData.append_attribute("Vectors") = vectors.str().c_str();

  for(int i=0; i<arrays.size(); i++){
    arrays[i].Write(&cellData);
  }

  doc->save_file(fname.c_str());
  delete doc;
}
