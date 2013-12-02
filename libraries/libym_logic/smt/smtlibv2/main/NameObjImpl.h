#ifndef NAMEOBJIMPL_H
#define NAMEOBJIMPL_H

/// @file NameObjImpl.h
/// @brief NameObjImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtNameObj.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class NameObjImpl NameObjImpl.h "NameObjImpl.h"
/// @brief SmtNameObj の実装クラス
//////////////////////////////////////////////////////////////////////
class NameObjImpl :
  public SmtNameObj
{
  friend class SmtNameMgr;

protected:

  /// @brief コンストラクタ
  NameObjImpl();

  /// @brief デストラクタ
  virtual
  ~NameObjImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SmtFun を持っているとき true を返す．
  virtual
  bool
  is_fun() const;

  /// @brief SmtVar を持っているとき true を返す．
  virtual
  bool
  is_var() const;

  /// @brief SmtFun を返す．
  virtual
  const SmtFun*
  fun() const;

  /// @brief SmtVar を返す．
  virtual
  const SmtVar*
  var() const;



private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表で用いるリンクポインタ
  SmtNameObj* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class FunObj NameObjImpl.h "NameObjImpl.h"
/// @brief SmtFun を表す NameObj の派生クラス
//////////////////////////////////////////////////////////////////////
class FunObj :
  public NameObjImpl
{
  friend class SmtNameMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] fun 関数
  FunObj(const SmtFun* fun);

  /// @brief デストラクタ
  virtual
  ~FunObj();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を表す識別子を返す．
  virtual
  const SmtId*
  name() const;

  /// @brief SmtFun を持っているとき true を返す．
  virtual
  bool
  is_fun() const;

  /// @brief SmtFun を返す．
  virtual
  const SmtFun*
  fun() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  const SmtFun* mFun;

};


//////////////////////////////////////////////////////////////////////
/// @class VarObj NameObjImpl.h "NameObjImpl.h"
/// @brief SmtVar を表す NameObj の派生クラス
//////////////////////////////////////////////////////////////////////
class VarObj :
  public NameObjImpl
{
  friend class SmtNameMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] var 変数
  VarObj(const SmtVar* var);

  /// @brief デストラクタ
  virtual
  ~VarObj();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を表す識別子を返す．
  virtual
  const SmtId*
  name() const;

  /// @brief SmtVar を持っているとき true を返す．
  virtual
  bool
  is_var() const;

  /// @brief SmtVar を返す．
  virtual
  const SmtVar*
  var() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数
  const SmtVar* mVar;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // NAMEOBJIMPL_H
