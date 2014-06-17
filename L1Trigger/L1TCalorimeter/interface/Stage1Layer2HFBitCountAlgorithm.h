///
/// \class l1t::Stage1Layer2HFBitCountAlgorithm
///
/// Description: interface for MP firmware
///
/// Implementation:
///
/// \author: R. Alex Barbieri
///

//

#ifndef Stage1Layer2HFBitCountAlgorithm_h
#define Stage1Layer2HFBitCountAlgorithm_h

#include "DataFormats/L1TCalorimeter/interface/CaloRegion.h"
#include "DataFormats/L1TCalorimeter/interface/CaloEmCand.h"
#include "DataFormats/L1Trigger/interface/HFBitCount.h"

#include <vector>

namespace l1t {

  class Stage1Layer2HFBitCountAlgorithm {
  public:
    virtual void processEvent(const std::vector<l1t::CaloRegion> & regions,
			      const std::vector<l1t::CaloEmCand> & EMCands,
			      std::vector<l1t::HFBitCount> * counts) = 0;

    virtual ~Stage1Layer2HFBitCountAlgorithm(){};
  };

}

#endif
