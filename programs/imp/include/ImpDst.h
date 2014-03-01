#ifndef IMPDST_H
#define IMPDST_H

/// @file ImpDst.h
/// @brief ImpDst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "networks/BNetwork.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpNode;


//////////////////////////////////////////////////////////////////////
/// @class ImpDst ImpDst.h "ImpDst.h"
/// @brief 含意結果を表すクラス
//////////////////////////////////////////////////////////////////////
class ImpDst
{
public:

  /// @brief 空のコンストラクタ
  ImpDst();

  /// @brief 値を指定したコンストラクタ
  /// @param[in] node ノード
  /// @param[in] val 値
  ImpDst(ImpNode* node,
	 ymuint val);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを得る．
  ImpNode*
  node() const;

  /// @brief 値を得る．
  ymuint
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードと値の情報をパックしたもの
  ympuint mNodeVal;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
ImpDst::ImpDst() :
  mNodeVal(0ULL)
{
}

// @brief 値を指定したコンストラクタ
// @param[in] node ノード
// @param[in] val 値
inline
ImpDst::ImpDst(ImpNode* node,
	       ymuint val) :
  mNodeVal(reinterpret_cast<ympuint>(node) | val)
{
}

// @brief ノードを得る．
inline
ImpNode*
ImpDst::node() const
{
  return reinterpret_cast<ImpNode*>(mNodeVal & ~1ULL);
}

// @brief 値を得る．
inline
ymuint
ImpDst::val() const
{
  return static_cast<ymuint>(mNodeVal & 1ULL);
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPDST_H
