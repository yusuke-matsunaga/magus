
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
SmtLibNode::SmtLibNode(const FileRegion& loc) :
  mLoc(loc),
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


//////////////////////////////////////////////////////////////////////
// クラス SmtLibTerminalNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibTerminalNode::SmtLibTerminalNode(const FileRegion& loc,
				       const ShString& val) :
  SmtLibNode(loc),
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
// @param[in] val 値
SmtLibNumNode::SmtLibNumNode(const FileRegion& loc,
			     ymint32 val) :
  SmtLibNode(loc),
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
// @param[in] val 値
SmtLibDecNode::SmtLibDecNode(const FileRegion& loc,
			     const ShString& val) :
  SmtLibTerminalNode(loc, val)
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
// @param[in] val 値
SmtLibHexNode::SmtLibHexNode(const FileRegion& loc,
			     const ShString& val) :
  SmtLibTerminalNode(loc, val)
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
// @param[in] val 値
SmtLibBinNode::SmtLibBinNode(const FileRegion& loc,
			     const ShString& val) :
  SmtLibTerminalNode(loc, val)
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
// @param[in] val 値
SmtLibStrNode::SmtLibStrNode(const FileRegion& loc,
			     const ShString& val) :
  SmtLibTerminalNode(loc, val)
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
// @param[in] val 値
SmtLibSymbolNode::SmtLibSymbolNode(const FileRegion& loc,
				   const ShString& val) :
  SmtLibTerminalNode(loc, val)
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
// @param[in] val 値
SmtLibKeywordNode::SmtLibKeywordNode(const FileRegion& loc,
				     const ShString& val) :
  SmtLibTerminalNode(loc, val)
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
SmtLibListNode::SmtLibListNode(const FileRegion& loc,
			       ymuint num,
			       const SmtLibNode* child) :
  SmtLibNode(loc),
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
