#ifndef VTP_WRITER_H
#define VTP_WRITER_H

class VTPWriter : public VTKWriter {
private:
  DataArray posarray;

public:
  VTPWriter(int nump);
  ~VTPWriter(){};
  void AddPositions(std::string name, double *data, int numComponents, Order::order order);
  void Write(std::string fname);

};

#endif