#ifndef SMTFUNIMPL_H
#define SMTFUNIMPL_H

/// @file SmtFunImpl.h
/// @brief SmtFunImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtFun.h"
#include "ym_smtlibv2/SmtSortedVar.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtFunImpl SmtFunImpl.h "SmtFunImpl.h"
/// @brief SmtFun の実装クラス
//////////////////////////////////////////////////////////////////////
class SmtFunImpl :
  public SmtFun
{
  friend class SmtFunMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] sort 出力の型
  /// @param[in] input_num 入力数
  SmtFunImpl(const SmtId* name,
	     const SmtSort* sort,
	     ymuint input_num);

  /// @brief デストラクタ
  virtual
  ~SmtFunImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  virtual
  ymuint
  id() const;

  /// @brief 名前を返す．
  virtual
  const SmtId*
  name() const;

  /// @brief 出力の型を返す．
  virtual
  const SmtSort*
  sort() const;

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_num() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // 名前
  const SmtId* mName;

  // 出力の型
  const SmtSort* mSort;

  // 入力数
  ymuint32 mInputNum;

  // ハッシュ用のリンクポインタ
  SmtFunImpl* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtFun1 SmtFunImpl.h "SmtFunImpl.h"
/// @brief 宣言のみの関数
//////////////////////////////////////////////////////////////////////
class SmtFun1 :
  public SmtFunImpl
{
  friend class SmtFunMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] sort 出力の型
  /// @param[in] input_num 入力数
  SmtFun1(const SmtId* name,
	  const SmtSort* sort,
	  ymuint input_num);

  /// @brief デストラクタ
  virtual
  ~SmtFun1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtSort*
  input_sort(ymuint pos) const;

  /// @brief 入力変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtId*
  input_var(ymuint pos) const;

  /// @brief 本体の式を返す．
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtTerm*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力の型の配列
  // 実際には必要な領域を確保する．
  const SmtSort* mInputList[1];

};


//////////////////////////////////////////////////////////////////////
/// @class SmtFun2 SmtFunImpl.h "SmtFunImpl.h"
/// @brief 定義を持つ関数
//////////////////////////////////////////////////////////////////////
class SmtFun2 :
  public SmtFunImpl
{
  friend class SmtFunMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] sort 出力の型
  /// @param[in] input_num 入力数
  /// @param[in] body 本体
  SmtFun2(const SmtId* name,
	  const SmtSort* sort,
	  ymuint input_num,
	  const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtFun2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtSort*
  input_sort(ymuint pos) const;

  /// @brief 入力変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtId*
  input_var(ymuint pos) const;

  /// @brief 本体の式を返す．
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtTerm*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  const SmtTerm* mBody;

  // 入力の変数と型の配列
  // 実際には必要な領域を確保する．
  SmtSortedVar mInputList[1];

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTFUNIMPL_H
