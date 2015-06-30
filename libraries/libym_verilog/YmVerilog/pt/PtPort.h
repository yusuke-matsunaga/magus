#ifndef VERILOG_PT_POPORT_H
#define VERILOG_PT_POPORT_H

/// @file YmVerilog/pt/PtPort.h
/// @brief PtPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtPort PtPort.h <YmVerilog/pt/PtPort.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief port を表すクラス
//////////////////////////////////////////////////////////////////////
class PtPort :
  public PtBase
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~PtPort() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外向の名前の取得
  /// @return 外向の名前(本当のポート名)\n
  /// 無い場合は NULL を返す
  virtual
  const char*
  ext_name() const = 0;

  /// @brief 内側のポート結線を表す式の取得
  virtual
  const PtExpr*
  portref() const = 0;

  /// @brief 内部のポート結線リストのサイズの取得
  virtual
  ymuint
  portref_size() const = 0;

  /// @brief 内部のポート結線リストの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  virtual
  const PtExpr*
  portref_elem(ymuint pos) const = 0;

  /// @brief 内部ポート結線の方向の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < portref_num() )
  virtual
  tVlDirection
  portref_dir(ymuint pos) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_PT_POPORT_H
