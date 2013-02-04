#ifndef UWRITERBED6_H_INCLUDED
#define UWRITERBED6_H_INCLUDED

#include "uWriterBed.h"
#include <iostream>
#include <fstream>

namespace NGS
{

class uWriterBed6 : public uWriterBed
{
public:
    /** \brief Empty constructor (call object with init through factory instead)
      */
    uWriterBed6() {}
    virtual void writeToken(const uToken& token);
    static uWriterBase * Create() { return new uWriterBed6(); }
private:
  //  static DerivedRegister<uWriterBed6> reg;

}; // End of class uWriterBed6

} // End of namespace NGS
#endif // UWRITERBED6_H_INCLUDED
