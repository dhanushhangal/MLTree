/**
 * @file    MLTreeMaker.h
 * @author  Joakim Olsson <joakim.olsson@cern.ch>
 * @brief   Athena package to save cell images of clusters for ML training 
 * @date    October 2016
 */

#ifndef MLTREE_MLTREEMAKER_H
#define MLTREE_MLTREEMAKER_H

#include <string>

#include "AthenaBaseComps/AthHistogramAlgorithm.h"
#include "AthenaBaseComps/AthAlgorithm.h"
#include "GaudiKernel/ToolHandle.h"
#include "GaudiKernel/ServiceHandle.h"
#include "CaloIdentifier/CaloCell_ID.h"
#include "RecoToolInterfaces/IParticleCaloExtensionTool.h"
#include "xAODCaloEvent/CaloClusterContainer.h"
#include "xAODCaloEvent/CaloClusterChangeSignalState.h"

#include "AthExOnnxRuntime/AthExOnnxRuntime.h"

// Local include(s).
#include "AthOnnxruntimeService/IONNXRuntimeSvc.h"

// Framework include(s).
#include "AthenaBaseComps/AthReentrantAlgorithm.h"

// ONNX Runtime include(s).
#include <core/session/onnxruntime_cxx_api.h>

// System include(s).
#include <memory>
#include <iostream> 
#include <fstream>
#include <arpa/inet.h>
#include <vector>
#include <iterator>

class TileTBID;
class ICaloSurfaceHelper;

namespace Trk {
  class IExtrapolator;
  class Surface;
  class TrackParametersIdHelper;
}

namespace InDet {
    class IInDetTrackSelectionTool;
}

class MLTreeMaker: public ::AthHistogramAlgorithm { 

  public: 
    MLTreeMaker( const std::string& name, ISvcLocator* pSvcLocator );
    virtual ~MLTreeMaker(); 

    virtual StatusCode  initialize();
    virtual StatusCode  execute();
    virtual StatusCode  finalize();

  private: 
    bool m_doEventTree;
    bool m_doClusterTree;
    bool m_doClusterMoments;
    bool m_doUncalibratedClusters;
    // bool m_isMC;
    bool m_doTracking;
  bool m_doJets;
    bool m_doEventCleaning;
    bool m_doPileup;
    bool m_doShapeEM;
    bool m_doShapeLC;
    bool m_doEventTruth;
  bool m_doTruthParticles;
  bool m_keepOnlyStableTruthParticles;
    std::string m_prefix;
    std::string m_eventInfoContainerName;
    std::string m_truthContainerName;
    std::string m_vxContainerName;
    std::string m_trackContainerName;
    std::string m_caloClusterContainerName;
  std::vector<std::string> m_jetContainerNames;
    ToolHandle<Trk::IExtrapolator> m_extrapolator;
    ToolHandle<Trk::IParticleCaloExtensionTool> m_theTrackExtrapolatorTool;
    ToolHandle<InDet::IInDetTrackSelectionTool> m_trkSelectionTool;
    Trk::TrackParametersIdHelper* m_trackParametersIdHelper;
    ToolHandle<ICaloSurfaceHelper> m_surfaceHelper;
    const TileTBID* m_tileTBID; 

    // Retrieve tree
    TTree* m_eventTree;
    TTree* m_clusterTree;

    // Cluster and cell selections
    float m_clusterE_min;
    float m_clusterE_max;
    float m_clusterEtaAbs_max;
    float m_cellE_thres;

    //// Add to eventTree

    // Event info
    int      m_runNumber;
    Long64_t m_eventNumber;
    int      m_lumiBlock;
    uint32_t m_coreFlags;
    uint32_t m_timeStamp;
    uint32_t m_timeStampNSOffset;
    bool     m_TileError;
    bool     m_LArError;
    bool     m_SCTError;
    uint32_t m_TileFlags;
    uint32_t m_LArFlags;
    uint32_t m_SCTFlags;
    int      m_mcEventNumber;
    int      m_mcChannelNumber;
    float    m_mcEventWeight;
    float    m_weight_pileup;
    float    m_correct_mu;
    int      m_rand_run_nr;
    int      m_rand_lumiblock_nr;
    int      m_bcid;
    float    m_prescale_DataWeight;
    // pileup
    int      m_npv;
    float    m_actualMu;
    float    m_averageMu;
    // shapeEM
    double   m_rhoEM;
    double   m_rhoLC;
    // truth
    int      m_pdgId1;
    int      m_pdgId2;
    int      m_pdfId1;
    int      m_pdfId2;
    float    m_x1;
    float    m_x2;
    // float m_scale;
    // float    m_q;
    // float m_pdf1;
    // float m_pdf2;
    float    m_xf1;
    float    m_xf2;

    // Truth particles
    int m_nTruthPart;
    std::vector<int>   m_pdgId;
    std::vector<int>   m_status;
    std::vector<int>   m_barcode;
    std::vector<float> m_truthPartPt;
    std::vector<float> m_truthPartE;
    std::vector<float> m_truthPartMass;
    std::vector<float> m_truthPartEta;
    std::vector<float> m_truthPartPhi;

    // Track variables
    int m_nTrack;
    std::vector<float> m_trackPt;
    std::vector<float> m_trackP;
    std::vector<float> m_trackMass;
    std::vector<float> m_trackEta;
    std::vector<float> m_trackPhi;

    // Track extrapolation
    // Presampler
    std::vector<float> m_trackEta_PreSamplerB;
    std::vector<float> m_trackPhi_PreSamplerB;
    std::vector<float> m_trackEta_PreSamplerE;
    std::vector<float> m_trackPhi_PreSamplerE;
    // LAr EM Barrel layers
    std::vector<float> m_trackEta_EMB1; 
    std::vector<float> m_trackPhi_EMB1; 
    std::vector<float> m_trackEta_EMB2; 
    std::vector<float> m_trackPhi_EMB2; 
    std::vector<float> m_trackEta_EMB3; 
    std::vector<float> m_trackPhi_EMB3; 
    // LAr EM Endcap layers
    std::vector<float> m_trackEta_EME1; 
    std::vector<float> m_trackPhi_EME1; 
    std::vector<float> m_trackEta_EME2; 
    std::vector<float> m_trackPhi_EME2; 
    std::vector<float> m_trackEta_EME3; 
    std::vector<float> m_trackPhi_EME3; 
    // Hadronic Endcap layers
    std::vector<float> m_trackEta_HEC0; 
    std::vector<float> m_trackPhi_HEC0; 
    std::vector<float> m_trackEta_HEC1; 
    std::vector<float> m_trackPhi_HEC1; 
    std::vector<float> m_trackEta_HEC2; 
    std::vector<float> m_trackPhi_HEC2; 
    std::vector<float> m_trackEta_HEC3; 
    std::vector<float> m_trackPhi_HEC3; 
    // Tile Barrel layers
    std::vector<float> m_trackEta_TileBar0; 
    std::vector<float> m_trackPhi_TileBar0; 
    std::vector<float> m_trackEta_TileBar1; 
    std::vector<float> m_trackPhi_TileBar1; 
    std::vector<float> m_trackEta_TileBar2; 
    std::vector<float> m_trackPhi_TileBar2; 
    // Tile Gap layers
    std::vector<float> m_trackEta_TileGap1; 
    std::vector<float> m_trackPhi_TileGap1; 
    std::vector<float> m_trackEta_TileGap2; 
    std::vector<float> m_trackPhi_TileGap2; 
    std::vector<float> m_trackEta_TileGap3; 
    std::vector<float> m_trackPhi_TileGap3; 
    // Tile Extended Barrel layers
    std::vector<float> m_trackEta_TileExt0;
    std::vector<float> m_trackPhi_TileExt0;
    std::vector<float> m_trackEta_TileExt1;
    std::vector<float> m_trackPhi_TileExt1;
    std::vector<float> m_trackEta_TileExt2;
    std::vector<float> m_trackPhi_TileExt2;

    // Clusters and cells 
    long m_clusterCount;
    int m_nCluster;
    std::vector<int> m_cluster_nCells;
    std::vector<float> m_clusterE;
    std::vector<float> m_clusterPt;
    std::vector<float> m_clusterEta;
    std::vector<float> m_clusterPhi;
    std::vector<float> m_cluster_sumCellE;
    std::vector<float> m_cluster_cell_dEta;
    std::vector<float> m_cluster_cell_dPhi;
    std::vector<float> m_cluster_cell_dR_min;
    std::vector<float> m_cluster_cell_dR_max;
    std::vector<float> m_cluster_cell_dEta_min;
    std::vector<float> m_cluster_cell_dEta_max;
    std::vector<float> m_cluster_cell_dPhi_min;
    std::vector<float> m_cluster_cell_dPhi_max;

    std::vector<float> m_cluster_cell_centerCellEta;
    std::vector<float> m_cluster_cell_centerCellPhi;
    std::vector<int>   m_cluster_cell_centerCellLayer;
  std::vector<std::vector<float> > m_jet_pt;
  std::vector<std::vector<float> > m_jet_eta;
  std::vector<std::vector<float> > m_jet_phi;
  std::vector<std::vector<float> > m_jet_E;
  std::vector<std::vector<int> > m_jet_flavor;
  //// Add to clusterTree
    
    int   m_fCluster_nCells;
    float m_fClusterTruthE;
    float m_fClusterTruthPt;
    float m_fClusterTruthEta;
    float m_fClusterTruthPhi;
  int   m_fClusterIndex;
    float m_fClusterE;
    float m_fClusterECalib;
    float m_fClusterPt;
    float m_fClusterEta;
    float m_fClusterPhi;
    float m_fCluster_sumCellE;

  float m_fCluster_ENG_CALIB_TOT;
  float m_fCluster_ENG_pred;
  float m_fCluster_ENG_CALIB_OUT_T;
  float m_fCluster_ENG_CALIB_DEAD_TOT;

    float m_fCluster_EM_PROBABILITY;
  float m_fCluster_HAD_WEIGHT;
  float m_fCluster_OOC_WEIGHT;
  float m_fCluster_DM_WEIGHT;
  float m_fCluster_CENTER_MAG;
  float m_fCluster_FIRST_ENG_DENS;
  float m_fCluster_CENTER_LAMBDA;
  float m_fCluster_ISOLATION;
  float m_fCluster_ENERGY_DigiHSTruth;

    float m_fCluster_cell_dR_min;
    float m_fCluster_cell_dR_max;
    float m_fCluster_cell_dEta_min;
    float m_fCluster_cell_dEta_max;
    float m_fCluster_cell_dPhi_min;
    float m_fCluster_cell_dPhi_max;

    float m_fCluster_cell_centerCellEta;
    float m_fCluster_cell_centerCellPhi;
    int   m_fCluster_cell_centerCellLayer;

    std::vector<float> m_cluster_cellE_norm;

    // Images: eta x phi = 0.4 x 0.4 
  float m_PSB[16][4];
    float m_EMB1[128][4];
    float m_EMB2[16][16];
    float m_EMB3[8][16];
    float m_TileBar0[4][4];
    float m_TileBar1[4][4];
    float m_TileBar2[2][4];

    float m_EMB1_expand[128][4][1];
    float m_EMB23[16][16][2];
    float m_Tiles[4][4][3];

    int m_duplicate_PSB;
    int m_duplicate_EMB1;
    int m_duplicate_EMB2;
    int m_duplicate_EMB3;
    int m_duplicate_TileBar0;
    int m_duplicate_TileBar1;
    int m_duplicate_TileBar2;


  std::vector<float*> m_v_PSB;
  std::vector<float*> m_v_EMB1;
  std::vector<float*> m_v_EMB2;
  std::vector<float*> m_v_EMB3;
  std::vector<float*> m_v_TileBar0;
  std::vector<float*> m_v_TileBar1;
  std::vector<float*> m_v_TileBar2;

  std::vector<int*> m_v_duplicates;
  std::vector<std::vector<float*>*> m_v_images;

  static constexpr unsigned int NUMSAMPLINGS=7;
  static constexpr unsigned int NUMETABINS[NUMSAMPLINGS]={16,128,16,8,4,4,2};
  static constexpr unsigned int NUMPHIBINS[NUMSAMPLINGS]={4,4,16,16,4,4,4};
  static constexpr const char*  SAMPLINGNAMES[NUMSAMPLINGS]={"PSB","EMB1","EMB2","EMB3","TileBar0","TileBar1","TileBar2"};

  /// Name of the model file to load
  Gaudi::Property< std::string > m_modelFileName{ this, "ModelFileName",
     "dev/MLTest/2020-03-02/MNIST_testModel.onnx",
     "Name of the model file to load" };
  Gaudi::Property< std::string > m_pixelFileName{ this, "InputDataPixel",
     "dev/MLTest/2020-03-31/t10k-images-idx3-ubyte",
     "Name of the input pixel file to load" };
  Gaudi::Property< std::string > m_labelFileName{ this, "InputDataLabel",
     "dev/MLTest/2020-03-31/t10k-labels-idx1-ubyte",
     "Name of the label file to load" };
  Gaudi::Property<int> m_testSample {this, "TestSample", 0, "A Random Test Sample"};

  /// Following properties needed to be consdered if the .onnx model is evaluated in batch mode
  Gaudi::Property<bool> m_doBatches {this, "DoBatches", false, "Processing events by batches"};
  Gaudi::Property<int> m_numberOfBatches {this, "NumberOfBatches", 1, "No. of batches to be passed"};
  Gaudi::Property<int> m_sizeOfBatch {this, "SizeOfBatch", 1, "No. of elements/example in a batch"};
  
  /// Handle to @c AthONNX::IONNXRuntimeSvc
  ServiceHandle< AthONNX::IONNXRuntimeSvc > m_svc{ this, "ONNXRuntimeSvc",
                                          "AthONNX::ONNXRuntimeSvc",
                                          "Name of the service to use" };

  /// The "session" of ONNX Runtime that we'll be using
  std::unique_ptr< Ort::Session > m_session;
  std::vector<std::vector<float>> m_input_tensor_values;
  std::vector<int> m_output_tensor_values;

  size_t input_tensor_size_tiles = 1*4*4*3;
  size_t input_tensor_size_EMB23 = 1*16*16*2;
  size_t input_tensor_size_EMB1 = 1*128*4*1;

  std::vector<float> input_tensor_values_tiles_;
  std::vector<float> input_tensor_values_EMB23_;
  std::vector<float> input_tensor_values_EMB1_;

  std::vector<int64_t> input_node_dims;
  std::vector<int64_t> output_node_dims;

  size_t num_input_nodes;
  size_t num_output_nodes;

  std::vector<const char*> input_node_names;
  std::vector<const char*> output_node_names;

  std::vector<int64_t> input_node_dims_tiles, input_node_dims_EMB23, input_node_dims_EMB1;

}; 

#endif
