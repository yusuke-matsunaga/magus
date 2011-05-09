#ifndef YM_DOTLIB_DOTLIBATTRMAP_H
#define YM_DOTLIB_DOTLIBATTRMAP_H

/// @file ym_dotlib/DotlibAttrMap.h
/// @brief DotlibAttrMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/dotlib_nsdef.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibAttrMap DotlibAttrMap.h "ym_dotlib/DotlibAttrMap.h"
/// @brief 属性名をキーにした属性値のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibAttrMap
{
public:

  typedef hash_map<ShString, list<DotlibNode*> >::const_iterator const_iterator;
  typedef hash_map<ShString, list<DotlibNode*> >::iterator iterator;

public:

  /// @brief コンストラクタ
  DotlibAttrMap();

  /// @brief デストラクタ
  ~DotlibAttrMap();


public:

  /// @brief 値を追加する．
  /// @param[in] name 属性名
  /// @param[in] node 値を表すノード
  void
  add(const ShString& name,
      const DotlibNode* node);

  /// @brief 値のリストを取り出す．
  /// @param[in] name 属性名
  /// @param[in]
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表
  hash_map<ShString, list<DotlibNode*> > mHash;

}

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBATTRMAP_H
