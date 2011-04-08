#ifndef YM_MVN_MVNINPUTPIN_H
#define YM_MVN_MVNINPUTPIN_H

/// @file ym_mvn/MvnInputPin.h
/// @brief MvnInputPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnInputPin MvPin.h "ym_mvn/MvPin.h"
/// @brief ノードの入力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnInputPin :
  public DlElem
{
  friend class MvnMgr;
  friend class MvnNode;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvnInputPin();

  /// @brief デストラクタ
  ~MvnInputPin();


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

  /// @brief 接続している出力ピンを得る．
  MvnOutputPin*
  src_pin() const;


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

  // 接続している出力ピン
  MvnOutputPin* mSrcPin;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @breif 属しているノードを得る．
inline
MvnNode*
MvnInputPin::node() const
{
  return mNode;
}

// @brief ビット幅を得る．
inline
ymuint
MvnInputPin::bit_width() const
{
  return mBitWidth;
}

// @brief ノードの何番目のピンかを返す．
// @note 入力ピンと出力ピンは区別される．
inline
ymuint
MvnInputPin::pos() const
{
  return mPos;
}

// @brief 接続している出力ピンを得る．
inline
MvnOutputPin*
MvnInputPin::src_pin() const
{
  return mSrcPin;
}

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVNINPUTPIN_H
