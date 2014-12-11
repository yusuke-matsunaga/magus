
/// @file YmslVM.cc
/// @brief YmslVM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslVM
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslVM::YmslVM()
{
}

// @brief デストラクタ
YmslVM::~YmslVM()
{
}

// @brief バイトコードを実行する．
void
YmslVM::execute(ymuint code_size,
		const ymuint8* byte_codes)
{
  for (ymuint i = 0; i < code_size; ++ i) {
    ymuint8 code = byte_coes[i];

    switch ( code ) {
    case YMVM_PUSH_INT8_IMM:

    }
  }
}

END_NAMESPACE_YM_YMSL
