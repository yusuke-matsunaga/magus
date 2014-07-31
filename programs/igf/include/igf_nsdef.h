#ifndef IGF_NSDEF_H
#define IGF_NSDEF_H

/// @file igf_nsdef.h
/// @brief IGF 用の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

//////////////////////////////////////////////////////////////////////
// 名前空間の定義用マクロ
// namespace でネストするのがいやなので define マクロでごまかす．
//////////////////////////////////////////////////////////////////////

#define BEGIN_NAMESPACE_YM_IGF \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsIgf)

#define END_NAMESPACE_YM_IGF \
END_NAMESPACE(nsIgf) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_IGF

class RegVect;
class RvMgr;

class Variable;

class InputFunc;

class FuncVect;

END_NAMESPACE_YM_IGF

#endif // IGF_NSDEF_H
