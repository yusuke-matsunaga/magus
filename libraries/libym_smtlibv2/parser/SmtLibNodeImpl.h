#ifndef LIBYM_SMTLIBV2_SMTLIBNODEIMPL_H
#define LIBYM_SMTLIBV2_SMTLIBNODEIMPL_H

/// @file libym_smtlibv2/SmtLibNodeImpl.h
/// @brief SmtLibNode の継承クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibNode.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtLibNodeBase SmtLibNodeImpl.h "SmtLibNodeImpl.h"
/// @brief SmtLibNode の継承クラス
//////////////////////////////////////////////////////////////////////
class SmtLibNodeBase :
  public SmtLibNode
{
protected:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  SmtLibNodeBase(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~SmtLibNodeBase();


public:
  //////////////////////////////////////////////////////////////////////
  // SmtLibNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル上の位置
  FileRegion mLoc;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLibTerminalNode SmtLibNodeImpl.h "SmtLibNodeImpl.h"
/// @brief 終端ノードを表す SmtLibNode の継承クラス
//////////////////////////////////////////////////////////////////////
class SmtLibTerminalNode :
  public SmtLibNodeBase
{
protected:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibTerminalNode(const FileRegion& loc,
		     const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtLibTerminalNode();


public:
  //////////////////////////////////////////////////////////////////////
  // SmtLibNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 終端型の場合の値を返す．
  virtual
  const char*
  value() const;

  /// @brief LIST型の場合の子供のノードの要素数を返す．
  virtual
  ymuint
  child_num() const;

  /// @brief LIST型の場合の子供のノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < child_num() )
  virtual
  const SmtLibNode*
  child(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ShString mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLibNumNode SmtLibNodeImpl.h "SmtLibNodeImpl.h"
/// @brief Num を表す SmtLibNode の継承クラス
//////////////////////////////////////////////////////////////////////
class SmtLibNumNode :
  public SmtLibTerminalNode
{
  friend class SmtLibParser;

private:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNumNode(const FileRegion& loc,
		const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtLibNumNode();


public:
  //////////////////////////////////////////////////////////////////////
  // SmtLibNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tTokenType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLibDecNode SmtLibNodeImpl.h "SmtLibNodeImpl.h"
/// @brief Dec を表す SmtLibNode の継承クラス
//////////////////////////////////////////////////////////////////////
class SmtLibDecNode :
  public SmtLibTerminalNode
{
  friend class SmtLibParser;

private:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibDecNode(const FileRegion& loc,
		const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtLibDecNode();


public:
  //////////////////////////////////////////////////////////////////////
  // SmtLibNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tTokenType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLibHexNode SmtLibNodeImpl.h "SmtLibNodeImpl.h"
/// @brief Hex を表す SmtLibNode の継承クラス
//////////////////////////////////////////////////////////////////////
class SmtLibHexNode :
  public SmtLibTerminalNode
{
  friend class SmtLibParser;

private:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibHexNode(const FileRegion& loc,
		const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtLibHexNode();


public:
  //////////////////////////////////////////////////////////////////////
  // SmtLibNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tTokenType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLibBinNode SmtLibNodeImpl.h "SmtLibNodeImpl.h"
/// @brief Bin を表す SmtLibNode の継承クラス
//////////////////////////////////////////////////////////////////////
class SmtLibBinNode :
  public SmtLibTerminalNode
{
  friend class SmtLibParser;

private:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibBinNode(const FileRegion& loc,
		const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtLibBinNode();


public:
  //////////////////////////////////////////////////////////////////////
  // SmtLibNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tTokenType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLibStringNode SmtLibNodeImpl.h "SmtLibNodeImpl.h"
/// @brief String を表す SmtLibNode の継承クラス
//////////////////////////////////////////////////////////////////////
class SmtLibStringNode :
  public SmtLibTerminalNode
{
  friend class SmtLibParser;

private:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibStringNode(const FileRegion& loc,
		   const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtLibStringNode();


public:
  //////////////////////////////////////////////////////////////////////
  // SmtLibNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tTokenType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLibSymbolNode SmtLibNodeImpl.h "SmtLibNodeImpl.h"
/// @brief Symbol を表す SmtLibNode の継承クラス
//////////////////////////////////////////////////////////////////////
class SmtLibSymbolNode :
  public SmtLibTerminalNode
{
  friend class SmtLibParser;

private:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibSymbolNode(const FileRegion& loc,
		   const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtLibSymbolNode();


public:
  //////////////////////////////////////////////////////////////////////
  // SmtLibNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tTokenType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLibKeywordNode SmtLibNodeImpl.h "SmtLibNodeImpl.h"
/// @brief Keyword を表す SmtLibNode の継承クラス
//////////////////////////////////////////////////////////////////////
class SmtLibKeywordNode :
  public SmtLibTerminalNode
{
  friend class SmtLibParser;

private:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibKeywordNode(const FileRegion& loc,
		    const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtLibKeywordNode();


public:
  //////////////////////////////////////////////////////////////////////
  // SmtLibNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tTokenType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLibListNode SmtLibNodeImpl.h "SmtLibNodeImpl.h"
/// @brief 終端ノードを表す SmtLibNode の継承クラス
//////////////////////////////////////////////////////////////////////
class SmtLibListNode :
  public SmtLibNodeBase
{
  friend class SmtLibParser;

private:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  SmtLibListNode(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~SmtLibListNode();


public:
  //////////////////////////////////////////////////////////////////////
  // SmtLibNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tTokenType
  type() const;

  /// @brief 終端型の場合の値を返す．
  virtual
  const char*
  value() const;

  /// @brief LIST型の場合の子供のノードの要素数を返す．
  virtual
  ymuint
  child_num() const;

  /// @brief LIST型の場合の子供のノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < child_num() )
  virtual
  const SmtLibNode*
  child(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 子供の要素数
  ymuint32 mChildNum;

  // 子供のノードの配列
  SmtLibNode** mChildArray;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // LIBYM_SMTLIBV2_SMTLIBNODEIMPL_H
