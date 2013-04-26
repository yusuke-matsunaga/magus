#ifndef YM_UTILS_MULTIPERMGEN_H
#define YM_UTILS_MULTIPERMGEN_H

/// @file ym_utils/MultiPermGen.h
/// @brief MultiPermGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiGenBase.h"


BEGIN_NAMESPACE_YM

class MultiPermGenIterator;

//////////////////////////////////////////////////////////////////////
/// @class MultiPermGen MultiPermGen.h "ym_utils/MultiPermGen.h"
/// @ingroup GeneratorGroup
/// @brief 複数の要素のグループの順列を生成するクラス
//////////////////////////////////////////////////////////////////////
class MultiPermGen :
  public MultiGenBase
{
public:

  typedef MultiPermGenIterator iterator;

public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiPermGen(const vector<pair<ymuint, ymuint> >& nk_array);

  /// @brief デストラクタ
  ~MultiPermGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最初の順列を取り出す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class MultiPermGenIterator MultiPermGen.h "ym_utils/MultiPermGen.h"
/// @ingroup GeneratorGroup
/// @brief MultiPermGen の反復子
//////////////////////////////////////////////////////////////////////
class MultiPermGenIterator :
  public MultiGenIterator
{
  friend class MultiPermGen;

public:

  /// @brief 空のコンストラクタ
  MultiPermGenIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MultiPermGenIterator(const MultiPermGenIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const MultiPermGenIterator&
  operator=(const MultiPermGenIterator& src);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  /// @return 次の要素を指す反復子
  MultiPermGenIterator
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
  /// @brief MultiPermGen が用いる．
  MultiPermGenIterator(const MultiPermGen* parent);

  /// @brief operator++() のサブルーティン
  void
  next(ymuint pos);

  /// @brief grp 番目のグループが終了状態の時 true を返す．
  bool
  is_end_sub(ymuint grp) const;

};

END_NAMESPACE_YM

#endif // YM_UTILS_MULTIPERMGEN_H
