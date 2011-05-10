#ifndef YM_DOTLIB_DOTLIBATTRMAP_H
#define YM_DOTLIB_DOTLIBATTRMAP_H

/// @file ym_dotlib/DotlibAttrMap.h
/// @brief DotlibAttrMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/dotlib_nsdef.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibAttrMap DotlibAttrMap.h "ym_dotlib/DotlibAttrMap.h"
/// @brief 属性名をキーにした属性値のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibAttrMap
{
  friend class DotlibNode;

public:

  /// @brief コンストラクタ
  DotlibAttrMap();

  /// @brief デストラクタ
  ~DotlibAttrMap();

  /// @brief 内容を初期化する．
  void
  clear();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を得る．
  /// @param[in] name 属性名
  /// @param[out] node_list 値のリストを格納する変数
  void
  get(const ShString& name,
      vector<const DotlibNode*>& node_list) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // DotlibNode から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を追加する．
  /// @param[in] name 属性名
  /// @param[in] node 値を表すノード
  void
  add(const ShString& name,
      const DotlibNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表
  hash_map<ShString, list<const DotlibNode*> > mHash;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBATTRMAP_H
