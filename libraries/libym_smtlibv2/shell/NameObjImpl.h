#ifndef NAMEOBJIMPL_H
#define NAMEOBJIMPL_H

/// @file NameObjImpl.h
/// @brief NameObjImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "NameObj.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class NameObjImpl NameObjImpl.h "NameObjImpl.h"
/// @brief SmtNameObj の実装クラス
//////////////////////////////////////////////////////////////////////
class NameObjImpl :
  public NameObj
{
  friend class NameMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] name_id 名前を表す識別子
  NameObjImpl(const SmtId* name_id);

  /// @brief デストラクタ
  virtual
  ~NameObjImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を表す識別子を返す．
  virtual
  const SmtId*
  name() const;

  /// @brief 組み込み関数の時 true を返す．
  virtual
  bool
  is_builtin_fun() const;

  /// @brief SmtFun を持っているとき true を返す．
  virtual
  bool
  is_fun() const;

  /// @brief SmtVar を持っているとき true を返す．
  virtual
  bool
  is_var() const;

  /// @brief SmtTerm を持っているとき true を返す．
  virtual
  bool
  is_term() const;

  /// @brief tSumFun を返す．
  /// @note is_builtin_fun() == true の時のみ意味がある．
  virtual
  tSmtFunType
  fun_type() const;

  /// @brief SmtFun を返す．
  /// @note is_fun() == true の時のみ意味がある．
  virtual
  const SmtFun*
  fun() const;

  /// @brief SmtVar を返す．
  /// @note is_var() == true の時のみ意味がある．
  virtual
  const SmtVar*
  var() const;

  /// @brief SmtTerm を返す．
  /// @note is_term() == true の時のみ意味がある．
  virtual
  const SmtTerm*
  term() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const SmtId* mName;

  // ハッシュ表で用いるリンクポインタ
  NameObjImpl* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class BuiltinFunObj NameObjImpl.h "NameObjImpl.h"
/// @brief tSmtFunType を表す NameObj の派生クラス
//////////////////////////////////////////////////////////////////////
class BuiltinFunObj :
  public NameObjImpl
{
  friend class NameMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] fun_type 関数の型
  BuiltinFunObj(const SmtId* name_id,
		tSmtFunType fun_type);

  /// @brief デストラクタ
  virtual
  ~BuiltinFunObj();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の時 true を返す．
  virtual
  bool
  is_builtin_fun() const;

  /// @brief tSumFun を返す．
  /// @note is_builtin_fun() == true の時のみ意味がある．
  virtual
  tSmtFunType
  fun_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数の型
  tSmtFunType mFunType;

};


//////////////////////////////////////////////////////////////////////
/// @class FunObj NameObjImpl.h "NameObjImpl.h"
/// @brief SmtFun を表す NameObj の派生クラス
//////////////////////////////////////////////////////////////////////
class FunObj :
  public NameObjImpl
{
  friend class NameMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] fun 関数
  FunObj(const SmtId* name_id,
	 const SmtFun* fun);

  /// @brief デストラクタ
  virtual
  ~FunObj();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SmtFun を持っているとき true を返す．
  virtual
  bool
  is_fun() const;

  /// @brief SmtFun を返す．
  /// @note is_fun() == true の時のみ意味がある．
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
  friend class NameMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] var 変数
  VarObj(const SmtId* name_id,
	 const SmtVar* var);

  /// @brief デストラクタ
  virtual
  ~VarObj();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SmtVar を持っているとき true を返す．
  virtual
  bool
  is_var() const;

  /// @brief SmtVar を返す．
  /// @note is_var() == true の時のみ意味がある．
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


//////////////////////////////////////////////////////////////////////
/// @class TermObj NameObjImpl.h "NameObjImpl.h"
/// @brief SmtTerm を表す NameObj の派生クラス
//////////////////////////////////////////////////////////////////////
class TermObj :
  public NameObjImpl
{
  friend class NameMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] term 式
  TermObj(const SmtId* name_id,
	  const SmtTerm* term);

  /// @brief デストラクタ
  virtual
  ~TermObj();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SmtTerm を持っているとき true を返す．
  virtual
  bool
  is_term() const;

  /// @brief SmtTerm を返す．
  /// @note is_term() == true の時のみ意味がある．
  virtual
  const SmtTerm*
  term() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式
  const SmtTerm* mTerm;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // NAMEOBJIMPL_H
