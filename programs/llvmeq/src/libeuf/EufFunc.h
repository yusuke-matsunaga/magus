#ifndef EUFFUNC_H
#define EUFFUNC_H

/// @file EufFunc.h
/// @brief EufFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufNode.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufFunc EufFunc.h "EufFunc.h"
/// @brief EUF 式の関数を表すクラス
//////////////////////////////////////////////////////////////////////
class EufFunc :
  public EufNode
{
  friend class EufFuncMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  EufFunc(ymuint id,
	  const string& name,
	  const vector<EufNode*>& arg_list);

  /// @brief デストラクタ
  virtual
  ~EufFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief 識別子名を得る．
  /// @note type() が kFunc, kVar の時のみ有効
  virtual
  string
  id_name() const;

  /// @brief 引数の数を得る．
  /// @note type() が kFunc の時のみ有効
  virtual
  ymuint
  arg_num() const;

  /// @brief 引数を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < arg_num() )
  /// @note type() が kFunc の時のみ有効
  virtual
  EufNode*
  arg(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  string mName;

  // 引数のリスト
  vector<EufNode*> mArgList;

  // ハッシュで用いるリンク
  EufFunc* mLink;

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFFUNC_H
