#ifndef VLDUMPER_H
#define VLDUMPER_H

/// @file VlDumper.h
/// @brief VlDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlMgr;
class VlDumperImpl;

//////////////////////////////////////////////////////////////////////
/// @class VlDumper VlDumper.h <ym_YmVerilog/vl/VlDumper.h>
/// @ingroup Vlgroup
/// @brief VlObj の出力用クラス
//////////////////////////////////////////////////////////////////////
class VlDumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力ストリーム
  VlDumper(ostream& s);

  /// @brief デストラクタ
  ~VlDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を出力する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief VlMgr の持っている内容を出力する
  /// @param[in] vlmgr VlMgr
  void
  operator()(const VlMgr& vlmgr);


public:
  //////////////////////////////////////////////////////////////////////
  // オプションの設定/取得用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置情報を出力するモードにする
  void
  enable_file_loc_mode();

  /// @brief ファイル位置情報の出力を抑制するモードにする．
  void
  disable_file_loc_mode();

  /// @brief ファイル位置情報を出力するモードのとき true を返す．
  bool
  file_loc_mode() const;

  /// @brief NULL ポインタの出力を抑止する．
  void
  enable_nullptr_suppress_mode();

  /// @brief NULL ポインタの出力を行う．
  void
  disable_nullptr_suppress_mode();

  /// @brief NULL ポインタの出力抑止モードの状態を得る．
  bool
  nullptr_suppress_mode() const;

  /// @brief bit 展開した内容を出力する．
  void
  enable_bitexpand_mode();

  /// @brief bit 展開しない．
  void
  disable_bitexpand_mode();

  /// @brief bit 展開モードの時 true を返す．
  bool
  bitexpand_mode() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の実装クラス
  VlDumperImpl* mImpl;

};


END_NAMESPACE_YM_VERILOG

#endif // VLDUMPER_H
