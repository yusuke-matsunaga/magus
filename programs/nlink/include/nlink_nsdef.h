#ifndef NLINK_NSDEF_H
#define NLINK_NSDEF_H

/// @file nlink_nsdef.h
/// @brief nlink_nsdef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

//////////////////////////////////////////////////////////////////////
// 名前空間の定義用マクロ
// namespace でネストするのがいやなので define マクロでごまかす．
//////////////////////////////////////////////////////////////////////

#define BEGIN_NAMESPACE_YM_NLINK \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsNlink)

#define END_NAMESPACE_YM_NLINK \
END_NAMESPACE(nsNlink) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス名の宣言
//////////////////////////////////////////////////////////////////////

class NlConnection;
class NlGraph;
class NlNode;
class NlEdge;
class NlPoint;
class NlProblem;
class NlSolution;
class NlSolver;

END_NAMESPACE_YM_NLINK

#endif // NLINK_NSDEF_H
