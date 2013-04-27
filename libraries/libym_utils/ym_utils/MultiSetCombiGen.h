#ifndef YM_UTILS_MULTISETCOMBIGEN_H
#define YM_UTILS_MULTISETCOMBIGEN_H

/// @file MultiSetCombiGen.h
/// @brief MultiSetCombiGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiSetGenBase.h"


BEGIN_NAMESPACE_YM

class MscgIterator;

//////////////////////////////////////////////////////////////////////
/// @class MultiSetCombiGen MultiSetCombiGen.h "MultiSetCombiGen.h"
/// @brief 重複を許した集合の順列を作るクラス
//////////////////////////////////////////////////////////////////////
class MultiSetCombiGen :
  public MultiSetGenBase
{
public:

  typedef MscgIterator iterator;

public:

  /// @brief コンストラクタ
  /// @param[in] num_array 各要素の重複度を納めた配列
  /// @param[in] k 選び出す要素数
  MultiSetCombiGen(const vector<ymuint>& num_array,
		   ymuint k);

  /// @brief デストラクタ
  ~MultiSetCombiGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 先頭の反復子を返す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class MscgIterator MultiSetCombiGen.h "MultiSetCombiGen.h"
/// @brief MultiSetCombiGen の反復子
//////////////////////////////////////////////////////////////////////
class MscgIterator :
  public MsGenIterator
{
  friend class MultiSetCombiGen;

public:

  /// @brief 空のコンストラクタ
  MscgIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MscgIterator(const MscgIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const MscgIterator&
  operator=(const MscgIterator& src);

  /// @brief デストラクタ
  ~MscgIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  /// @return 次の要素を指す反復子
  MscgIterator
  operator++();

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] parent 親の MultiSetCombiGen オブジェクト
  MscgIterator(const MultiSetCombiGen* parent);

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num_array 各要素の重複度を納めた配列
// @param[in] k 選び出す要素数
inline
MultiSetCombiGen::MultiSetCombiGen(const vector<ymuint>& num_array,
				   ymuint k) :
  MultiSetGenBase(num_array, k)
{
}

// @brief デストラクタ
inline
MultiSetCombiGen::~MultiSetCombiGen()
{
}

// @brief 先頭の反復子を返す．
inline
MultiSetCombiGen::iterator
MultiSetCombiGen::begin()
{
  return iterator(this);
}

// @brief 空のコンストラクタ
inline
MscgIterator::MscgIterator()
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
MscgIterator::MscgIterator(const MscgIterator& src) :
  MsGenIterator(src)
{
}

// @brief コンストラクタ
// @param[in] parent 親の MultiSetCombiGen オブジェクト
inline
MscgIterator::MscgIterator(const MultiSetCombiGen* parent) :
  MsGenIterator(parent)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
inline
const MscgIterator&
MscgIterator::operator=(const MscgIterator& src)
{
  copy(src);
  return *this;
}

// @brief デストラクタ
inline
MscgIterator::~MscgIterator()
{
}

// @brief 末尾のチェック
// @return 末尾の時に true を返す．
inline
bool
MscgIterator::is_end() const
{
  if ( parent() == NULL ) {
    return true;
  }
  return elem(0) == group_num();
}

END_NAMESPACE_YM

#endif // YM_UTILS_MULTISETCOMBIGEN_H
