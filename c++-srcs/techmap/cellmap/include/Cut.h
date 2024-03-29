﻿#ifndef CUT_H
#define CUT_H

/// @file Cut.h
/// @brief Cut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "cellmap_nsdef.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class Cut Cut.h "Cut.h"
/// @brief パタングラフとマッチするサブジェクトグラフのサブグラフ
///
/// - 具体的には葉のノード配列と葉の極性を持つ．
/// - 根のノードはこのクラスには保持されない．
/// - 葉から根を通らずに出力へ至る経路は存在しないので cut と呼ぶ．
//////////////////////////////////////////////////////////////////////
class Cut
{
public:

  /// @brief コンストラクタ
  /// @param[in] nl 葉の数(入力数)
  Cut(SizeType nl = 0);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  Cut(const Cut& src);

  /// @brief デストラクタ
  ~Cut();

  /// @brief 代入演算子
  const Cut&
  operator=(const Cut& src);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 葉の数を再設定する．
  void
  resize(SizeType nl);

  /// @brief 葉のノードを設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
  /// @param[in] leaf_node 葉のノード
  /// @param[in] leaf_inv 葉の極性
  void
  set_leaf(SizeType pos,
	   const SbjNode* leaf_node,
	   bool leaf_inv);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 葉の数を得る．
  SizeType
  leaf_num() const;

  /// @brief 葉のノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
  const SbjNode*
  leaf_node(SizeType pos) const;

  /// @brief 葉の極性を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
  bool
  leaf_inv(SizeType pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 葉の数
  SizeType mLeafNum;

  // 葉のノードと極性をパックしたの配列
  PtrIntType* mLeafArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 葉の数を再設定する．
inline
void
Cut::resize(SizeType nl)
{
  if ( mLeafNum != nl ) {
    delete [] mLeafArray;
    mLeafNum = nl;
    mLeafArray = new PtrIntType[nl];
  }
}

// @brief 葉のノードを設定する．
// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
// @param[in] leaf_node 葉のノード
// @param[in] leaf_inv 葉の極性
inline
void
Cut::set_leaf(SizeType pos,
	      const SbjNode* leaf_node,
	      bool leaf_inv)
{
  ASSERT_COND( pos < leaf_num() );
  mLeafArray[pos] = reinterpret_cast<PtrIntType>(leaf_node) | leaf_inv;
}

// @brief 葉の数を得る．
inline
SizeType
Cut::leaf_num() const
{
  return mLeafNum;
}

// @brief 葉のノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
inline
const SbjNode*
Cut::leaf_node(SizeType pos) const
{
  ASSERT_COND( pos < leaf_num() );
  return reinterpret_cast<const SbjNode*>(mLeafArray[pos] & ~1UL);
}

// @brief 葉の極性を得る．
// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
inline
bool
Cut::leaf_inv(SizeType pos) const
{
  ASSERT_COND( pos < leaf_num() );
  return static_cast<bool>(mLeafArray[pos] & 1U);
}

END_NAMESPACE_CELLMAP

#endif // CUT_H
