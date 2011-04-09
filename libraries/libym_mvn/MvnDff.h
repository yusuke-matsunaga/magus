#ifndef LIBYM_MVN_MVNDFF_H
#define LIBYM_MVN_MVNDFF_H

/// @file libym_mvn/MvnDff.h
/// @brief MvnDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MvnNodeBase.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnDff MvnDff.h "MvnDff.h"
/// @brief D-FF を表すクラス
//////////////////////////////////////////////////////////////////////
class MvnDff :
  public MvnNodeBase
{
  friend class MvnMgr;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] clock_pol クロックの極性
  /// @param[in] control_array 非同期セットの極性を入れた配列
  /// @note 極性値は以下のとおり
  ///  - 1 : 正極性(posedge)
  ///  - 0 : 負極性(negedge)
  MvnDff(MvnModule* module,
	 ymuint clock_pol,
	 const vector<ymuint>& control_array);

  /// @brief デストラクタ
  ~MvnDff();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クロック信号の極性を得る．
  /// @retval 1 正極性(posedge)
  /// @retval 0 負極性(negedge)
  /// @note type() が kDff の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  clock_pol() const;

  /// @brief 非同期セット信号の極性を得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
  /// @retval 1 正極性(posedge)
  /// @retval 0 負極性(negedge)
  /// @note type() が kDff の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  control_pol(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // クロックと非同期セット入力の極性の配列
  ymuint32* mPolArray;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNDFF_H
