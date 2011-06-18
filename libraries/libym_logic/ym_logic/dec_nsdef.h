#ifndef YM_LOGIC_DEC_NSDEF_H
#define YM_LOGIC_DEC_NSDEF_H

/// @file ym_logic/dec_nsdef.h
/// @brief 関数分解パッケージの名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Dg.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief DEC 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_DEC \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsDec)

/// @brief DEC 用の名前空間の終了
#define END_NAMESPACE_YM_DEC \
END_NAMESPACE(nsDec) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_DEC

/// @brief ノードのタイプを表す列挙型
enum tType {
  kC0   = 0, //< 定数0
  kC1   = 1, //< 定数1
  kLitP = 2, //< 肯定のリテラル
  kLitN = 3, //< 否定のリテラル
  kOr   = 4, //< OR
  kAnd  = 5, //< AND
  kXor  = 6, //< XOR
  kXnor = 7, //< XNOR
  kCplx = 8  //< それ以外
};

//////////////////////////////////////////////////////////////////////
// クラス名の前方参照用の宣言
//////////////////////////////////////////////////////////////////////

class DgMgr;
class Dg;
class DgNode;
class PrimDec;
class Bidec;

/// @brief PrimDec のリスト
typedef list<PrimDec> PrimDecList;

/// @brief Bidec のベクタ
typedef vector<Bidec> BidecVector;

/// @brief DgNode を指す枝
typedef ympuint tDgEdge;

END_NAMESPACE_YM_DEC


BEGIN_NAMESPACE_YM

using nsDec::DgMgr;
using nsDec::Dg;
using nsDec::PrimDec;
using nsDec::Bidec;
using nsDec::PrimDecList;
using nsDec::BidecVector;

END_NAMESPACE_YM

#endif // YM_LOGIC_DEC_NSDEF_H
