#ifndef K4MARLINWRAPPER_ITIMINGSERVICE_H
#define K4MARLINWRAPPER_ITIMINGSERVICE_H

#include "GaudiKernel/IService.h"

class ITimingService : virtual public IService {

public:

  DeclareInterfaceID( ITimingService, 3, 0 );

  virtual void addTime(
    const std::string& collection,
    const int elapsed_time) = 0;

  // virtual void addTime() = 0;
};

#endif