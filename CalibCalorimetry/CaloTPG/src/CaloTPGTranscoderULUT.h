#ifndef CALOTPGTRANSCODERULUT_H
#define CALOTPGTRANSCODERULUT_H 1

#include <memory>
#include <vector>
#include <set>
#include "CalibFormats/CaloTPG/interface/CaloTPGTranscoder.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "CondFormats/HcalObjects/interface/HcalLutMetadata.h"

/** \class CaloTPGTranscoderULUT
  *  
  * \author J. Mans - Minnesota
  */

class HcalTrigTowerGeometry;

class CaloTPGTranscoderULUT : public CaloTPGTranscoder {
public:
  CaloTPGTranscoderULUT(const std::string& compressionFile = "", const std::string& decompressionFile = "");
  ~CaloTPGTranscoderULUT() override;
  HcalTriggerPrimitiveSample hcalCompress(const HcalTrigTowerDetId& id,
                                          unsigned int sample,
                                          int fineGrain) const override;
  EcalTriggerPrimitiveSample ecalCompress(const EcalTrigTowerDetId& id,
                                          unsigned int sample,
                                          bool fineGrain) const override;

  void rctEGammaUncompress(const HcalTrigTowerDetId& hid,
                           const HcalTriggerPrimitiveSample& hc,
                           const EcalTrigTowerDetId& eid,
                           const EcalTriggerPrimitiveSample& ec,
                           unsigned int& et,
                           bool& egVecto,
                           bool& activity) const override;
  void rctJetUncompress(const HcalTrigTowerDetId& hid,
                        const HcalTriggerPrimitiveSample& hc,
                        const EcalTrigTowerDetId& eid,
                        const EcalTriggerPrimitiveSample& ec,
                        unsigned int& et) const override;
  double hcaletValue(const int& ieta, const int& iphi, const int& version, const int& compressedValue) const override;
  double hcaletValue(const HcalTrigTowerDetId& hid, const HcalTriggerPrimitiveSample& hc) const override;
  virtual bool HTvalid(const int ieta, const int iphi, const int version) const;
  virtual const std::vector<unsigned int> getCompressionLUT(const HcalTrigTowerDetId& id) const;
  virtual void setup(HcalLutMetadata const&,
                     HcalTrigTowerGeometry const&,
                     int nctScaleShift,
                     int rctScaleShift,
                     double lsbQIE8,
                     double lsbQIE11,
                     bool allLinear);
  virtual int getOutputLUTId(const HcalTrigTowerDetId& id) const;
  virtual int getOutputLUTId(const int ieta, const int iphi, const int version) const;

private:
  // Constant
  static const int NOUTLUTS = 4176;

  // Two possible linearization scales
  static const unsigned int REDUCE10BIT = 1024;
  static const unsigned int REDUCE11BIT = 2048;

  // Map different QIE to the right linearization
  static const unsigned int QIE8_OUTPUT_LUT_SIZE = REDUCE10BIT;
  static const unsigned int QIE10_OUTPUT_LUT_SIZE = REDUCE11BIT;
  static const unsigned int QIE11_OUTPUT_LUT_SIZE = REDUCE11BIT;
  static const unsigned int OUTPUT_LUT_SIZE =
      std::max({QIE8_OUTPUT_LUT_SIZE, QIE10_OUTPUT_LUT_SIZE, QIE11_OUTPUT_LUT_SIZE});
  static const unsigned int TPGMAX = 256;

  // Typedef
  typedef uint8_t LUT;
  typedef std::array<float, TPGMAX> RCTdecompression;

  const HcalTopology* theTopology;
  static const bool newHFphi = true;

  // Member functions
  unsigned int getOutputLUTSize(const HcalTrigTowerDetId& id) const;
  bool isOnlyQIE11(const HcalTrigTowerDetId& id) const;
  void loadHCALCompress(HcalLutMetadata const&, HcalTrigTowerGeometry const&);  //Analytical compression tables

  // Member Variables
  bool allLinear_ = false;
  double nominal_gain_;
  double lsb_factor_;
  double rct_factor_;
  double nct_factor_;
  double lin8_factor_;
  double lin11_factor_;
  const std::string compressionFile_;
  const std::string decompressionFile_;
  std::vector<int> ietal;
  std::vector<int> ietah;
  std::vector<int> ZS;
  std::vector<int> LUTfactor;

  std::vector<std::vector<LUT>> outputLUT_;
  std::vector<RCTdecompression> hcaluncomp_;

  std::set<HcalDetId> plan1_towers_;
};
#endif
