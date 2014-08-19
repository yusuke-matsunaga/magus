#ifndef SMTLIBNODE_H
#define SMTLIBNODE_H

/// @file SmtLibNode.h
/// @brief SmtLibNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "smtlibv2_nsdef.h"
#include "YmUtils/ShString.h"
#include "YmUtils/FileRegion.h"
#include "TokenType.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtLibNode SmtLibNode.h "SmtLibNode.h"
/// @brief SmtLib のS式を表すノードの基底クラス
//////////////////////////////////////////////////////////////////////
class SmtLibNode
{
  friend class SmtLibParser;
protected:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  /// @param[in] id ID番号
  SmtLibNode(const FileRegion& loc,
	     ymuint id);

  /// @brief デストラクタ
  virtual
  ~SmtLibNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tTokenType
  type() const = 0;

  /// @brief ファイル上の位置を返す．
  FileRegion
  loc() const;

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief NUM型の場合の整数値を返す．
  virtual
  ymint32
  int_value() const;

  /// @brief 終端型の場合の文字列値を返す．
  virtual
  ShString
  str_value() const;

  /// @brief LIST型の場合の子供のノードの要素数を返す．
  virtual
  ymuint
  child_num() const;

  /// @brief LIST型の場合の子供の先頭のノードを返す．
  virtual
  const SmtLibNode*
  child() const;

  /// @brief LISTの要素の場合の次のノードを返す．
  const SmtLibNode*
  sibling() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル上の位置
  FileRegion mLoc;

  // ID 番号
  ymuint32 mId;

  // 次のノード
  SmtLibNode* mSibling;

};


/// @relates SmtLibNode
/// @brief SmtLibNode の内容を出力する(デバッグ用)．
/// @param[in] s 出力先のストリーム
/// @param[in] node 対象のノード
/// @param[in] ident_level 字下げのレベル
/// @param[in] print_loc ファイル位置の情報を出力するとき true にするフラグ
extern
void
display(ostream& s,
	const SmtLibNode* node,
	ymuint ident_level = 0,
	bool print_loc = false);

/// @relates SmtLibNode
/// @brief SmtLibNode の内容をもとの形で出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] node 対象のノード
extern
void
print(ostream& s,
      const SmtLibNode* node);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ファイル上の位置を返す．
inline
FileRegion
SmtLibNode::loc() const
{
  return mLoc;
}

// @brief ID番号を返す．
inline
ymuint
SmtLibNode::id() const
{
  return mId;
}

// @brief LISTの要素の場合の次のノードを返す．
inline
const SmtLibNode*
SmtLibNode::sibling() const
{
  return mSibling;
}

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTLIBNODE_H
