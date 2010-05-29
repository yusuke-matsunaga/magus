#ifndef MAGUS_SEAL_SEAL_H
#define MAGUS_SEAL_SEAL_H

/// @file magus/seal/seal.h
/// @brief seal パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: seal.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "magus.h"

/// @brief seal 用の名前空間の開始
#define BEGIN_NAMESPACE_MAGUS_SEAL \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(nsSeal)

/// @brief seal 用の名前空間の終了
#define END_NAMESPACE_MAGUS_SEAL \
END_NAMESPACE(nsSeal) \
END_NAMESPACE_MAGUS

#endif // MAGUS_SEAL_SEAL_H
