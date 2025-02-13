#ifndef VIT_WRITER_H
#define VIT_WRITER_H

class VTIWriter : public VTKWriter {
private:

  std::vector<int> L;
  std::vector<float> origin;
  DTYPE h;

public:
  VTIWriter(std::vector<int> L, DTYPE h=1.0);
  VTIWriter(std::vector<int> L, std::vector<float> origin, DTYPE h=1.0);
  ~VTIWriter(){};

  void Write(std::string fname);
};

#endif