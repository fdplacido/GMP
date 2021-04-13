#ifndef K4MARLINWRAPPER_TIMINGSERVICE_H
#define K4MARLINWRAPPER_TIMINGSERVICE_H

#include "ITimingService.h"

#include "GaudiKernel/Service.h"

#include <vector>

class TimingService : public extends1<Service, ITimingService> {

public:

  /// Constructor.
  explicit TimingService(const std::string& name, ISvcLocator* svcLoc);
  /// Initialize.
  virtual StatusCode initialize();
  /// Finalize.
  virtual StatusCode finalize();
  /// Destructor
  virtual ~TimingService() {}

  void addTime();

private:
  std::vector<int> m_measurements = {};
};

#endif