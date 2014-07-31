#ifndef MVNDFF_H
#define MVNDFF_H

/// @file MvnDff.h
/// @brief MvnDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MvnNodeBase.h"
#include "YmNetworks/MvnMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

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
  /// @param[in] pol_array 非同期セット信号の極性情報を入れた配列
  /// @param[in] val_array 非同期セットの値を入れた配列
  /// @note 極性値は以下のとおり
  ///  - 1 : 正極性(posedge)
  ///  - 0 : 負極性(negedge)
  MvnDff(MvnModule* module,
	 ymuint clock_pol,
	 const vector<ymuint>& pol_array,
	 const vector<MvnNode*>& val_array);

  /// @brief デストラクタ
  virtual
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

  /// @brief 非同期セットの値を表す定数ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
  /// @note デフォルトの実装では NULL を返す．
  virtual
  const MvnNode*
  control_val(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // クロックと非同期セット入力の極性の配列
  ymuint32* mPolArray;

  // 非同期セットの値を表すノードの配列
  MvnNode** mValArray;

};

END_NAMESPACE_YM_NETWORKS_MVN

#endif // MVNDFF_H
