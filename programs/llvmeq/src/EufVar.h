#ifndef EUFVAR_H
#define EUFVAR_H

/// @file EufVar.h
/// @brief EufVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufNode.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufVar EufVar.h "EufVar.h"
/// @brief EUF 式の変数を表すクラス
//////////////////////////////////////////////////////////////////////
class EufVar :
  public EufNode
{
  friend class EufMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 変数名
  EufVar(const string& name);

  /// @brief デストラクタ
  virtual
  ~EufVar();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  string mName;

  // ハッシュで用いるリンク
  EufVar* mLink;

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFVAR_H
