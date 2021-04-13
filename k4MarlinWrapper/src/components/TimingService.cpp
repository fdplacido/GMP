#include "TimingService.h"

DECLARE_COMPONENT(TimingService);

TimingService::TimingService(const std::string& name, ISvcLocator* svcLoc)
  : base_class(name, svcLoc) {

}

StatusCode TimingService::initialize() {
  if (Service::initialize().isFailure()) {
    error() << "Unable to initialize Service()" << endmsg;
    return StatusCode::FAILURE;
  }
  return StatusCode::SUCCESS;
}

StatusCode TimingService::finalize() {
  return StatusCode::SUCCESS;
}

void TimingService::addTime()
{
  if (m_measurements.size() == 0) {
    m_measurements.push_back(1);
  } else {
    int measurement = m_measurements.back() + 1;
    m_measurements.push_back(measurement);
  }

  std::cout << "BTIME\n";
  for (auto& elem : m_measurements) {
    std::cout << elem << ", ";
  }
  std::cout << "\nATIME\n";
}



