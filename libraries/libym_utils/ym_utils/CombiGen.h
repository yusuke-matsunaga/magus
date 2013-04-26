#ifndef YM_UTILS_COMBIGEN_H
#define YM_UTILS_COMBIGEN_H

/// @file ym_utils/CombiGen.h
/// @brief CombiGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/GenBase.h"


BEGIN_NAMESPACE_YM

class CombiGenIterator;

//////////////////////////////////////////////////////////////////////
/// @class CombiGen CombiGen.h "ym_utils/CombiGen.h"
/// @ingroup GeneratorGroup
/// @brief 組み合わせ生成器を表すクラス
//////////////////////////////////////////////////////////////////////
class CombiGen :
  public GenBase
{
public:

  typedef CombiGenIterator iterator;

public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選び出す要素数
  CombiGen(ymuint n,
	   ymuint k);

  /// @brief デストラクタ
  ~CombiGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最初の組み合わせを取り出す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class CombiGenIterator CombiGen.h "ym_utils/CombiGen.h"
/// @ingroup GeneratorGroup
/// @brief CombiGen の反復子
//////////////////////////////////////////////////////////////////////
class CombiGenIterator :
  public GenIterator
{
  friend class CombiGen;

public:

  /// @brief 空のコンストラクタ
  CombiGenIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  CombiGenIterator(const CombiGenIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const CombiGenIterator&
  operator=(const CombiGenIterator& src);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  /// @return 次の要素を指す反復子
  CombiGenIterator
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
  /// @param[in] parent 親の CombiGen オブジェクト
  /// @note CombiGen が用いる．
  CombiGenIterator(const CombiGen* parent);

  /// @brief operator++() のサブルーティン
  void
  next(ymuint pos);

};

END_NAMESPACE_YM

#endif // YM_UTILS_COMBIGEN_H
