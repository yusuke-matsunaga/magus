#ifndef YM_BDN_BDNPORT_H
#define YM_BDN_BDNPORT_H

/// @file ym_networks/BdnPort.h
/// @brief BdnPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn_nsdef.h"


BEGIN_NAMESPACE_YM_BDN

class BdnAuxData;

//////////////////////////////////////////////////////////////////////
/// @class BdnPort BdnPort.h "BdnPort.h"
/// @brief 外部入出力ポートを表すクラス
//////////////////////////////////////////////////////////////////////
class BdnPort
{
  friend class BdnMgrImpl;

private:

  /// @brief コンストラクタ
  BdnPort();

  /// @brief デストラクタ
  ~BdnPort();


public:

  /// @brief ID 番号を得る．
  ymuint
  id() const;

  /// @brief 名前を得る．
  string
  name() const;

  /// @brief ビット幅を得る．
  ymuint
  bit_width() const;

  /// @brief 入力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
  /// @note 出力ポートの場合には NULL を返す．
  BdnNode*
  input(ymuint pos) const;

  /// @brief 出力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
  /// @note 入力ポートの場合には NULL を返す．
  BdnNode*
  output(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // 名前
  string mName;

  // ビット幅
  ymuint32 mBitWidth;

  // 入力ノードの配列
  BdnNode** mInputArray;

  // 出力ノードの配列
  BdnNode** mOutputArray;

  // BdnNode に付加する情報
  BdnAuxData** mAuxDataArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID 番号を得る．
inline
ymuint
BdnPort::id() const
{
  return mId;
}

// @brief 名前を得る．
inline
string
BdnPort::name() const
{
  return mName;
}

// @brief ビット幅を得る．
inline
ymuint
BdnPort::bit_width() const
{
  return mBitWidth;
}

// @brief 入力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
// @note 出力ポートの場合には NULL を返す．
inline
BdnNode*
BdnPort::input(ymuint pos) const
{
  return mInputArray[pos];
}

// @brief 出力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
// @note 入力ポートの場合には NULL を返す．
inline
BdnNode*
BdnPort::output(ymuint pos) const
{
  return mOutputArray[pos];
}

END_NAMESPACE_YM_BDN

#endif // YM_BDN_BDNPORT_H
