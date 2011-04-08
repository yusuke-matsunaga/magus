#ifndef YM_MVN_MVNOUTPUTPIN_H
#define YM_MVN_MVNOUTPUTPIN_H

/// @file ym_mvn/MvnOutputPin.h
/// @brief MvnOutputPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnOutputPin MvPin.h "ym_mvn/MvPin.h"
/// @brief ノードの出力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnOutputPin
{
  friend class MvnMgr;
  friend class MvnNode;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvnOutputPin();

  /// @brief デストラクタ
  ~MvnOutputPin();


public:

  /// @breif 属しているノードを得る．
  MvnNode*
  node() const;

  /// @brief ビット幅を得る．
  ymuint
  bit_width() const;

  /// @brief ノードの何番目のピンかを返す．
  /// @note 入力ピンと出力ピンは区別される．
  ymuint
  pos() const;

  /// @brief 接続している入力ピンのリストを得る．
  const MvnInputPinList&
  dst_pin_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のノード
  MvnNode* mNode;

  // ビット幅
  ymuint32 mBitWidth;

  // 位置
  ymuint32 mPos;

  // 接続している入力ピンのリスト
  MvnInputPinList mDstPinList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @breif 属しているノードを得る．
inline
MvnNode*
MvnOutputPin::node() const
{
  return mNode;
}

// @brief ビット幅を得る．
inline
ymuint
MvnOutputPin::bit_width() const
{
  return mBitWidth;
}

// @brief ノードの何番目のピンかを返す．
// @note 入力ピンと出力ピンは区別される．
inline
ymuint
MvnOutputPin::pos() const
{
  return mPos;
}

// @brief 接続している入力ピンのリストを得る．
inline
const MvnInputPinList&
MvnOutputPin::dst_pin_list() const
{
  return mDstPinList;
}

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVNPIN_H
