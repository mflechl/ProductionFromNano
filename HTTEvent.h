#ifndef WarsawAnalysis_HTTDataFormats_HTTEvent_h
#define WarsawAnalysis_HTTDataFormats_HTTEvent_h

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TBits.h"
#include <map>
#include <vector>
#include <bitset>
#include <iostream>

#include "PropertyEnum.h"
#include "JecUncEnum.h"
#include "TriggerEnum.h"
#include "FilterEnum.h"
#include "SelectionBitsEnum.h"
#include "AnalysisEnums.h"
///////////////////////////////////////////////////
///////////////////////////////////////////////////
class HTTEvent{

 public:

  enum sampleTypeEnum {DUMMY = -1, DATA = 0, DY = 1, DYLowM = 2, WJets=3, TTbar=4, h=5, H=6, A=7};

  static const int ntauIds = 13;
  static const int againstMuIdOffset = 0;
  static const int againstEIdOffset = againstMuIdOffset+2;
  static const int mvaIsoIdOffset = againstEIdOffset+5;
  static const TString tauIDStrings[ntauIds];//implementation in cxx

  HTTEvent(){clear();}

  ~HTTEvent(){}

  ///Data member setters.
  void setRun(unsigned int x){runId = x;}

  void setEvent(unsigned long int x){eventId = x;}

  void setLS(unsigned long int x){lsId = x;}

  void setNPU(float x){nPU = x;}

  void setNPV(unsigned int x){nPV = x;}

  void setRho(float x){rho = x;}

  void setMCatNLOWeight(float x){aMCatNLOweight = x;}

  void setMCWeight(float x){mcWeight = x;}

  void setPtReWeight(float x){ptReWeight = x;}

  void setPtReWeightR1(float x){ptReWeightR1 = x;}

  void setPtReWeightSUSY(float x){ptReWeightSUSY = x;}

  void setLHE_Ht(float x){lheHt = x;}

  void setLHEnOutPartons(int x){lheNOutPartons = x;}

  void setSampleType(sampleTypeEnum x){sampleType = x;}

  void setDecayModeMinus(int x){decayModeMinus = x;}

  void setDecayModePlus(int x){decayModePlus = x;}

  void setDecayModeBoson(int x){decayModeBoson = x;}

  void setGenBosonP4(const TLorentzVector &p4, const TLorentzVector &visP4) {bosP4 = p4; bosVisP4 = visP4; }

  void setGenPV(const TVector3 & aPV) {genPV = aPV;}

  void setAODPV(const TVector3 & aPV) {AODPV = aPV;}

  void setRefittedPV(const TVector3 & aPV) {refittedPV = aPV;}

  void setIsRefit(bool aBit){isRefit = aBit;};

  void setNTracksInRefit(const int & nTracks) {nTracksInRefit = nTracks;};

  void setSelectionBit(SelectionBitsEnum iBit, bool value = true) {selectionWord.SetBitNumber((int)iBit, value);}

  void setMET(const TVector2 &aVector) {met = aVector;}

  void setMET_uncorr(const TVector2 &aVector) {met_uncorr = aVector;}

  void setMETFilterDecision(unsigned int aMETFilterDecision) {metFilterDecision = aMETFilterDecision;}

  void setFilters(const std::vector<Int_t> & aFilters) { filters = aFilters;}

  ////////////////////////

  ///Reset class data members
  void clear();

  void clearSelectionWord() {selectionWord.ResetAllBits();}

  ///Data member getters.
  unsigned int getRunId() const {return runId;}

  unsigned long int getEventId() const {return eventId;}

  unsigned long int getLSId() const {return lsId;}

  float getNPU() const {return nPU;}

  unsigned int getNPV() const {return nPV;}

  float getRho() const {return rho;}

  float getMCatNLOWeight() const {return aMCatNLOweight;}

  float getPtReWeight() const {return ptReWeight;}

  float getPtReWeightR1() const {return ptReWeightR1;}

  float getPtReWeightSUSY() const {return ptReWeightSUSY;}

  float getMCWeight() const {return mcWeight;}

  float getLHE_Ht() const {return lheHt;}

  int getLHEnOutPartons() const {return lheNOutPartons;}

  sampleTypeEnum getSampleType() const {return sampleType;}

  int getDecayModeMinus() const {return decayModeMinus;}

  int getDecayModePlus() const {return decayModePlus;}

  int getDecayModeBoson() const {return decayModeBoson;}

  TLorentzVector getGenBosonP4(bool visP4=false) const { return visP4 ? bosVisP4 : bosP4 ; }

  TVector2 getMET() const {return met;}

  TVector2 getMET_uncorr() const {return met_uncorr;}

  const TVector3 & getGenPV() const {return genPV;}

  const TVector3 & getAODPV() const {return AODPV;}

  const TVector3 & getRefittedPV() const {return refittedPV;}

  bool getIsRefit() const {return isRefit;};

  int getNTracksInRefit() const {return nTracksInRefit;}

  bool checkSelectionBit(SelectionBitsEnum iBit) const {return selectionWord.TestBitNumber((unsigned int)iBit);}

  unsigned int getMETFilterDecision() const { return metFilterDecision;}

  int getFilter(FilterEnum index) const {return (unsigned int)index<filters.size()?  filters[(unsigned int)index]: -999;}

 private:

  ///Event run, run and LS number
  unsigned int runId;
  unsigned long int eventId, lsId;

  //Generator event weight
  float mcWeight;

  ///Weight used to modify the pt shape.
  float ptReWeight, ptReWeightR1, ptReWeightSUSY;

  ///Ht value from LHE record.
  float lheHt;

  ///Number of outgoing partons from LHE record
  int   lheNOutPartons;

  ///MCatNLO weight
  float aMCatNLOweight;

  ///Number of true PU vertices from MC
  float nPU;

  //Number of reocnstructed PV
  unsigned int nPV;

  ///PU energy density with FastJet, rho
  float rho;

  ///Type of the physics process or DATA
  sampleTypeEnum sampleType;

  ///Boson (H, Z, W) decay mode
  int decayModeBoson;

  ///Boson (H, Z, W) p4 and visible p4
  TLorentzVector bosP4, bosVisP4;

  ///Tau decay modes
  int decayModeMinus, decayModePlus;

  ///Primary Vertices recontructed with different methods
  //Generated PV position
  TVector3 genPV;

  //PV stored in miniAOD
  TVector3 AODPV;

  ///PV recontructed from PF candidates, refitted
  TVector3 refittedPV;

  ///Flag marking if refit was successfull
  bool isRefit;

  ///Number of tracks used in the refit
  int nTracksInRefit;

  ///Bit word coding event selection result
  TBits selectionWord;

  //MET vector, uncorrected
  TVector2 met_uncorr;

  //MET vector, recoil corr
  TVector2 met;

  //MET filter decision
  unsigned int metFilterDecision;

  std::vector<Int_t> filters;

};

class HTTParticle{

  public:

  HTTParticle(){ clear();}

  ~HTTParticle(){}

  void clear();

  ///Data member setters.
  void setP4(const TLorentzVector &aP4) { p4 = aP4;}

  void setChargedP4(const TLorentzVector &aP4) { chargedP4 = aP4;}

  void setNeutralP4(const TLorentzVector &aP4) { neutralP4 = aP4;}

  void setPCA(const TVector3 &aV3) {pca = aV3;}

  void setPCARefitPV(const TVector3 &aV3) {pcaRefitPV = aV3;}

  void setPCAGenPV(const TVector3 &aV3) {pcaGenPV = aV3;}

  void setProperties(const std::vector<Double_t> & aProperties) { properties = aProperties;}

  ///Data member getters.
  const TLorentzVector & getP4(HTTAnalysis::sysEffects type=HTTAnalysis::NOMINAL) const {return getSystScaleP4(type);}

  const TLorentzVector & getChargedP4() const {return chargedP4;}

  const TLorentzVector getNeutralP4() const {return neutralP4;}

  const TVector3 & getPCA() const {return pca;}

  const TVector3 & getPCARefitPV() const {return pcaRefitPV;}

  const TVector3 & getPCAGenPV() const {return pcaGenPV;}

  int getPDGid() const {return getProperty(PropertyEnum::pdgId);}

  int getCharge() const {return getProperty(PropertyEnum::charge);}

  Double_t getProperty(PropertyEnum index) const {return (unsigned int)index<properties.size()?  properties[(unsigned int)index]: -999;}

  bool hasTriggerMatch(TriggerEnum index) const {return (unsigned int)getProperty(PropertyEnum::isGoodTriggerType)& (1<<(unsigned int)index) &&
                                                        (unsigned int)getProperty(PropertyEnum::FilterFired)& (1<<(unsigned int)index);}

 private:

  ///Return four-momentum modified according DATA/MC energy scale factors.
  const TLorentzVector & getNominalShiftedP4() const;

  ///Return four-momentum modified according to given systematic effect.
  ///The method recognises particle type, e.g. muons are not affected by
  ///TES variations etc.
  const TLorentzVector & getSystScaleP4(HTTAnalysis::sysEffects type=HTTAnalysis::NOMINAL) const;

  ///Return four-momentum shifted with scale.
  ///Shift modifies three-momentum transverse part only, leaving mass constant.
  const TLorentzVector & getShiftedP4(float scale, bool preserveMass=true) const;

  ///Nominal (as recontructed) four-momentum
  TLorentzVector p4;

  ///Scaled four-momentum cache;
  mutable TLorentzVector p4Cache;
  mutable HTTAnalysis::sysEffects lastSystEffect;

  ///Charged and neutral components four-momentum
  TLorentzVector chargedP4, neutralP4;

  ///Vectors from primary vertex to point of closest approach (PCA)
  ///calculated with respect to AOD vertex, refitted and generated vertex.
  TVector3 pca, pcaRefitPV, pcaGenPV;

  ///Vector of various particle properties.
  ///Index generated automatically during conversion from
  ///LLR ntuple format
  std::vector<Double_t> properties;

  //Corrections of nominal tau-scale: https://twiki.cern.ch/twiki/bin/view/CMS/TauIDRecommendation13TeV#Tau_energy_scale 
  /*dummy
  static constexpr float TES_1p=0.0;
  static constexpr float TES_1ppi0=0.0;
  static constexpr float TES_3p=0.0;
  */
  /*early 2016 as SM HTT*/
  static constexpr float TES_1p=-0.018;
  static constexpr float TES_1ppi0=0.010;
  static constexpr float TES_3p=0.004;
  /**/
  /*final 2016
  static constexpr float TES_1p=-0.005;
  static constexpr float TES_1ppi0=0.011;
  static constexpr float TES_3p=0.006;
  */
  static constexpr float TES = 0.012;
  static constexpr float EES = 0.03;
  static constexpr float MES = 0.03;
  
};
///////////////////////////////////////////////////
///////////////////////////////////////////////////
class HTTPair{

 public:

  HTTPair(){ clear();}

  ~HTTPair(){}

  void clear();

  ///Data member setters.
  void setP4(const TLorentzVector &aP4, HTTAnalysis::sysEffects type = HTTAnalysis::NOMINAL) {p4Vector[(unsigned int)type] = aP4;}

  void setLeg1P4(const TLorentzVector &aP4, HTTAnalysis::sysEffects type = HTTAnalysis::NOMINAL) {leg1p4Vector[(unsigned int)type] = aP4;}

  void setLeg2P4(const TLorentzVector &aP4, HTTAnalysis::sysEffects type = HTTAnalysis::NOMINAL) {leg2p4Vector[(unsigned int)type] = aP4;}
  
  void setMET(const TVector2 &aVector) {met = aVector;}

  void setSVMET(const TVector2 &aVector, HTTAnalysis::sysEffects type = HTTAnalysis::NOMINAL) {svMetVector[(unsigned int)type] = aVector;}

  void setMTLeg1(const float & aMT) {mtLeg1 = aMT;}

  void setMTLeg2(const float & aMT) {mtLeg2 = aMT;}

  void setLeg1(const HTTParticle &aParticle, int idx=-1){leg1 = aParticle; indexLeg1=idx;}
 
  void setLeg2(const HTTParticle &aParticle, int idx=-1){leg2 = aParticle; indexLeg2=idx;}

  void setMETMatrix(float m00, float m01, float m10, float m11) {metMatrix.push_back(m00); metMatrix.push_back(m01); metMatrix.push_back(m10); metMatrix.push_back(m11);}

  ///Data member getters.
  const TLorentzVector & getP4(HTTAnalysis::sysEffects type = HTTAnalysis::NOMINAL) const;

  const TLorentzVector & getLeg1P4(HTTAnalysis::sysEffects type = HTTAnalysis::NOMINAL) const;

  const TLorentzVector & getLeg2P4(HTTAnalysis::sysEffects type = HTTAnalysis::NOMINAL) const;

  const TVector2 & getMET(HTTAnalysis::sysEffects type = HTTAnalysis::NOMINAL) const {return getSystScaleMET(type);}

  const TVector2 & getSVMET(HTTAnalysis::sysEffects type = HTTAnalysis::NOMINAL) const {return svMetVector[(unsigned int)type];}

  float getMTLeg1(HTTAnalysis::sysEffects type = HTTAnalysis::NOMINAL) const {return getSystScaleMT(leg1, type);}

  float getMTLeg2(HTTAnalysis::sysEffects type = HTTAnalysis::NOMINAL) const {return getSystScaleMT(leg2, type);}

  const HTTParticle & getLeg1() const {return leg1;}

  const HTTParticle & getLeg2() const {return leg2;}

  int getIndexLeg1() {return indexLeg1;}

  int getIndexLeg2() {return indexLeg2;}

  const HTTParticle & getMuon() const {return abs(leg1.getPDGid())==13 ? leg1 : leg2; }

  const HTTParticle & getTau() const {return abs(leg1.getPDGid())==15 ? leg1 : leg2; }

  float getMTMuon(HTTAnalysis::sysEffects type = HTTAnalysis::NOMINAL) const {return abs(leg1.getPDGid())==13 ? getMTLeg1(type) : getMTLeg2(type); }

  std::vector<float> getMETMatrix() const {return metMatrix;}

 private:

  ///Return MET modified according to given systematic effect.
  ///The MET is corrected for accorging leptons corrections.
  ///The recoil correctino is not updated.
  const TVector2 & getSystScaleMET(HTTAnalysis::sysEffects type=HTTAnalysis::NOMINAL) const;

  ///Return transverse mass caluculated according to the scale shifts.
  float getSystScaleMT(const HTTParticle &aPerticle,
		       HTTAnalysis::sysEffects type=HTTAnalysis::NOMINAL) const;

  ///Nominal met as calculated from PF.
  ///Includes recoil corrections.
  TVector2 met;

  ///Scaled four-momentum cache;
  mutable TVector2 metCache;
  mutable HTTAnalysis::sysEffects lastSystEffect;
  mutable float mtCache;

  ///Vectors holding p4 and MET for
  ///for various scale variances.
  std::vector<TLorentzVector> p4Vector;
  std::vector<TLorentzVector> leg1p4Vector;
  std::vector<TLorentzVector> leg2p4Vector;
  std::vector<TVector2> svMetVector;

  //MVAMET covariance matrix in order 00,01,10,11
  std::vector<float> metMatrix;

  ///MT calculated for (leg1,MET) and (leg2,MET)
  float mtLeg1, mtLeg2;

  ///Pair legs
  HTTParticle leg1, leg2;
  int indexLeg1, indexLeg2;

};

#endif
