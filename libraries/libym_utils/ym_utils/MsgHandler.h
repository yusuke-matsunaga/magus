#ifndef YM_UTILS_MSGHANDLER_H
#define YM_UTILS_MSGHANDLER_H

/// @file ym_utils/MsgHandler.h
/// @brief MsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MsgHandler.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/msg_type.h"
#include "ym_utils/Binder.h"


BEGIN_NAMESPACE_YM

class FileRegion;

//////////////////////////////////////////////////////////////////////
/// @class MsgHandler MsgHandler.h <ym_utils/MsgHandler.h>
/// @ingroup YmUtils
/// @brief メッセージハンドラを表す基底クラス
//////////////////////////////////////////////////////////////////////
class MsgHandler :
  public T6Binder<const char*,
		  int,
		  const FileRegion&,
		  tMsgType,
		  const char*,
		  const char*>
{
public:

  /// @brief コンストラクタ
  /// @param[in] mask メッセージマスク
  MsgHandler(ymuint32 mask = kMaskAll);

  /// @brief デストラクタ
  /// @note 登録されていたら削除する．
  virtual
  ~MsgHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // メッセージマスクの管理
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージマスクの設定
  void
  set_mask(ymuint32 mask);

  /// @brief メッセージマスクの取得
  ymuint32
  mask() const;

  /// @brief マスクの付加
  /// @param[in] type 付加するメッセージタイプ
  /// @note type はビットマスクではない．
  void
  add_mask(tMsgType type);

  /// @brief マスクの削除
  /// @param[in] type 削除するメッセージタイプ
  /// @note type はビットマスクではない．
  void
  delete_mask(tMsgType type);


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& loc,
	  tMsgType type,
	  const char* label,
	  const char* body) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  event_proc(const char* src_file,
	     int src_line,
	     const FileRegion& loc,
	     tMsgType type,
	     const char* label,
	     const char* body);

  /// @brief メッセータイプからビットマスクを得る．
  static
  ymuint32
  conv2bitmask(tMsgType type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージマスク
  ymuint32 mMask;

};


//////////////////////////////////////////////////////////////////////
/// @class StreamMsgHandler MsgHandler.h <ym_utils/MsgHandler.h>
/// @ingroup YmUtils
/// @brief ストリーム出力する MsgHandler の実装
//////////////////////////////////////////////////////////////////////
class StreamMsgHandler :
  public MsgHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] stream_ptr ストリームへのポインタ
  StreamMsgHandler(ostream* stream_ptr);

  /// @brief デストラクタ
  virtual
  ~StreamMsgHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // MsgHandler (T6Binder) の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& loc,
	  tMsgType type,
	  const char* label,
	  const char* body);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  ostream* mStreamPtr;

};


//////////////////////////////////////////////////////////////////////
//　インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief メッセータイプからビットマスクを得る．
inline
ymuint32
MsgHandler::conv2bitmask(tMsgType type)
{
  return 1U << static_cast<ymuint32>(type);
}

END_NAMESPACE_YM

#endif // YM_UTILS_MSGHANDLER_H
