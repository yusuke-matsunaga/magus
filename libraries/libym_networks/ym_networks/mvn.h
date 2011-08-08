#ifndef YM_NETWORKS_MVN_H
#define YM_NETWORKS_MVN_H

/// @file ym_networks/mvn.h
/// @brief mvn の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/networks_nsdef.h"
#include "ym_utils/DlList.h"


/// @brief mvn 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_NETWORKS_MVN		\
  BEGIN_NAMESPACE_YM_NETWORKS			\
  BEGIN_NAMESPACE(nsMvn)

/// @brief mvn 用の名前空間の終了
#define END_NAMESPACE_YM_NETWORKS_MVN		\
  END_NAMESPACE(nsMvn)				\
  END_NAMESPACE_YM_NETWORKS

/// @brief MvnVerilogReader 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_NETWORKS_MVN_VERILOG	\
  BEGIN_NAMESPACE_YM_NETWORKS_MVN		\
  BEGIN_NAMESPACE(nsVerilog)

/// @brief MvnVerilogReader 用の名前空間の終了
#define END_NAMESPACE_YM_NETWORKS_MVN_VERILOG	\
  END_NAMESPACE(nsVerilog)			\
  END_NAMESPACE_YM_NETWORKS_MVN


BEGIN_NAMESPACE_YM_NETWORKS_MVN

// クラス名の先行宣言

class MvnMgr;
class MvnModule;
class MvnPort;
class MvnPortRef;
class MvnNode;
class MvnInputPin;
class MvnOutputPin;

typedef DlList<MvnInputPin> MvnInputPinList;

class MvnVerilogReader;
class MvnVlMap;

class MvnDumper;
class MvnVerilogWriter;

END_NAMESPACE_YM_NETWORKS_MVN

BEGIN_NAMESPACE_YM

using nsNetworks::nsMvn::MvnMgr;
using nsNetworks::nsMvn::MvnModule;
using nsNetworks::nsMvn::MvnPort;
using nsNetworks::nsMvn::MvnPortRef;
using nsNetworks::nsMvn::MvnNode;
using nsNetworks::nsMvn::MvnInputPin;
using nsNetworks::nsMvn::MvnInputPinList;
using nsNetworks::nsMvn::MvnOutputPin;

using nsNetworks::nsMvn::MvnVerilogReader;
using nsNetworks::nsMvn::MvnVlMap;

using nsNetworks::nsMvn::MvnDumper;
using nsNetworks::nsMvn::MvnVerilogWriter;

END_NAMESPACE_YM

#endif // YM_NETWORKS_MVN_H
