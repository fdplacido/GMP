#ifndef K4MARLINWRAPPER_ITIMINGSERVICE_H
#define K4MARLINWRAPPER_ITIMINGSERVICE_H

#include "GaudiKernel/IService.h"

class ITimingService : virtual public IService {

public:

  DeclareInterfaceID( ITimingService, 2, 0 );

  virtual void addTime() = 0;
};

#endif