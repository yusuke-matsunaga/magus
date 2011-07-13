#ifndef YM_VERILOG_VLUDPVAL_H
#define YM_VERILOG_VLUDPVAL_H

/// @file ym_verilog/VlUdpVal.h
/// @brief VlUdpVal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: verilog.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlUdpVal VlUdpVal.h "ym_verilog/VlUdpVal.h"
/// @brief UDP のテーブルで使われる値を表すクラス
//////////////////////////////////////////////////////////////////////
class VlUdpVal
{
public:

  /// @brief 値を表す文字からのコンストラクタ
  explicit
  VlUdpVal(char symbol = '-');

  /// @brief 値を表す2つの文字からのコンストラクタ
  VlUdpVal(char symbol1,
	   char symbol2);

  /// @brief デストラクタ
  ~VlUdpVal();


public:

  /// @brief レベルシンボルの時に true を返す．
  bool
  is_level_symbol() const;

  /// @brief 遷移シンボル(エッジシンボル)の時に true を返す．
  bool
  is_edge_symbol() const;

  /// @brief No Change シンボルの時に true を返す．
  bool
  is_nc_symbol() const;

  /// @brief 複合値の時に true を返す．
  bool
  is_composite_symbol() const;

  /// @brief 文字列を返す．
  string
  to_string() const;

  /// @brief 2バイトの文字にする．
  ymuint16
  to_dbyte() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の値
  ymuint16 mData;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VLUDPVAL_H
