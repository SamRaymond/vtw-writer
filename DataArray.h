#ifndef DATA_ARRAY_H
#define DATA_ARRAY_H

#include <pugixml.hpp>

#include "DataOrder.h"

// Types:
// 0 double
// 1 float
// 2 int


class DataArray {
public:
    DataArray(std::string name, void *data, int numComponents, int numBytes, int type, Order::order order, long size, bool twoD);
    DataArray(){};
    ~DataArray();

    int NumComponents();
    std::string Name();

    void Write(pugi::xml_node *node);

private:
    
    std::string GetEncodedString(void *data);

    std::string GetCompressedVTKData();

    template <class O, class I>
    void twodToVTK(O *buffer, void *data, int vtkComponents){
        
        if(vtkComponents==1) {
            iiiToVTK<O,I>(buffer, data, vtkComponents);
            return;
        }

        for (int i = 0; i < size; i++) {
            for (int d = 0; d < numComponents; d++) buffer[i*vtkComponents+d] = (O)((I **)data)[d][i];
        }

        // Zero the third component if required
        if(numComponents != vtkComponents){
            for (int i = 0; i < size; i++) buffer[i*vtkComponents+(vtkComponents-1)]=0;
        }
    };
    
    template <class O, class I>
    void ijkToVTK(O *buffer, void *data, int vtkComponents){
        for (int i = 0; i < size; i++) {
            for (int d = 0; d < numComponents; d++) {
                buffer[i*vtkComponents+d] = (O)((I *)data)[i*numComponents+d];
            }
        }

        // Zero the third component if required
        if(numComponents != vtkComponents){
            for (int i = 0; i < size; i++) buffer[i*vtkComponents+(vtkComponents-1)]=0;
        }
    };

    template <class O, class I>
    void iiiToVTK(O *buffer, void *data, int vtkComponents){
        for (int i = 0; i < size; i++) {
            for (int d = 0; d < numComponents; d++) {
                buffer[i*vtkComponents+d] = (O)((I *)data)[i+d*size];
            }
        }

        // Zero the third component if required
        if(numComponents != vtkComponents){
            for (int i = 0; i < size; i++) buffer[i*vtkComponents+(vtkComponents-1)]=0;
        }
    };

    int numComponents;
    int vtkComponents;
    std::string name;
    std::string vtkType;
    int numBytes;
    Order::order order;
    int type;
    long size;
    void *data;
    bool twoD;
};

#endif