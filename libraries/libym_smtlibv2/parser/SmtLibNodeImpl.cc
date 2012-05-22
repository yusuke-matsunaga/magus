
/// @file libym_smtlibv2/SmtLibNodeImpl.cc
/// @brief SmtLibNode の継承クラスの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibNodeImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtLibNodeBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
SmtLibNodeBase::SmtLibNodeBase(const FileRegion& loc) :
  mLoc(loc)
{
}

// @brief デストラクタ
SmtLibNodeBase::~SmtLibNodeBase()
{
}

// @brief ファイル上の位置を返す．
FileRegion
SmtLibNodeBase::loc() const
{
  return mLoc;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLibTerminalNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibTerminalNode::SmtLibTerminalNode(const FileRegion& loc,
				       const ShString& val) :
  SmtLibNodeBase(loc),
  mVal(val)
{
}

// @brief デストラクタ
SmtLibTerminalNode::~SmtLibTerminalNode()
{
}

// @brief 終端型の場合の値を返す．
const char*
SmtLibTerminalNode::value() const
{
  return mVal;
}

// @brief LIST型の場合の子供のノードの要素数を返す．
ymuint
SmtLibTerminalNode::child_num() const
{
  return 0;
}

// @brief LIST型の場合の子供のノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < child_num() )
const SmtLibNode*
SmtLibTerminalNode::child(ymuint pos) const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLibNumNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNumNode::SmtLibNumNode(const FileRegion& loc,
			     const ShString& val) :
  SmtLibTerminalNode(loc, val)
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
// クラス SmtLibStringNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibStringNode::SmtLibStringNode(const FileRegion& loc,
				   const ShString& val) :
  SmtLibTerminalNode(loc, val)
{
}

// @brief デストラクタ
SmtLibStringNode::~SmtLibStringNode()
{
}

// @brief 型を返す．
tTokenType
SmtLibStringNode::type() const
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
SmtLibListNode::SmtLibListNode(const FileRegion& loc) :
  SmtLibNodeBase(loc),
  mChildNum(0),
  mChildArray(NULL)
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

// @brief 終端型の場合の値を返す．
const char*
SmtLibListNode::value() const
{
  return NULL;
}

// @brief LIST型の場合の子供のノードの要素数を返す．
ymuint
SmtLibListNode::child_num() const
{
  return mChildNum;
}

// @brief LIST型の場合の子供のノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < child_num() )
const SmtLibNode*
SmtLibListNode::child(ymuint pos) const
{
  return mChildArray[pos];
}

END_NAMESPACE_YM_SMTLIBV2
