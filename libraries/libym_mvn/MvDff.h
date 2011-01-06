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
  /// @param[in] bit_width ビット幅
  /// @param[in] np 非同期セット入力ピン数
  /// @param[in] control_array 非同期セットの極性と値を入れた配列
  MvDff(MvModule* module,
	ymuint bit_width,
	ymuint np,
	const vector<ymuint32>& control_array);

  /// @brief デストラクタ
  ~MvDff();


public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

  /// @brief 非同期セット信号の極性を得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
  /// @retval 1 正極性(posedge)
  /// @retval 0 負極性(negedge)
  /// @note type() が kDff の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  control_pol(ymuint pos) const;

  /// @brief 非同期セット信号のセット値を得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kDff の時のみ意味を持つ．
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  control_value(ymuint pos,
		vector<ymuint32>& val) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 非同期セット入力の極性の配列
  ymuint32* mPolArray;

  // 非同期セット入力のセット値の配列の配列
  // 実際には1次元配列に詰めて格納する．
  ymuint32* mValArray;

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
