#ifndef ATPGMSGHANDLER_H
#define ATPGMSGHANDLER_H

/// @file AtpgMsgHandler.h
/// @brief AtpgMsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "YmUtils/MsgHandler.h"
#include "YmTclpp/TclObj.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class AtpgMsgHandler AtpgMsgHandler.h
/// @brief ReadBlif/ReadIscas89 用のメッセージハンドラ
//////////////////////////////////////////////////////////////////////
class AtpgMsgHandler :
  public MsgHandler
{
public:

  /// @brief コンストラクタ
  AtpgMsgHandler();

  /// @brief デストラクタ
  virtual
  ~AtpgMsgHandler();


public:

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

  /// @brief 内容をクリアする．
  void
  clear();


public:

  /// @brief メッセージオブジェクトを取り出す．
  TclObj
  msg_obj() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  TclObj mMsg;

};

END_NAMESPACE_YM_SATPG

#endif // ATPGMSGHANDLER_H
