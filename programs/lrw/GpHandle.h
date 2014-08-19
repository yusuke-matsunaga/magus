/// @file Gphandle.h
/// @brief GpHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "GpNode.h"


BEGIN_NAMESPACE_YM

class GpNode;

//////////////////////////////////////////////////////////////////////
/// @class GpHandle GpNode.h "GpNode.h"
/// @brief GpNode + 否定属性の情報を持つハンドルクラス
//////////////////////////////////////////////////////////////////////
class GpHandle
{
  friend class GpMgr;

public:

  /// @brief 空のコンストラクタ
  GpHandle();

  /// @brief 普通のコンストラクタ
  GpHandle(GpNode* node,
	   bool inv = false);

  /// @brief デストラクタ
  ~GpHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取り出すメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを取り出す．
  GpNode*
  node() const;

  /// @brief 否定属性を取り出す．
  bool
  inv() const;

  /// @brief 定数0を表しているとき true を返す．
  bool
  is_const0() const;

  /// @brief 定数1を表しているとき true を返す．
  bool
  is_const1() const;

  /// @brief 関数ベクタを取り出す．
  ymuint16
  func() const;

  /// @brief 等価比較演算子
  bool
  operator==(const GpHandle right) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 極性を入れ替える．
  GpHandle
  operator~() const;

  /// @brief 反転属性を取り除く
  /// @return 自分自身を返す．
  GpHandle
  cannonicalize();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  GpHandle(ympuint data);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードのポインタ + 否定属性
  ympuint mNodePol;

};


//////////////////////////////////////////////////////////////////////
// GpHandle のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
GpHandle::GpHandle() :
  mNodePol(0UL)
{
}

// @brief 普通のコンストラクタ
inline
GpHandle::GpHandle(GpNode* node,
		   bool inv) :
  mNodePol(reinterpret_cast<ympuint>(node) | static_cast<ympuint>(inv))
{
}

// @brief 内容を直接指定したコンストラクタ
inline
GpHandle::GpHandle(ympuint data) :
  mNodePol(data)
{
}

// @brief デストラクタ
inline
GpHandle::~GpHandle()
{
}

// @brief ノードを取り出す．
inline
GpNode*
GpHandle::node() const
{
  return reinterpret_cast<GpNode*>(mNodePol & ~1UL);
}

// @brief 否定属性を取り出す．
inline
bool
GpHandle::inv() const
{
  return static_cast<bool>(mNodePol & 1UL);
}

// @brief 定数0を表しているとき true を返す．
inline
bool
GpHandle::is_const0() const
{
  return node() == NULL && !inv();
}

// @brief 定数1を表しているとき true を返す．
inline
bool
GpHandle::is_const1() const
{
  return node() == NULL && inv();
}

// @brief 関数ベクタを取り出す．
inline
ymuint16
GpHandle::func() const
{
  ymuint16 ans = 0U;
  if ( node() != NULL ) {
    ans = node()->func();
  }
  if ( inv() ) {
    ans = ~ans & 0xFFFF;
  }
  return ans;
}

// @brief 等価比較演算子
inline
bool
GpHandle::operator==(const GpHandle right) const
{
  return mNodePol == right.mNodePol;
}

// @brief 極性を入れ替える．
inline
GpHandle
GpHandle::operator~() const
{
  return GpHandle(mNodePol ^ 1UL);
}

// @brief 反転属性を取り除く
// @return 自分自身を返す．
inline
GpHandle
GpHandle::cannonicalize()
{
  mNodePol &= ~1UL;
  return *this;
}

END_NAMESPACE_YM
