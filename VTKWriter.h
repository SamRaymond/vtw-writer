#ifndef VTK_WRITER_H
#define VTK_WRITER_H

#include "DataOrder.h"
#include "DataArray.h"

class VTKWriter {
public:
    VTKWriter(){};
    ~VTKWriter(){};

    void AddArray(std::string name, std::vector<double> &data, int numComponents, Order::order order=Order::twod);
    void AddArray(std::string name, double *data, int numComponents, Order::order order=Order::twod);
    void AddArray(std::string name, double **data, int numComponents, Order::order order=Order::twod);
    void AddArray(std::string name, std::vector<float> &data, int numComponents, Order::order order=Order::twod);
    void AddArray(std::string name, float *data, int numComponents, Order::order order=Order::twod);
    void AddArray(std::string name, float **data, int numComponents, Order::order order=Order::twod);
    void AddArray(std::string name, std::vector<int> &data, int numComponents, Order::order order=Order::twod);
    void AddArray(std::string name, int *data, int numComponents, Order::order order=Order::twod);
    void AddArray(std::string name, int **data, int numComponents, Order::order order=Order::twod);
    virtual void Write(std::string fname)=0;

protected:

    std::vector<DataArray> arrays;
    long size;
    bool twoD;
};

#endif