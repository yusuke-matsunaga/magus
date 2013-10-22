#ifndef SMTNAMEOBJ_H
#define SMTNAMEOBJ_H

/// @file SmtNameObj.h
/// @brief SmtNameObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtNameObj SmtNameObj.h "SmtNameObj.h"
/// @brief SmtFun/SmtVar を持つ擬似ポインタの基底クラス
//////////////////////////////////////////////////////////////////////
class SmtNameObj
{
  friend class SmtNameMgr;

protected:

  /// @brief デストラクタ
  virtual
  ~SmtNameObj() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を表す識別子を返す．
  virtual
  const SmtId*
  name() const = 0;

  /// @brief SmtFun を持っているとき true を返す．
  virtual
  bool
  is_fun() const = 0;

  /// @brief SmtVar を持っているとき true を返す．
  virtual
  bool
  is_var() const = 0;

  /// @brief SmtFun を返す．
  virtual
  const SmtFun*
  fun() const = 0;

  /// @brief SmtVar を返す．
  virtual
  const SmtVar*
  var() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表で用いるリンクポインタ
  SmtNameObj* mLink;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTNAMEOBJ_H
