#ifndef VERILOG_PT_PTBASE_H
#define VERILOG_PT_PTBASE_H

/// @file YmVerilog/pt/PtBase.h
/// @brief Pt の基底クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/verilog.h"
#include "YmVerilog/pt/PtP.h"
#include "YmUtils/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtBase PtBase.h <ym_YmVerilog/pt/PtBase.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 全ての Pt クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class PtBase
{
public:

  /// @brief デストラクタ
  virtual
  ~PtBase() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class PtNamedBase PtBase.h <ym_YmVerilog/pt/PtBase.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 名前を持つ PtBase
//////////////////////////////////////////////////////////////////////
class PtNamedBase :
  public PtBase
{
public:

  /// @brief デストラクタ
  virtual
  ~PtNamedBase() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtNamedBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_PT_PTBASE_H
