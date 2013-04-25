#ifndef MULTISETPERMGEN_H
#define MULTISETPERMGEN_H

/// @file MultiSetPermGen.h
/// @brief MultiSetPermGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

class MultiSetPermGen;

//////////////////////////////////////////////////////////////////////
/// @class MspgIterator MultiSetPermGen.h "MultiSetPermGen.h"
/// @brief MultiSetPermGen の反復子
//////////////////////////////////////////////////////////////////////
class MspgIterator
{
  friend class MultiSetPermGen;

public:

  /// @brief 空のコンストラクタ
  MspgIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MspgIterator(const MspgIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const MspgIterator&
  operator=(const MspgIterator& src);

  /// @brief デストラクタ
  ~MspgIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の取得
  /// @param[in] pos 取り出す要素の位置
  ymuint
  operator()(ymuint pos) const;

  /// @brief 次の要素を求める．
  /// @return 次の要素を指す反復子
  MspgIterator
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
  /// @param[in] parent 親の MultiSetPermGen オブジェクト
  MspgIterator(const MultiSetPermGen* parent);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在の要素
  vector<ymuint32> mElem;

  // 親の MultiSetPermGen
  const MultiSetPermGen* mParent;

};


//////////////////////////////////////////////////////////////////////
/// @class MultiSetPermGen MultiSetPermGen.h "MultiSetPermGen.h"
/// @brief 重複を許した集合の順列を作るクラス
//////////////////////////////////////////////////////////////////////
class MultiSetPermGen
{
public:

  typedef MspgIterator iterator;

public:

  /// @brief コンストラクタ
  /// @param[in] num_array 各要素の重複度を納めた配列
  /// @param[in] k 選び出す要素数
  MultiSetPermGen(const vector<ymuint>& num_array,
		  ymuint k);

  /// @brief デストラクタ
  ~MultiSetPermGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の種類の数を得る．
  ymuint
  group_num() const;

  /// @brief 各要素の重複度を得る．
  /// @param[in] grp ( 0 <= grp < group_num() )
  ymuint
  n(ymuint grp) const;

  /// @brief 選択する要素数を返す．
  ymuint
  k() const;

  /// @brief 先頭の反復子を返す．
  iterator
  begin();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の重複度
  vector<ymuint32> mMultiArray;

  // 選択する要素数
  ymuint32 mK;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // MULTISETPERMGEN_H
