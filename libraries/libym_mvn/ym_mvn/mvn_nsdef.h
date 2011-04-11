#ifndef YM_MVN_MVN_NSDEF_H
#define YM_MVN_MVN_NSDEF_H

/// @file ym_mvn/mvn_nsdef.h
/// @brief mvn の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/DlList.h"


/// @brief mvn 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_MVN			\
  BEGIN_NAMESPACE_YM				\
  BEGIN_NAMESPACE(nsMvn)

/// @brief mvn 用の名前空間の終了
#define END_NAMESPACE_YM_MVN			\
  END_NAMESPACE(nsMvn)				\
  END_NAMESPACE_YM

/// @brief verilog reader 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_MVN_VERILOG		\
  BEGIN_NAMESPACE_YM_MVN			\
  BEGIN_NAMESPACE(nsVerilog)

/// @brief verilog reader 用の名前空間の終了
#define END_NAMESPACE_YM_MVN_VERILOG		\
  END_NAMESPACE(nsVerilog)			\
  END_NAMESPACE_YM_MVN


BEGIN_NAMESPACE_YM_MVN

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

END_NAMESPACE_YM_MVN

BEGIN_NAMESPACE_YM

using nsMvn::MvnMgr;
using nsMvn::MvnModule;
using nsMvn::MvnPort;
using nsMvn::MvnPortRef;
using nsMvn::MvnNode;
using nsMvn::MvnInputPin;
using nsMvn::MvnInputPinList;
using nsMvn::MvnOutputPin;
using nsMvn::MvnNet;

using nsMvn::MvnVerilogReader;
using nsMvn::MvnVlMap;

using nsMvn::MvnDumper;
using nsMvn::MvnVerilogWriter;

END_NAMESPACE_YM

#endif // YM_MVN_MVN_NSDEF_H
