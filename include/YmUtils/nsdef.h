#ifndef YMUTILS_NSDEF_H
#define YMUTILS_NSDEF_H

/// @file YmUtils/nsdef.h
/// @brief 名前空間の定義に関するヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


//////////////////////////////////////////////////////////////////////
// 名前空間の定義
//////////////////////////////////////////////////////////////////////

/// @brief 名前空間の開始
///
/// @param[in] nm 開始する名前空間名
///
/// namespace でインデントするのがいやなので define マクロでごまかす．
#define BEGIN_NAMESPACE(nm) \
namespace nm {

/// @brief 名前空間の終了
///
/// @param[in] nm 終了する名前空間名
///
/// 実は nm は使っていない．プログラマの確認用
#define END_NAMESPACE(nm) \
}

/// @brief 無名の名前空間の開始
#define BEGIN_NONAMESPACE \
namespace {

/// @brief 無名の名前空間の終了
#define END_NONAMESPACE \
}

/// @brief 標準(std)の名前空間の開始
#define BEGIN_NAMESPACE_STD \
BEGIN_NAMESPACE(std)

/// @brief 標準(std)の名前空間の終了
#define END_NAMESPACE_STD \
END_NAMESPACE(std)

/// @brief hash 用の名前空間の開始
#define BEGIN_NAMESPACE_HASH \
BEGIN_NAMESPACE(HASH_NAMESPACE)

/// @brief hash 用の名前空間の終了
#define END_NAMESPACE_HASH \
END_NAMESPACE(HASH_NAMESPACE)

/// @brief c 用の名前空間の開始
#define BEGIN_EXTERN_C extern "C" {

/// @brief c 用の名前空間の終了
#define END_EXTERN_C }

/// @brief ymtools 用の名前空間の開始
#define BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(YMTOOLS_NAMESPACE)

/// @brief ymtools 用の名前空間の終了
#define END_NAMESPACE_YM \
END_NAMESPACE(YMTOOLS_NAMESPACE)

/// @namespace nsYm
/// @brief ymtools の全ての要素が属する名前空間

/// @defgroup YmUtils ユーティリティモジュール

#endif // YMUTILS_NSDEF_H
