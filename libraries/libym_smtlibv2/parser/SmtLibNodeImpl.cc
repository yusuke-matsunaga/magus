
/// @file libym_smtlibv2/SmtLibNodeImpl.cc
/// @brief SmtLibNode の継承クラスの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibNodeImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtLibNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] id ID番号
SmtLibNode::SmtLibNode(const FileRegion& loc,
		       ymuint id) :
  mLoc(loc),
  mId(id),
  mSibling(NULL)
{
}

// @brief デストラクタ
SmtLibNode::~SmtLibNode()
{
}

// @brief 終端型の場合の値を返す．
ShString
SmtLibNode::str_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

// @brief NUM型の場合の整数値を返す．
ymint32
SmtLibNode::int_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief LIST型の場合の子供のノードの要素数を返す．
ymuint
SmtLibNode::child_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief LIST型の場合の子供の先頭のノードを返す．
const SmtLibNode*
SmtLibNode::child() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


BEGIN_NONAMESPACE

// 字下げ用の空白を出力する．
void
print_space(ostream& s,
	    ymuint ident_level)
{
  for (ymuint i = 0; i < ident_level; ++ i) {
    s << "  ";
  }
}

END_NONAMESPACE

// @relates SmtLibNode
// @brief SmbLitNode の内容を出力する(デバッグ用)．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
// @param[in] ident_level 字下げのレベル
// @param[in] print_loc ファイル位置の情報を出力するとき true にするフラグ
void
display(ostream& s,
	const SmtLibNode* node,
	ymuint ident_level,
	bool print_loc)
{
  if ( print_loc ) {
    print_space(s, ident_level);
    s << "Loc:  " << node->loc() << endl;
  }
  const char* type_str = NULL;
  switch ( node->type() ) {
  case kNumToken:     type_str = "Numeral"; break;
  case kDecToken:     type_str = "Decimal"; break;
  case kHexToken:     type_str = "Hexadecimal"; break;
  case kBinToken:     type_str = "Binary"; break;
  case kStringToken:  type_str = "String"; break;
  case kSymbolToken:  type_str = "Symbol"; break;
  case kKeywordToken: type_str = "Keyword"; break;
  case kListToken:    type_str = "List"; break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  print_space(s, ident_level);
  s << "Type: " << type_str << endl;
  if ( node->type() == kListToken ) {
    ymuint ident_level1 = ident_level + 1;
    ymuint n = node->child_num();
    const SmtLibNode* child = node->child();
    for (ymuint i = 0; i < n; ++ i, child = child->sibling()) {
      print_space(s, ident_level1);
      s << "Child#" << i << endl;
      display(s, child, ident_level1, print_loc);
    }
  }
  else if ( node->type() == kNumToken ) {
    print_space(s, ident_level);
    s << "Value: " << node->int_value() << endl;
  }
  else {
    print_space(s, ident_level);
    s << "Value: " << node->str_value() << endl;
  }
}

// @relates SmtLibNode
// @brief SmtLibNode の内容をもとの形で出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
print(ostream& s,
      const SmtLibNode* node)
{
  for ( ; node != NULL; node = node->sibling()) {
    s << " ";
    switch ( node->type() ) {
    case kNumToken:
      s << node->int_value();
      break;

    case kDecToken:
    case kHexToken:
    case kBinToken:
    case kStringToken:
    case kSymbolToken:
    case kKeywordToken:
      s << node->str_value();
      break;

    case kListToken:
      s << "(";
      print(s, node->child());
      s << " )";
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLibTerminalNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] id ID番号
// @param[in] val 値
SmtLibTerminalNode::SmtLibTerminalNode(const FileRegion& loc,
				       ymuint id,
				       const ShString& val) :
  SmtLibNode(loc, id),
  mVal(val)
{
}

// @brief デストラクタ
SmtLibTerminalNode::~SmtLibTerminalNode()
{
}

// @brief 終端型の場合の値を返す．
ShString
SmtLibTerminalNode::str_value() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLibNumNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] id ID番号
// @param[in] val 値
SmtLibNumNode::SmtLibNumNode(const FileRegion& loc,
			     ymuint id,
			     ymint32 val) :
  SmtLibNode(loc, id),
  mValue(val)
{
}

// @brief デストラクタ
SmtLibNumNode::~SmtLibNumNode()
{
}

// @brief 型を返す．
tTokenType
SmtLibNumNode::type() const
{
  return kNumToken;
}

// @brief NUM型の場合の整数値を返す．
ymint32
SmtLibNumNode::int_value() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLibDecNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] id ID番号
// @param[in] val 値
SmtLibDecNode::SmtLibDecNode(const FileRegion& loc,
			     ymuint id,
			     const ShString& val) :
  SmtLibTerminalNode(loc, id, val)
{
}

// @brief デストラクタ
SmtLibDecNode::~SmtLibDecNode()
{
}

// @brief 型を返す．
tTokenType
SmtLibDecNode::type() const
{
  return kDecToken;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLibHexNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] id ID番号
// @param[in] val 値
SmtLibHexNode::SmtLibHexNode(const FileRegion& loc,
			     ymuint id,
			     const ShString& val) :
  SmtLibTerminalNode(loc, id, val)
{
}

// @brief デストラクタ
SmtLibHexNode::~SmtLibHexNode()
{
}

// @brief 型を返す．
tTokenType
SmtLibHexNode::type() const
{
  return kHexToken;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLibBinNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] id ID番号
// @param[in] val 値
SmtLibBinNode::SmtLibBinNode(const FileRegion& loc,
			     ymuint id,
			     const ShString& val) :
  SmtLibTerminalNode(loc, id, val)
{
}

// @brief デストラクタ
SmtLibBinNode::~SmtLibBinNode()
{
}

// @brief 型を返す．
tTokenType
SmtLibBinNode::type() const
{
  return kBinToken;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLibStrNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] id ID番号
// @param[in] val 値
SmtLibStrNode::SmtLibStrNode(const FileRegion& loc,
			     ymuint id,
			     const ShString& val) :
  SmtLibTerminalNode(loc, id, val)
{
}

// @brief デストラクタ
SmtLibStrNode::~SmtLibStrNode()
{
}

// @brief 型を返す．
tTokenType
SmtLibStrNode::type() const
{
  return kStringToken;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLibSymbolNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] id ID番号
// @param[in] val 値
SmtLibSymbolNode::SmtLibSymbolNode(const FileRegion& loc,
				   ymuint id,
				   const ShString& val) :
  SmtLibTerminalNode(loc, id, val)
{
}

// @brief デストラクタ
SmtLibSymbolNode::~SmtLibSymbolNode()
{
}

// @brief 型を返す．
tTokenType
SmtLibSymbolNode::type() const
{
  return kSymbolToken;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLibKeywordNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] id ID番号
// @param[in] val 値
SmtLibKeywordNode::SmtLibKeywordNode(const FileRegion& loc,
				     ymuint id,
				     const ShString& val) :
  SmtLibTerminalNode(loc, id, val)
{
}

// @brief デストラクタ
SmtLibKeywordNode::~SmtLibKeywordNode()
{
}

// @brief 型を返す．
tTokenType
SmtLibKeywordNode::type() const
{
  return kKeywordToken;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLibListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] id ID番号
SmtLibListNode::SmtLibListNode(const FileRegion& loc,
			       ymuint id,
			       ymuint num,
			       const SmtLibNode* child) :
  SmtLibNode(loc, id),
  mChildNum(num),
  mChild(child)
{
}

// @brief デストラクタ
SmtLibListNode::~SmtLibListNode()
{
}

// @brief 型を返す．
tTokenType
SmtLibListNode::type() const
{
  return kListToken;
}

// @brief LIST型の場合の子供のノードの要素数を返す．
ymuint
SmtLibListNode::child_num() const
{
  return mChildNum;
}

// @brief LIST型の場合の子供のノードを返す．
const SmtLibNode*
SmtLibListNode::child() const
{
  return mChild;
}

END_NAMESPACE_YM_SMTLIBV2
