#ifndef LLVMEQ_NSDEF_H
#define LLVMEQ_NSDEF_H

/// @file llvmeq_nsdef.h
/// @brief llvmeq 用の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

//////////////////////////////////////////////////////////////////////
// 名前空間の定義用マクロ
// namespace でネストするのがいやなので define マクロでごまかす．
//////////////////////////////////////////////////////////////////////

#define BEGIN_NAMESPACE_YM_LLVMEQ \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsLLVMeq)

#define END_NAMESPACE_YM_LLVMEQ \
END_NAMESPACE(nsLLVMeq) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_LLVMEQ

class EufNode;
class EufMgr;

END_NAMESPACE_YM_LLVMEQ

#endif // LLVMEQ_NSDEF_H
