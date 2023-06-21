#ifndef K4MARLINWRAPPER_IEDMCONVERTER_H
#define K4MARLINWRAPPER_IEDMCONVERTER_H

#include <GaudiKernel/IAlgTool.h>

// LCIO
#include <IMPL/LCEventImpl.h>
#include <lcio.h>

class IEDMConverter : virtual public IAlgTool {
public:
  DeclareInterfaceID(IEDMConverter, 1, 0);

  virtual StatusCode convertCollections(lcio::LCEventImpl* lcio_event) = 0;
};

#endif
