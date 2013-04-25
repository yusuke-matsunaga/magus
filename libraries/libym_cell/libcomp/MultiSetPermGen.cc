
/// @file MultiSetPermGen.cc
/// @brief MultiSetPermGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "MultiSetPermGen.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
// クラス MspgIterator
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
MspgIterator::MspgIterator()
{
  mParent = NULL;
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
MspgIterator::MspgIterator(const MspgIterator& src) :
  mElem(src.mElem),
  mParent(src.mParent)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
const MspgIterator&
MspgIterator::operator=(const MspgIterator& src)
{
  if ( this != &src ) {
    mElem = src.mElem;
    mParent = src.mParent;
  }
  return *this;
}

// @brief デストラクタ
MspgIterator::~MspgIterator()
{
}

// @brief 要素の取得
// @param[in] pos 取り出す要素の位置
ymuint
MspgIterator::operator()(ymuint pos) const
{
  return mElem[pos];
}

// @brief 次の要素を求める．
// @return 次の要素を指す反復子
MspgIterator
MspgIterator::operator++()
{
}

// @brief 末尾のチェック
// @return 末尾の時に true を返す．
bool
MspgIterator::is_end() const
{
}

// @brief コンストラクタ
// @param[in] parent 親の MultiSetPermGen オブジェクト
MspgIterator::MspgIterator(const MultiSetPermGen* parent)
{
}


//////////////////////////////////////////////////////////////////////
// クラス MultiSetPermGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num_array 各要素の重複度を納めた配列
// @param[in] k 選び出す要素数
MultiSetPermGen::MultiSetPermGen(const vector<ymuint>& num_array,
				 ymuint k)
{
}

// @brief デストラクタ
MultiSetPermGen::~MultiSetPermGen()
{
}

// @brief 要素の種類の数を得る．
ymuint
MultiSetPermGen::group_num() const
{
}

// @brief 各要素の重複度を得る．
// @param[in] grp ( 0 <= grp < group_num() )
ymuint
MultiSetPermGen::n(ymuint grp) const
{
}

// @brief 選択する要素数を返す．
ymuint
MultiSetPermGen::k() const
{
}

// @brief 先頭の反復子を返す．
MultiSetPermGen::iterator
MultiSetPermGen::begin()
{
}

END_NAMESPACE_YM_CELL_LIBCOMP
