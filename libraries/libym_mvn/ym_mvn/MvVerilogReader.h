#ifndef YM_MVN_MVVERILOGREADER_H
#define YM_MVN_MVVERILOGREADER_H

/// @file ym_mvn/MvVerilogReader.h
/// @brief MvVerilogReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_verilog/VlMgr.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvVerilogReader MvVerilogReader.h "ym_mvn/MvVerilogReader.h"
/// @brief 合成可能な Verilog 記述を読み込んで MVN に設定するクラス
//////////////////////////////////////////////////////////////////////
class MvVerilogReader
{
public:

  /// @brief コンストラクタ
  MvVerilogReader();

  /// @brief デストラクタ
  ~MvVerilogReader();


public:

  /// @brief 内部のデータをクリアする．
  void
  clear();
  
  /// @brief verilog 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @retval true 正常に読み込めた．
  /// @retval false 読込中にエラーが起こった．
  bool
  read(const string& filename,
       const SearchPathList& searchpath = SearchPathList(),
       const list<VlLineWatcher*> watcher_list = list<VlLineWatcher*>());

  /// @brief 今まで読み込んだ情報からネットワークを生成する．
  /// @param[in] mgr ネットワーク生成用のマネージャ
  /// @param[in] searchpath サーチパス
  /// @param[in] watcher_list 行番号ウォッチャーのリスト
  /// @retval true 正常に処理を行った．
  /// @retval false 生成中にエラーが起こった．
  bool
  gen_network(MvMgr& mgr);
  
  /// @brief メッセージハンドラを付加する．
  /// @param[in] msg_handler 登録するハンドラ
  /// @note このハンドラはこのオブジェクトの破壊に伴って破壊される．
  void
  add_msg_handler(MsgHandler* msg_handler);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // メッセージハンドラの管理者
  MsgMgr mMsgMgr;

  // Verilog を扱うマネージャ
  VlMgr mVlMgr;
  
};

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVVERILOGREADER_H
