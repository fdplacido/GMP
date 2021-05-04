#ifndef K4MARLINWRAPPER_TIMINGSERVICE_H
#define K4MARLINWRAPPER_TIMINGSERVICE_H

#include "ITimingService.h"

#include "GaudiKernel/Service.h"

#include <vector>
#include <numeric>

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

  void addTime(
    const std::string& collection,
    const int elapsed_time);

private:
  // EDM4hep -> LCIO elapsed time counters
  std::vector<int> m_e2l_measurements = {};
  // Processors elapsed time counters
  std::vector<int> m_pe_measurements = {};
  // LCIO -> EDM4hep elapsed time counters
  std::vector<int> m_l2e_measurements = {};
};

#endif