#ifndef YM_UTILS_MULTICOMBIGEN_H
#define YM_UTILS_MULTICOMBIGEN_H

/// @file ym_utils/MultiCombiGen.h
/// @brief MultiCombiGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiGenBase.h"


BEGIN_NAMESPACE_YM

class MultiCombiGenIterator;

//////////////////////////////////////////////////////////////////////
/// @class MultiCombiGen MultiCombiGen.h "ym_utils/MultiCombiGen.h"
/// @ingroup GeneratorGroup
/// @brief 複数の要素のグループの組み合わせを生成するクラス
//////////////////////////////////////////////////////////////////////
class MultiCombiGen :
  public MultiGenBase
{
public:

  typedef MultiCombiGenIterator iterator;

public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiCombiGen(const vector<pair<ymuint, ymuint> >& nk_array);

  /// @brief デストラクタ
  ~MultiCombiGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最初の組み合わせを取り出す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class MultiCombiGenIterator MultiCombiGen.h "ym_utils/MultiCombiGen.h"
/// @ingroup GeneratorGroup
/// @brief MultiCombiGen の反復子
//////////////////////////////////////////////////////////////////////
class MultiCombiGenIterator :
  public MultiGenIterator
{
  friend class MultiCombiGen;

public:

  /// @brief 空のコンストラクタ
  MultiCombiGenIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MultiCombiGenIterator(const MultiCombiGenIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const MultiCombiGenIterator&
  operator=(const MultiCombiGenIterator& src);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  /// @return 次の要素を指す反復子
  MultiCombiGenIterator
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
  /// @param[in] parent 親のオブジェクト
  /// @brief MultiCombiGen が用いる．
  MultiCombiGenIterator(const MultiCombiGen* parent);

  /// @brief operator++() のサブルーティン
  void
  next(ymuint g,
       ymuint pos);

  /// @brief grp 番目のグループが終了状態の時 true を返す．
  bool
  is_end_sub(ymuint grp) const;

};

END_NAMESPACE_YM

#endif // YM_UTILS_MULTICOMBIGEN_H
