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
  MvDff(MvModule* module,
	ymuint bit_width,
	ymuint np);

  /// @brief デストラクタ
  ~MvDff();


public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

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
