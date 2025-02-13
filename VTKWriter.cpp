#include <vector>
#include <string>
#include <iostream>
#include <ostream>

#include "VTKWriter.h"

void VTKWriter::AddArray(std::string name, std::vector<double> &data, int numComponents, Order::order order){
    if(data.size()!=size) std::cout << "Warning: Array of incorrect size (" << data.size() << ") passed to VTKWriter, size should be " << size << ". Array will not be added." << std::endl;
    else{
        DataArray arr(name,(void *)data.data(),numComponents,sizeof(double),0,order,size,twoD);
        arrays.push_back(arr);
    }
}

void VTKWriter::AddArray(std::string name, double *data, int numComponents, Order::order order){
    DataArray arr(name,(void *)data,numComponents,sizeof(double),0,order,size,twoD);
    arrays.push_back(arr);
}

void VTKWriter::AddArray(std::string name, double **data, int numComponents, Order::order order){
    DataArray arr(name,(void *)data,numComponents,sizeof(double),0,order,size,twoD);
    arrays.push_back(arr);
}

void VTKWriter::AddArray(std::string name, std::vector<float> &data, int numComponents, Order::order order){
    if(data.size()!=size) std::cout << "Warning: Array of incorrect size (" << data.size() << ") passed to VTKWriter, size should be " << size << ". Array will not be added." << std::endl;
    else{
        DataArray arr(name,(void *)data.data(),numComponents,sizeof(float),1,order,size,twoD);
        arrays.push_back(arr);
    }
}

void VTKWriter::AddArray(std::string name, float *data, int numComponents, Order::order order){
    DataArray arr(name,(void *)data,numComponents,sizeof(float),1,order,size,twoD);
    arrays.push_back(arr);
}

void VTKWriter::AddArray(std::string name, float **data, int numComponents, Order::order order){
    DataArray arr(name,(void *)data,numComponents,sizeof(float),1,order,size,twoD);
    arrays.push_back(arr);
}

void VTKWriter::AddArray(std::string name, std::vector<int> &data, int numComponents, Order::order order){
    if(data.size()!=size) std::cout << "Warning: Array of incorrect size (" << data.size() << ") passed to VTKWriter, size should be " << size << ". Array will not be added." << std::endl;
    else{
        DataArray arr(name,(void *)data.data(),numComponents,sizeof(int),2,order,size,twoD);
        arrays.push_back(arr);
    }
}

void VTKWriter::AddArray(std::string name, int *data, int numComponents, Order::order order){
    DataArray arr(name,(void *)data,numComponents,sizeof(int),2,order,size,twoD);
    arrays.push_back(arr);
}

void VTKWriter::AddArray(std::string name, int **data, int numComponents, Order::order order){
    DataArray arr(name,(void *)data,numComponents,sizeof(int),2,order,size,twoD);
    arrays.push_back(arr);
}