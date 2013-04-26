#ifndef YM_UTILS_PERMGEN_H
#define YM_UTILS_PERMGEN_H

/// @file ym_utils/PermGen.h
/// @brief PermGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/GenBase.h"


BEGIN_NAMESPACE_YM

class PermGenIterator;

//////////////////////////////////////////////////////////////////////
/// @class PermGen PermGen.h "ym_utils/PermGen.h"
/// @ingroup GeneratorGroup
/// @brief 順列生成器を表すクラス
//////////////////////////////////////////////////////////////////////
class PermGen :
  public GenBase
{
public:

  typedef PermGenIterator iterator;

public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選択する要素数
  PermGen(ymuint n,
	  ymuint k);

  /// @brief デストラクタ
  ~PermGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最初の順列を取り出す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class PermGenIterator PermGen.h "ym_utils/PermGen.h"
/// @ingroup GeneratorGroup
/// @brief PermGen の反復子
//////////////////////////////////////////////////////////////////////
class PermGenIterator :
  public GenIterator
{
  friend class PermGen;

public:

  /// @brief 空のコンストラクタ
  PermGenIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  PermGenIterator(const PermGenIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const PermGenIterator&
  operator=(const PermGenIterator& src);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  /// @return 次の要素を指す反復子
  PermGenIterator
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
  /// @param[in] parent 親の PermGen オブジェクト
  /// @note PermGen が用いる．
  PermGenIterator(const PermGen* parent);

};

END_NAMESPACE_YM

#endif // YM_UTILS_PERMGEN_H
