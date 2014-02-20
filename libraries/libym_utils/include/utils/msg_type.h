#ifndef UTILS_MSG_TYPE_H
#define UTILS_MSG_TYPE_H

/// @file utils/msg_type.h
/// @brief MsgMgr, MsgHandler 用の型定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief メッセージの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tMsgType {
  /// @brief エラー
  /// @note 不正なデータなどの再現性のある致命的な不具合
  kMsgError   = 1,
  /// @brief 警告
  /// @note 軽微な不具合
  kMsgWarning = 2,
  /// @brief 失敗
  /// @note メモリ不足，ファイル読み書き失敗のような外的要因の不具合
  kMsgFailure = 3,
  /// @brief 情報
  /// @note 付加的な情報
  kMsgInfo    = 4,
  /// @brief デバッグ情報
  /// @note デバッグ用の付加的な情報
  kMsgDebug   = 5
};

/// @brief tMsgType のストリーム出力演算子
/// @param[in] s 出力先のストリーム
/// @param[in] type メッセージの種類
ostream&
operator<<(ostream& s,
	   tMsgType type);


//////////////////////////////////////////////////////////////////////
// マスク用の定数
//////////////////////////////////////////////////////////////////////

/// @brief kMsgError 用のビットマスク
const ymuint32 kMaskError = (1U << static_cast<ymuint32>(kMsgError));

/// @brief kMsgWarning 用のビットマスク
const ymuint32 kMaskWarning = (1U << static_cast<ymuint32>(kMsgWarning));

/// @brief kMsgInfo 用のビットマスク
const ymuint32 kMaskInfo = (1U << static_cast<ymuint32>(kMsgInfo));

/// @brief kMsgFailure 用のビットマスク
const ymuint32 kMaskFailure = (1U << static_cast<ymuint32>(kMsgFailure));

/// @brief kMsgDebug 用のビットマスク
const ymuint32 kMaskDebug = (1U << static_cast<ymuint32>(kMsgDebug));

/// @brief 全てのを含むビットマスク
const ymuint32 kMaskAll =
  kMaskError | kMaskWarning | kMaskInfo | kMaskFailure | kMaskDebug;

/// @brief 全てを含まないビットマスク
const ymuint32 kMaskNone = 0U;

END_NAMESPACE_YM

#endif // UTILS_MSG_TYPE_H
