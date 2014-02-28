
/// @file MsgType.sip
/// @brief MsgMgr, MsgHandler 用の型定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

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