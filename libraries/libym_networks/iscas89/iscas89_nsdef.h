#ifndef LIBYM_NETWORKS_ISCAS89_ISCAS89_NSDEF_H
#define LIBYM_NETWORKS_ISCAS89_ISCAS89_NSDEF_H

/// @file ym_iscas89/iscas89_nsdef.h
/// @brief nsIscas89 の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: iscas89_nsdef.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup Iscas89Group ISCAS89フォーマットパーサ
///
/// ISCAS89 ベンチマーク回路フォーマット用パーサ


#include "ym_networks/networks_nsdef.h"


/// @brief ym_iscas89 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_ISCAS89 \
BEGIN_NAMESPACE_YM_NETWORKS \
BEGIN_NAMESPACE(nsIscas89)

/// @brief ym_iscas89 用の名前空間の終了
#define END_NAMESPACE_YM_ISCAS89 \
END_NAMESPACE(nsIscas89) \
END_NAMESPACE_YM_NETWORKS


BEGIN_NAMESPACE_YM_ISCAS89

// クラスの前方宣言
class Iscas89Handler;
class Iscas89Parser;
class Iscas89ParserImpl;

END_NAMESPACE_YM_ISCAS89

#endif // LIBYM_NETWORKS_ISCAS89_ISCAS89_NSDEF_H
