#ifndef VLWASSIGN_H
#define VLWASSIGN_H

/// @file VlwAssign.h
/// @brief VlwAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlwBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class VlwAssign VlwAssign.h "VlwAssign.h"
/// @brief assign 文を出力するためのクラス
//////////////////////////////////////////////////////////////////////
class VlwAssign :
  public VlwBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  VlwAssign(VlWriter& writer);

  /// @brief デストラクタ
  ~VlwAssign();


public:

  /// @brief 左辺を出力する．
  /// @param[in] name 左辺の信号線名
  void
  put_lhs(const char* name);

  /// @brief 左辺を出力する．
  /// @param[in] name 左辺の信号線名
  void
  put_lhs(const string& name);

  /// @brief 左辺を出力する．
  /// @param[in] name 左辺の信号線名
  /// @param[in] lsb 範囲の LSB
  /// @param[in] msb 範囲の MSB
  void
  put_lhs(const char* name,
	  int lsb,
	  int msb);

  /// @brief 左辺を出力する．
  /// @param[in] name 左辺の信号線名
  /// @param[in] lsb 範囲の LSB
  /// @param[in] msb 範囲の MSB
  void
  put_lhs(const string& name,
	  int lsb,
	  int msb);

  /// @brief 右辺を出力する．
  /// @param[in] expr 論理式
  /// @param[in] name_map 変数名の対応表
  /// @note expr 中の i 番めの変数の名前が name_map[i] に入っている．
  void
  put_rhs(const LogExpr& expr,
	  const VarStrMap& name_map);

};

END_NAMESPACE_YM_NETWORKS

#endif // VLWASSIGN_H
