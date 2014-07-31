#ifndef VERILOG_VL_VLDOBJ_H
#define VERILOG_VL_VLDOBJ_H

/// @file YmVerilog/vl/VlObj.h
/// @brief VlObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/verilog.h"
#include "YmUtils/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlObj VlObj.h "ym_YmVerilog/vl/VlObj.h"
/// @brief エラボレーション中のオブジェクトを表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlObj() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const = 0;

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_VL_VLOBJ_H
