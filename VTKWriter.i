%module VTKWriter

%include "std_vector.i"
%template(IntVector) std::vector<int>;
%template(DoubleVector) std::vector<double>;
%template(FloatVector) std::vector<float>;
%include "std_string.i"

%{
#include "DataOrder.h"
#include "VTKWriter.h"
%}

%include "DataOrder.h"
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
};

// Add type specific writers here
%{#include "VTIWriter.h"%}
%include "VTIWriter.h"

%{#include "VTPWriter.h"%}
%include "VTPWriter.h"