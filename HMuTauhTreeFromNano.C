#define HMuTauhTreeFromNano_cxx
#include "HMuTauhTreeFromNano.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TSystem.h>

#include <iostream>
#include <fstream>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HMuTauhTreeFromNano::pairSelection(unsigned int iPair){

  ///Baseline+post sync selection as on
  ///https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorking2016#Baseline_mu_tau_h
  ///Indices for multiplexed ID variables taken from   LLRHiggsTauTau/NtupleProducer/plugins/
  ///HTauTauNtuplizer.cc, MuFiller.cc, TauFiller.cc, EleFiller.cc

  if (event==check_event_number) cout << "pS1 " << endl;

  if(httPairs_.empty()) return false;

  if (event==check_event_number) cout << "pS2 " << endl;

  int pdgIdLeg1 = httPairs_[iPair].getLeg1().getPDGid();
  int pdgIdLeg2 = httPairs_[iPair].getLeg2().getPDGid();
  unsigned int indexMuonLeg = -1;
  if(std::abs(pdgIdLeg1)==13) indexMuonLeg = httPairs_[iPair].getIndexLeg1();
  else if(std::abs(pdgIdLeg2)==13) indexMuonLeg = httPairs_[iPair].getIndexLeg2();
  else return 0;

  if (event==check_event_number) cout << "pS3 " << endl;

  unsigned int indexTauLeg = -1;
  if(std::abs(pdgIdLeg1)==15) indexTauLeg = httPairs_[iPair].getIndexLeg1();
  else if(std::abs(pdgIdLeg2)==15) indexTauLeg = httPairs_[iPair].getIndexLeg2();
  else return 0;

  if (event==check_event_number) cout << "pS4 " << endl;

  int tauIDmask = 0;
  int tauID = (int)httLeptonCollection[indexTauLeg].getProperty(PropertyEnum::idAntiMu);
  tauID += (int)std::pow(2,HTTEvent::againstEIdOffset)*(int)httLeptonCollection[indexTauLeg].getProperty(PropertyEnum::idAntiEle);
  tauID += (int)std::pow(2,HTTEvent::mvaIsoIdOffset)*(int)httLeptonCollection[indexTauLeg].getProperty(PropertyEnum::idMVAoldDM);

  for(unsigned int iBit=0;iBit<HTTEvent::ntauIds;iBit++){
    if(HTTEvent::tauIDStrings[iBit]=="byTightIsolationMVArun2v1DBoldDMwLT") tauIDmask |= (1<<iBit);
    if(HTTEvent::tauIDStrings[iBit]=="againstMuonTight3") tauIDmask |= (1<<iBit);
    if(HTTEvent::tauIDStrings[iBit]=="againstElectronVLooseMVA6") tauIDmask |= (1<<iBit);
  }

  TLorentzVector muonP4 = httLeptonCollection[indexMuonLeg].getP4();
  TLorentzVector tauP4 = httLeptonCollection[indexTauLeg].getP4();

  //  if (event==check_event_number) cout << "pS4 " << endl;

  //Should be fine with NanoAOD?
  //int muonIdBit = 7;//Standard Medium ID
  //if(RunNumber<278808 && RunNumber>100000) muonIdBit = 6;//ICHEP Medium MuonID

  //  float eps=0;
  //  if (tweak_nano && event==1171228) eps=0.0001; //this is needed to make up for NanoAOD precision difference to MiniAOD...
  //  bool muonBaselineSelection =  muonP4.Pt()>20 && std::abs(muonP4.Eta())<=(2.1+eps) &&
  bool muonBaselineSelection =  muonP4.Pt()>20 && std::abs(muonP4.Eta())<=2.1 &&
    std::abs(httLeptonCollection[indexMuonLeg].getProperty(PropertyEnum::dz))<0.2 &&
    std::abs(httLeptonCollection[indexMuonLeg].getProperty(PropertyEnum::dxy))<0.045; // &&
  //    httLeptonCollection[indexMuonLeg].getProperty(PropertyEnum::mediumId)>0;

  if (event==check_event_number) cout << "pS4a " << muonP4.Pt() << " " << muonP4.Eta() << " " << std::abs(httLeptonCollection[indexMuonLeg].getProperty(PropertyEnum::dz)) << " " << std::abs(httLeptonCollection[indexMuonLeg].getProperty(PropertyEnum::dxy))<< " " << (int)std::abs(httLeptonCollection[indexMuonLeg].getProperty(PropertyEnum::mediumId))<<  endl;


  bool tauBaselineSelection = tauP4.Pt()>30 && std::abs(tauP4.Eta())<2.3 &&
    httLeptonCollection[indexTauLeg].getProperty(PropertyEnum::idDecayMode)>0.5 &&
    std::abs(httLeptonCollection[indexTauLeg].getProperty(PropertyEnum::dz))<0.2 &&
    (int)std::abs(httLeptonCollection[indexTauLeg].getProperty(PropertyEnum::charge))==1;

  if (event==check_event_number) cout << "pS4b " << tauP4.Pt() << " " << tauP4.Eta() << " " << httLeptonCollection[indexTauLeg].getProperty(PropertyEnum::idDecayMode) << " " << std::abs(httLeptonCollection[indexTauLeg].getProperty(PropertyEnum::dz)) << " " << (int)std::abs(httLeptonCollection[indexTauLeg].getProperty(PropertyEnum::charge))<<  endl;


  /*
  bool triggerSelection_singlemu = muonP4.Pt()>23 &&
    ( httLeptonCollection[indexMuonLeg].hasTriggerMatch(TriggerEnum::HLT_IsoMu22) ||
      httLeptonCollection[indexMuonLeg].hasTriggerMatch(TriggerEnum::HLT_IsoMu22_eta2p1) ||
      httLeptonCollection[indexMuonLeg].hasTriggerMatch(TriggerEnum::HLT_IsoTkMu22) ||
      httLeptonCollection[indexMuonLeg].hasTriggerMatch(TriggerEnum::HLT_IsoTkMu22_eta2p1) );

  bool triggerSelection_mutau = 
    ( httLeptonCollection[indexMuonLeg].hasTriggerMatch(TriggerEnum::HLT_IsoMu19_eta2p1_LooseIsoPFTau20) && httLeptonCollection[indexTauLeg].hasTriggerMatch(TriggerEnum::HLT_IsoMu19_eta2p1_LooseIsoPFTau20) ) ||
    ( httLeptonCollection[indexMuonLeg].hasTriggerMatch(TriggerEnum::HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1) && httLeptonCollection[indexTauLeg].hasTriggerMatch(TriggerEnum::HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1) && muonP4.Pt()<23 );

  bool triggerSelection = triggerSelection_singlemu || triggerSelection_mutau;
  */

  bool baselinePair = muonP4.DeltaR(tauP4) > 0.5;
  bool postSynchMuon = httLeptonCollection[indexMuonLeg].getProperty(PropertyEnum::pfRelIso04_all)<0.15;
  bool loosePostSynchMuon = httLeptonCollection[indexMuonLeg].getProperty(PropertyEnum::pfRelIso04_all)<0.3;
  bool postSynchTau = (tauID & tauIDmask) == tauIDmask;

  ///SUSY synch selection
  //muonBaselineSelection &= muonP4.Pt()>23 && std::abs(muonP4.Eta())<2.1;
  //tauBaselineSelection &= tauP4.Pt()>30 && std::abs(tauP4.Eta())<2.3;
  ///////////////////////

  httEvent->clearSelectionWord();
  httEvent->setSelectionBit(SelectionBitsEnum::muonBaselineSelection,muonBaselineSelection);
  httEvent->setSelectionBit(SelectionBitsEnum::tauBaselineSelection,tauBaselineSelection);
  httEvent->setSelectionBit(SelectionBitsEnum::baselinePair,baselinePair);
  httEvent->setSelectionBit(SelectionBitsEnum::postSynchMuon,postSynchMuon);
  httEvent->setSelectionBit(SelectionBitsEnum::postSynchTau,postSynchTau);
  httEvent->setSelectionBit(SelectionBitsEnum::diMuonVeto,diMuonVeto());
  httEvent->setSelectionBit(SelectionBitsEnum::extraMuonVeto,thirdLeptonVeto(indexMuonLeg, indexTauLeg, 13));
  httEvent->setSelectionBit(SelectionBitsEnum::extraElectronVeto,thirdLeptonVeto(indexMuonLeg, indexTauLeg, 11));

  if (event==check_event_number) cout << "pS5 " << muonBaselineSelection << " " << tauBaselineSelection << " " << baselinePair << endl;

  return muonBaselineSelection && tauBaselineSelection && baselinePair
    //&& postSynchTau && loosePostSynchMuon //comment out for sync
    //&& !diMuonVeto() && !thirdLeptonVeto(indexMuonLeg, indexTauLeg, 13) && !thirdLeptonVeto(indexMuonLeg, indexTauLeg, 11) //comment out for sync
    && true;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HMuTauhTreeFromNano::diMuonVeto(){

  std::vector<int> muonIndexes;
  for(unsigned int iLepton=0;iLepton<httLeptonCollection.size();++iLepton){

    if(std::abs(httLeptonCollection[iLepton].getPDGid())!=13) continue;
    TLorentzVector muonP4 = httLeptonCollection[iLepton].getP4();

    bool passLepton = muonP4.Pt()> 15 && std::abs(muonP4.Eta())<2.4 &&
      std::abs(httLeptonCollection[iLepton].getProperty(PropertyEnum::dz))<0.2 &&
      std::abs(httLeptonCollection[iLepton].getProperty(PropertyEnum::dxy))<0.045 &&
       httLeptonCollection[iLepton].getProperty(PropertyEnum::pfRelIso04_all)<0.3 &&
      //FIXME((daughters_typeOfMuon->at(iLepton) & ((1<<0) + (1<<1) + (1<<2))) == ((1<<0) + (1<<1) + (1<<2))) && //0=PF, 1=Global, 2=Tracker, muons in Nano are loose, i.e. PF&(Global|Tracker)
      true;

       if(passLepton) muonIndexes.push_back(iLepton);
  }

  if(muonIndexes.size()<2) return false;

  else{
    for(unsigned int iMuon1=0;iMuon1<muonIndexes.size()-1;++iMuon1){
      for(unsigned int iMuon2=iMuon1+1;iMuon2<muonIndexes.size();++iMuon2){
	TLorentzVector muon1P4 = httLeptonCollection[iMuon1].getP4();
	int muon1Charge = (int)httLeptonCollection[iMuon1].getProperty(PropertyEnum::charge);
	TLorentzVector muon2P4 = httLeptonCollection[iMuon2].getP4();
	int muon2Charge = (int)httLeptonCollection[iMuon2].getProperty(PropertyEnum::charge);
	float deltaR = muon1P4.DeltaR(muon2P4);
	if(muon2Charge*muon1Charge==-1 &&
	   deltaR>0.15) return true;
      }
    }
  }
  return false;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
