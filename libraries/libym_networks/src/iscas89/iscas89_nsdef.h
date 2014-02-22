#ifndef ISCAS89_NSDEF_H
#define ISCAS89_NSDEF_H

/// @file iscas89_nsdef.h
/// @brief nsIscas89 の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup Iscas89Group ISCAS89フォーマットパーサ
///
/// ISCAS89 ベンチマーク回路フォーマット用パーサ


#include "networks/networks_nsdef.h"


/// @brief iscas89 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_ISCAS89 \
BEGIN_NAMESPACE_YM_NETWORKS \
BEGIN_NAMESPACE(nsIscas89)

/// @brief iscas89 用の名前空間の終了
#define END_NAMESPACE_YM_ISCAS89 \
END_NAMESPACE(nsIscas89) \
END_NAMESPACE_YM_NETWORKS


BEGIN_NAMESPACE_YM_ISCAS89

// クラスの前方宣言
class Iscas89Handler;
class Iscas89Parser;
class Iscas89ParserImpl;
class Iscas89Network;
class Iscas89Node;
class Iscas89NetworkReader;

/// @brief ゲートの型
/// @ingroup Iscas89Group
enum tIscas89GateType {
  kGt89BUFF,
  kGt89NOT,
  kGt89AND,
  kGt89NAND,
  kGt89OR,
  kGt89NOR,
  kGt89XOR,
  kGt89XNOR,
  kGt89DFF
};

END_NAMESPACE_YM_ISCAS89

#endif // ISCAS89_NSDEF_H
