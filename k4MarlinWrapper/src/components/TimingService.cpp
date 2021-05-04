#include "TimingService.h"

DECLARE_COMPONENT(TimingService);

TimingService::TimingService(const std::string& name, ISvcLocator* svcLoc)
  : base_class(name, svcLoc) {}

StatusCode TimingService::initialize() {
  if (Service::initialize().isFailure()) {
    error() << "Unable to initialize Service()" << endmsg;
    return StatusCode::FAILURE;
  }
  return StatusCode::SUCCESS;
}

StatusCode TimingService::finalize() {

  if (m_e2l_measurements.size() > 0) {
    int sum = std::accumulate(m_e2l_measurements.begin(), m_e2l_measurements.end(), 0.0);
    double mean = sum / m_e2l_measurements.size();
    std::cout << "EDM4hep --> LCIO average time: " << mean << std::endl;
    std::cout << "List of times:\n";
    for (auto& elem : m_e2l_measurements) {
      std::cout << elem << ", ";
    }
    std::cout << std::endl;
  }

  if (m_pe_measurements.size() > 0) {
    int sum = std::accumulate(m_pe_measurements.begin(), m_pe_measurements.end(), 0.0);
    double mean = sum / m_pe_measurements.size();
    std::cout << "Processors average time: " << mean << std::endl;
    std::cout << "List of times:\n";
    for (auto& elem : m_pe_measurements) {
      std::cout << elem << ", ";
    }
    std::cout << std::endl;
  }

  if (m_l2e_measurements.size() > 0) {
    int sum = std::accumulate(m_l2e_measurements.begin(), m_l2e_measurements.end(), 0.0);
    double mean = sum / m_l2e_measurements.size();
    std::cout << "LCIO --> EDM4hep average time: " << mean << std::endl;
    std::cout << "List of times:\n";
    for (auto& elem : m_l2e_measurements) {
      std::cout << elem << ", ";
    }
    std::cout << std::endl;
  }

  return StatusCode::SUCCESS;
}

void TimingService::addTime(
  const std::string& collection,
  const int elapsed_time)
{
  if (collection == "E2L") {
    m_e2l_measurements.push_back(elapsed_time);
  } else if (collection == "PE") {
    m_pe_measurements.push_back(elapsed_time);
  } else if (collection == "PE") {
    m_l2e_measurements.push_back(elapsed_time);
  } else {
    error() << "Wrong Tool Timing collection: " << collection << endmsg;
  }
}



