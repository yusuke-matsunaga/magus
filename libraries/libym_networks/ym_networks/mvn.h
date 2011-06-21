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


/// @brief verilog reader 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_NETWORKS_VERILOG		\
  BEGIN_NAMESPACE_YM_NETWORKS			\
  BEGIN_NAMESPACE(nsVerilog)

/// @brief verilog reader 用の名前空間の終了
#define END_NAMESPACE_YM_NETWORKS_VERILOG		\
  END_NAMESPACE(nsVerilog)			\
  END_NAMESPACE_YM_NETWORKS


BEGIN_NAMESPACE_YM_NETWORKS

// クラス名の先行宣言

class MvnMgr;
class MvnModule;
class MvnPort;
class MvnPortRef;
class MvnNode;
class MvnInputPin;
class MvnOutputPin;
class MvnNet;

typedef DlList<MvnInputPin> MvnInputPinList;

class MvnVerilogReader;
class MvnVlMap;

class MvnDumper;
class MvnVerilogWriter;

END_NAMESPACE_YM_NETWORKS

BEGIN_NAMESPACE_YM

using nsNetworks::MvnMgr;
using nsNetworks::MvnModule;
using nsNetworks::MvnPort;
using nsNetworks::MvnPortRef;
using nsNetworks::MvnNode;
using nsNetworks::MvnInputPin;
using nsNetworks::MvnInputPinList;
using nsNetworks::MvnOutputPin;
using nsNetworks::MvnNet;

using nsNetworks::MvnVerilogReader;
using nsNetworks::MvnVlMap;

using nsNetworks::MvnDumper;
using nsNetworks::MvnVerilogWriter;

END_NAMESPACE_YM

#endif // YM_NETWORKS_MVN_H
