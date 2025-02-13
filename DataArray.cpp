#include <string>
#include <sstream>
#include "base64.h"
#include <zlib.h>
#include <cmath>
#include <iostream>

#include "DataArray.h"

DataArray::DataArray(std::string name, void *data, int numComponents, int numBytes, int type, Order::order order, long size, bool twoD){
    this->name = name;
    this->data = data;
    this->numComponents = numComponents;
    this->type = type;
    this->order = order;
    this->numBytes = numBytes;
    this->size = size;
    this->twoD = twoD;

    vtkComponents = numComponents==2 ? 3 : numComponents;
    switch(type){
        case 0:
            vtkType = "Float64";
            break;
        case 1:
            vtkType = "Float32";
            break;
        case 2:
            vtkType = "Int32";
            break;
    }
}

DataArray::~DataArray(){}

int DataArray::NumComponents(){
    return numComponents;
}

std::string DataArray::Name(){
    return name;
}

void DataArray::Write(pugi::xml_node *node)
{
  pugi::xml_node n = node->append_child("DataArray");
  n.append_attribute("type") = vtkType.c_str();
  n.append_attribute("NumberOfComponents") = vtkComponents;
  n.append_attribute("format") = "binary";
  n.append_attribute("Name") = name.c_str();
  n.append_child(pugi::node_pcdata).set_value(GetCompressedVTKData().c_str());
}

std::string DataArray::GetCompressedVTKData()
{
    void *tmp = (void*) (new char[size*vtkComponents*numBytes]);

    switch(order)
    {
        case Order::twod:
            switch(type){
                case 0:
                    twodToVTK<double,double>((double *)tmp,data,vtkComponents);
                    break;
                case 1:
                    twodToVTK<float,float>((float *)tmp,data,vtkComponents);
                    break;
                case 2:
                    twodToVTK<int,int>((int *)tmp,data,vtkComponents);
                    break;
            }
          break;
  
        case Order::ijk:
          switch(type){
              case 0:
                  ijkToVTK<double,double>((double *)tmp,data,vtkComponents);
                  break;
              case 1:
                  ijkToVTK<float,float>((float *)tmp,data,vtkComponents);
                  break;
              case 2:
                  ijkToVTK<int,int>((int *)tmp,data,vtkComponents);
                  break;
          }
          break;
          
        case Order::iii:
          switch(type){
              case 0:
                  iiiToVTK<double,double>((double *)tmp,data,vtkComponents);
                  break;
              case 1:
                  iiiToVTK<float,float>((float *)tmp,data,vtkComponents);
                  break;
              case 2:
                  iiiToVTK<int,int>((int *)tmp,data,vtkComponents);
                  break;
          }
          break;
    }

    std::string compressedData = GetEncodedString(tmp);
    delete [] tmp;
    return compressedData;
}

#define VTP_BINARY_BLOCK_SIZE (1<<30)
std::string DataArray::GetEncodedString(void *data)
{
  uLongf inputBytes = size * vtkComponents * numBytes;
  uLongf numberOfBlocks = ceil(inputBytes / (DTYPE) VTP_BINARY_BLOCK_SIZE);
  uLongf lastBlockSize  = inputBytes % VTP_BINARY_BLOCK_SIZE;


  int *headerData = new int[3+numberOfBlocks];
  headerData[0] = numberOfBlocks;
  headerData[1] = (numberOfBlocks>1) ? VTP_BINARY_BLOCK_SIZE : lastBlockSize;
  headerData[2] = lastBlockSize;

  uLongf compressedDataSize = compressBound((uLongf) lastBlockSize) + (numberOfBlocks-1)*compressBound((uLongf) VTP_BINARY_BLOCK_SIZE);
  unsigned char *compressBuffer = new unsigned char[compressedDataSize];
  int totalCompressedDataSize = 0;

  uLongf currentSize = compressedDataSize;

  unsigned char *compressBufferPtr = compressBuffer;
  for (int i=0; i<numberOfBlocks; i++)
  {
    int blockSize = VTP_BINARY_BLOCK_SIZE;
    if (numberOfBlocks == 1 || i==numberOfBlocks-1) // last block
      blockSize = lastBlockSize;
    int blockStart = i*VTP_BINARY_BLOCK_SIZE;

    compress(compressBufferPtr,
              &currentSize,
              (unsigned char *) data+blockStart,
              blockSize);

    compressBufferPtr += currentSize;
    totalCompressedDataSize += currentSize;

    headerData[3+i] = (int) currentSize;
    currentSize = compressedDataSize - (compressBufferPtr-compressBuffer);
  }

  uLongf b64HeaderSize = ((4*(numberOfBlocks+3)+2)/3)*4;
  uLongf b64DataSize   = ((totalCompressedDataSize+2)/3)*4;

  char *outputBuffer = new char[b64HeaderSize + b64DataSize+1];
  base64encode(headerData, 4*(numberOfBlocks+3), outputBuffer, b64HeaderSize+1);
  base64encode(compressBuffer, totalCompressedDataSize, outputBuffer+b64HeaderSize, b64DataSize+1);

  delete [] compressBuffer;
  delete [] headerData;

  std::stringstream buffer;
  buffer.write(outputBuffer, b64HeaderSize + b64DataSize);
  delete [] outputBuffer;

  return buffer.str();
}