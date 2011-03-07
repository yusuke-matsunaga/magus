#ifndef LIBYM_MVN_MVDFF_H
#define LIBYM_MVN_MVDFF_H

/// @file libym_mvn/MvDff.h
/// @brief MvDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvDff MvDff.h "MvDff.h"
/// @brief D-FF を表すクラス
//////////////////////////////////////////////////////////////////////
class MvDff :
  public MvNode
{
  friend class MvMgr;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] clock_pol クロックの極性
  /// @param[in] control_array 非同期セットの極性を入れた配列
  /// @param[in] bit_width ビット幅
  /// @note 極性値は以下のとおり
  ///  - 1 : 正極性(posedge)
  ///  - 0 : 負極性(negedge)
  MvDff(MvModule* module,
	ymuint clock_pol,
	const vector<ymuint>& control_array,
	ymuint bit_width);

  /// @brief デストラクタ
  ~MvDff();


public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

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


//////////////////////////////////////////////////////////////////////
/// @class MvLatch MvDff.h "MvDff.h"
/// @brief ラッチを表すクラス
//////////////////////////////////////////////////////////////////////
class MvLatch :
  public MvNode
{
  friend class MvMgr;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width ビット幅
  MvLatch(MvModule* module,
	  ymuint bit_width);

  /// @brief デストラクタ
  ~MvLatch();

public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVDFF_H
