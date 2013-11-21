#ifndef DOTLIB_DOTLIBLUT_H
#define DOTLIB_DOTLIBLUT_H

/// @file DotlibLut.h
/// @brief DotlibLut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibAttrMap.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibLut DotlibLut.h "DotlibLut.h"
/// @brief LUT 情報を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibLut :
  public DotlibAttrMap
{
public:

  /// @brief コンストラクタ
  DotlibLut();

  /// @brief デストラクタ
  ~DotlibLut();


public:

  /// @brief 内容をセットする．
  bool
  set_data(const DotlibNode* lut_node);

  /// @brief テンプレート名を返す．
  ShString
  template_name() const;

  /// @brief 'index_1' のノードを返す．
  /// @note 未定義なら NULL を返す．
  const DotlibNode*
  index_1() const;

  /// @brief 'index_2' のノードを返す．
  /// @note 未定義なら NULL を返す．
  const DotlibNode*
  index_2() const;

  /// @brief 'index_3' のノードを返す．
  /// @note 未定義なら NULL を返す．
  const DotlibNode*
  index_3() const;

  /// @brief 値のリストを返す．
  /// @note 1次元でもリストとなる．
  const DotlibNode*
  value_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート名
  ShString mName;

  // index_1
  const DotlibNode* mIndex1;

  // index_2
  const DotlibNode* mIndex2;

  // index_3
  const DotlibNode* mIndex3;

  // 値のリスト
  const DotlibNode* mValueList;

};

END_NAMESPACE_YM_DOTLIB

#endif // DOTLIB_DOTLIBLUT_H
