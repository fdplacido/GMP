#include "k4MarlinWrapper/converters/Lcio2EDM4hep.h"


DECLARE_COMPONENT(Lcio2EDM4hepTool);

Lcio2EDM4hepTool::Lcio2EDM4hepTool(const std::string& type, const std::string& name, const IInterface* parent)
    : GaudiTool(type, name, parent), m_eds("EventDataSvc", "Lcio2EDM4hepTool") {
  declareInterface<IEDMConverter>(this);

  StatusCode sc = m_eds.retrieve();
}

Lcio2EDM4hepTool::~Lcio2EDM4hepTool() { ; }


StatusCode Lcio2EDM4hepTool::initialize() {

  if (m_lcio2edm_params.size() % 3 != 0) {
    error() << " Error processing conversion parameters. 3 arguments per collection expected. " << endmsg;
    return StatusCode::FAILURE;
  }

  m_podioDataSvc = dynamic_cast<PodioDataSvc*>(m_eds.get());
  if (nullptr == m_podioDataSvc) return StatusCode::FAILURE;

  // Event Header
  m_dataHandlesMap["EventHeader"] =
    new DataHandle<edm4hep::EventHeaderCollection>("EventHeader", Gaudi::DataHandle::Writer, this);

  // Add and initialize DataHandles
  for (int i = 0; i < m_lcio2edm_params.size(); i=i+3) {
    if (m_lcio2edm_params[i] == "ReconstructedParticle") {
      m_dataHandlesMap[m_lcio2edm_params[i+2]] =
        new DataHandle<edm4hep::ReconstructedParticleCollection>(m_lcio2edm_params[i+2], Gaudi::DataHandle::Writer, this);
    } else if (m_lcio2edm_params[i] == "ParticleID") {
      m_dataHandlesMap[m_lcio2edm_params[i+2]] =
        new DataHandle<edm4hep::ParticleIDCollection>(m_lcio2edm_params[i+2], Gaudi::DataHandle::Writer, this);
    } else if (m_lcio2edm_params[i] == "MCParticle") {
      m_dataHandlesMap[m_lcio2edm_params[i+2]] =
        new DataHandle<edm4hep::MCParticleCollection>(m_lcio2edm_params[i+2], Gaudi::DataHandle::Writer, this);
    } else if (m_lcio2edm_params[i] == "Vertex") {
      m_dataHandlesMap[m_lcio2edm_params[i+2]] =
        new DataHandle<edm4hep::VertexCollection>(m_lcio2edm_params[i+2], Gaudi::DataHandle::Writer, this);
    } else if (m_lcio2edm_params[i] == "Track") {
      m_dataHandlesMap[m_lcio2edm_params[i+2]] =
        new DataHandle<edm4hep::TrackCollection>(m_lcio2edm_params[i+2], Gaudi::DataHandle::Writer, this);
    } else if (m_lcio2edm_params[i] == "TrackerHit") {
      m_dataHandlesMap[m_lcio2edm_params[i+2]] =
        new DataHandle<edm4hep::TrackerHitCollection>(m_lcio2edm_params[i+2], Gaudi::DataHandle::Writer, this);
    } else if (m_lcio2edm_params[i] == "SimTrackerHit") {
      m_dataHandlesMap[m_lcio2edm_params[i+2]] =
        new DataHandle<edm4hep::SimTrackerHitCollection>(m_lcio2edm_params[i+2], Gaudi::DataHandle::Writer, this);
    } else if (m_lcio2edm_params[i] == "CalorimeterHit") {
      m_dataHandlesMap[m_lcio2edm_params[i+2]] =
        new DataHandle<edm4hep::CalorimeterHitCollection>(m_lcio2edm_params[i+2], Gaudi::DataHandle::Writer, this);
    } else if (m_lcio2edm_params[i] == "SimCalorimeterHit") {
      m_dataHandlesMap[m_lcio2edm_params[i+2]] =
        new DataHandle<edm4hep::SimCalorimeterHitCollection>(m_lcio2edm_params[i+2], Gaudi::DataHandle::Writer, this);
      // Get associated collections
      m_dataHandlesMap["CaloHitContribution_EXT"] =
        new DataHandle<edm4hep::CaloHitContributionCollection>("CaloHitContribution_EXT", Gaudi::DataHandle::Writer, this);
    } else if (m_lcio2edm_params[i] == "RawCalorimeterHit") {
      m_dataHandlesMap[m_lcio2edm_params[i+2]] =
        new DataHandle<edm4hep::RawCalorimeterHitCollection>(m_lcio2edm_params[i+2], Gaudi::DataHandle::Writer, this);
    } else if (m_lcio2edm_params[i] == "TPCHit") {
      m_dataHandlesMap[m_lcio2edm_params[i+2]] =
        new DataHandle<edm4hep::TPCHitCollection>(m_lcio2edm_params[i+2], Gaudi::DataHandle::Writer, this);
    } else if (m_lcio2edm_params[i] == "Cluster") {
      m_dataHandlesMap[m_lcio2edm_params[i+2]] =
        new DataHandle<edm4hep::ClusterCollection>(m_lcio2edm_params[i+2], Gaudi::DataHandle::Writer, this);
    } else {
      debug() << m_lcio2edm_params[i] << ": conversion type not supported." << endmsg;
      debug() << "List of supported types: " <<
        "ReconstructedParticle, ParticleID, MCParticle, " <<
        "Vertex, Track, TrackerHit, " <<
        "SimTrackerHit, CalorimeterHit, SimCalorimeterHit, " <<
        "RawCalorimeterHit, TPCHit, Cluster." << endmsg;
    }
  }

  return GaudiTool::initialize();
}

StatusCode Lcio2EDM4hepTool::finalize() {

  for (const auto& [key, val] : m_dataHandlesMap) {
    delete val;
  }

  return GaudiTool::finalize();
}

template <typename T>
void Lcio2EDM4hepTool::convertPut(
  const std::string& edm_name,
  const std::string& lcio_name,
  k4LCIOConverter* lcio_converter,
  podio::CollectionIDTable* id_table)
{
  // Pass name of collection to get
  T* mycoll = dynamic_cast<T*>(lcio_converter->getCollection(lcio_name));

  auto* handle = dynamic_cast<DataHandle<T>*>( m_dataHandlesMap[edm_name] );
  if ( mycoll and handle->initialized() ) {
    handle->put(mycoll);
  } else {
    debug() << "Collection conversion for " << lcio_name << " returned nullptr: creating empty collection." << endmsg;
    mycoll = new T();
    mycoll->setID(id_table->add(lcio_name));
  }

}


bool Lcio2EDM4hepTool::collectionExist(
  const std::string& collection_name)
{
  auto collections = m_podioDataSvc->getCollections();
  for (const auto& [name, coll] : collections) {
    if (collection_name == name){
      std::cout << name << std::endl;;
      return true;
    }
  }
  return false;
}


StatusCode Lcio2EDM4hepTool::convertCollections(
  lcio::LCEventImpl* the_event)
{

  if (m_lcio2edm_params.size() % 3 != 0) {
    error() << " Error processing conversion parameters. 3 arguments per collection expected. " << endmsg;
    return StatusCode::FAILURE;
  }

  auto* id_table = m_podioDataSvc->getCollectionIDs();
  k4LCIOConverter* lcio_converter = new k4LCIOConverter(id_table);
  lcio_converter->set(the_event);

  convertPut<edm4hep::EventHeaderCollection>(
    "EventHeader", "EventHeader", lcio_converter, id_table);

  // Convert based on parameters
  for (int i = 0; i < m_lcio2edm_params.size(); i=i+3) {
    if (! collectionExist(m_lcio2edm_params[i+2])) {
      if (m_lcio2edm_params[i] == "ReconstructedParticle") {
        convertPut<edm4hep::ReconstructedParticleCollection>(
          m_lcio2edm_params[i+2], m_lcio2edm_params[i+1], lcio_converter, id_table);
      } else if (m_lcio2edm_params[i] == "ParticleID") {
        convertPut<edm4hep::ParticleIDCollection>(
          m_lcio2edm_params[i+2], m_lcio2edm_params[i+1], lcio_converter, id_table);
      } else if (m_lcio2edm_params[i] == "MCParticle") {
        convertPut<edm4hep::MCParticleCollection>(
          m_lcio2edm_params[i+2], m_lcio2edm_params[i+1], lcio_converter, id_table);
      } else if (m_lcio2edm_params[i] == "Vertex") {
        convertPut<edm4hep::VertexCollection>(
          m_lcio2edm_params[i+2], m_lcio2edm_params[i+1], lcio_converter, id_table);
      } else if (m_lcio2edm_params[i] == "Track") {
        convertPut<edm4hep::TrackCollection>(
          m_lcio2edm_params[i+2], m_lcio2edm_params[i+1], lcio_converter, id_table);
      } else if (m_lcio2edm_params[i] == "TrackerHit") {
        convertPut<edm4hep::TrackerHitCollection>(
          m_lcio2edm_params[i+2], m_lcio2edm_params[i+1], lcio_converter, id_table);
      } else if (m_lcio2edm_params[i] == "SimTrackerHit") {
        convertPut<edm4hep::SimTrackerHitCollection>(
          m_lcio2edm_params[i+2], m_lcio2edm_params[i+1], lcio_converter, id_table);
      } else if (m_lcio2edm_params[i] == "CalorimeterHit") {
        convertPut<edm4hep::CalorimeterHitCollection>(
          m_lcio2edm_params[i+2], m_lcio2edm_params[i+1], lcio_converter, id_table);
      } else if (m_lcio2edm_params[i] == "SimCalorimeterHit") {
        convertPut<edm4hep::SimCalorimeterHitCollection>(
          m_lcio2edm_params[i+2], m_lcio2edm_params[i+1], lcio_converter, id_table);
        // Get associated collections
        convertPut<edm4hep::CaloHitContributionCollection>(
          "CaloHitContribution_EXT", "CaloHitContribution_EXT", lcio_converter, id_table);
      } else if (m_lcio2edm_params[i] == "RawCalorimeterHit") {
        convertPut<edm4hep::RawCalorimeterHitCollection>(
          m_lcio2edm_params[i+2], m_lcio2edm_params[i+1], lcio_converter, id_table);
      } else if (m_lcio2edm_params[i] == "TPCHit") {
        convertPut<edm4hep::TPCHitCollection>(
          m_lcio2edm_params[i+2], m_lcio2edm_params[i+1], lcio_converter, id_table);
      } else if (m_lcio2edm_params[i] == "Cluster") {
        convertPut<edm4hep::ClusterCollection>(
          m_lcio2edm_params[i+2], m_lcio2edm_params[i+1], lcio_converter, id_table);
      } else {
        error() << m_lcio2edm_params[i] << ": conversion type not supported." << endmsg;
      }
    } else {
      debug() << " Collection " << m_lcio2edm_params[i+2] << " already in place, skipping conversion. " << endmsg;
    }
  }

  return StatusCode::SUCCESS;
}
