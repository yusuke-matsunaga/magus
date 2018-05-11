#ifndef SBJHANDLE_H
#define SBJHANDLE_H

/// @file SbjHandle.h
/// @brief SbjHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SbjHandle SbjGraph.h "SbjGraph.h"
/// @brief ノード＋極性の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class SbjHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 極性
  explicit
  SbjHandle(SbjNode* node = nullptr,
	    bool inv = false);

  /// @brief デストラクタ
  ~SbjHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 定数を作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0を返す．
  static
  SbjHandle
  make_zero();

  /// @brief 定数1を返す．
  static
  SbjHandle
  make_one();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを返す．
  SbjNode*
  node() const;

  /// @brief 極性を返す．
  bool
  inv() const;

  /// @brief 定数0を表しているかどうか調べる．
  bool
  is_const0() const;

  /// @brief 定数1を表しているかどうか調べる．
  bool
  is_const1() const;

  /// @brief 極性を反転させた結果を返す．
  SbjHandle
  operator~() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(SbjNode* node,
      bool inv);

  /// @brief 極性を反転させる．
  void
  invert();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを直接指定したコンストラクタ
  SbjHandle(ympuint data);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードのポインタと極性をパックしたもの
  ympuint mData;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node ノード
// @param[in] inv 極性
inline
SbjHandle::SbjHandle(SbjNode* node,
		     bool inv)
{
  set(node, inv);
}

// @brief データを直接指定したコンストラクタ
inline
SbjHandle::SbjHandle(ympuint data) :
  mData(data)
{
}

// @brief デストラクタ
inline
SbjHandle::~SbjHandle()
{
}

// @brief 定数0を返す．
inline
SbjHandle
SbjHandle::make_zero()
{
  return SbjHandle(static_cast<ympuint>(0ULL));
}

// @brief 定数1を返す．
inline
SbjHandle
SbjHandle::make_one()
{
  return SbjHandle(static_cast<ympuint>(1ULL));
}

// @brief ノードを返す．
inline
SbjNode*
SbjHandle::node() const
{
  return reinterpret_cast<SbjNode*>(mData & ~1ULL);
}

// @brief 極性を返す．
inline
bool
SbjHandle::inv() const
{
  return static_cast<bool>(mData & 1ULL);
}

// @brief 定数0を表しているかどうか調べる．
inline
bool
SbjHandle::is_const0() const
{
  return mData == 0ULL;
}

// @brief 定数1を表しているかどうか調べる．
inline
bool
SbjHandle::is_const1() const
{
  return mData == 1ULL;
}

// @brief 極性を反転させた結果を返す．
inline
SbjHandle
SbjHandle::operator~() const
{
  return SbjHandle(mData ^ 1ULL);
}

// @brief 内容を設定する．
inline
void
SbjHandle::set(SbjNode* node,
	       bool inv)
{
  mData = reinterpret_cast<ympuint>(node) | static_cast<ympuint>(inv);
}

// @brief 極性を反転させる．
inline
void
SbjHandle::invert()
{
  mData ^= 1ULL;
}

END_NAMESPACE_YM_SBJ

#endif // SBJGRAPH_H
