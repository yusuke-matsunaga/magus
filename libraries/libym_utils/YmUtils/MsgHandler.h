#ifndef YMUTILS_MSGHANDLER_H
#define YMUTILS_MSGHANDLER_H

/// @file YmUtils/MsgHandler.h
/// @brief MsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "YmUtils/MsgType.h"
#include "YmUtils/Binder.h"


BEGIN_NAMESPACE_YM

class FileRegion;

//////////////////////////////////////////////////////////////////////
/// @class MsgHandler MsgHandler.h "YmUtils/MsgHandler.h"
/// @ingroup YmUtils
/// @brief メッセージハンドラを表す基底クラス
//////////////////////////////////////////////////////////////////////
class MsgHandler :
  public T6Binder<const char*,
		  int,
		  const FileRegion&,
		  MsgType,
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
  add_mask(MsgType type);

  /// @brief マスクの削除
  /// @param[in] type 削除するメッセージタイプ
  /// @note type はビットマスクではない．
  void
  delete_mask(MsgType type);


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& loc,
	  MsgType type,
	  const char* label,
	  const char* body) = 0;

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  put_msg(const char* src_file,
	  int src_line,
	  MsgType type,
	  const char* label,
	  const char* body);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  event_proc(const char* src_file,
	     int src_line,
	     const FileRegion& loc,
	     MsgType type,
	     const char* label,
	     const char* body);

  /// @brief メッセータイプからビットマスクを得る．
  static
  ymuint32
  conv2bitmask(MsgType type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージマスク
  ymuint32 mMask;

};


//////////////////////////////////////////////////////////////////////
/// @class StreamMsgHandler MsgHandler.h "YmUtils/MsgHandler.h"
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
  /// @param[in] loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& loc,
	  MsgType type,
	  const char* label,
	  const char* body);

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  put_msg(const char* src_file,
	  int src_line,
	  MsgType type,
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
MsgHandler::conv2bitmask(MsgType type)
{
  return 1U << static_cast<ymuint32>(type);
}

END_NAMESPACE_YM

#endif // YMUTILS_MSGHANDLER_H
