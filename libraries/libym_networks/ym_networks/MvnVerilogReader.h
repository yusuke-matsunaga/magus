#ifndef YM_MVN_MVNVERILOGREADER_H
#define YM_MVN_MVNVERILOGREADER_H

/// @file ym_networks/MvnVerilogReader.h
/// @brief MvnVerilogReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/mvn_nsdef.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/vl/VlFwd.h"
#include "ym_utils/File.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

class ReaderImpl;

END_NAMESPACE_YM_MVN_VERILOG


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnVerilogReader MvnVerilogReader.h "ym_networks/MvnVerilogReader.h"
/// @brief 合成可能な Verilog 記述を読み込んで MVN に設定するクラス
//////////////////////////////////////////////////////////////////////
class MvnVerilogReader
{
public:

  /// @brief コンストラクタ
  MvnVerilogReader();

  /// @brief デストラクタ
  ~MvnVerilogReader();


public:

  /// @brief 内部のデータをクリアする．
  void
  clear();

  /// @brief verilog 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] searchpath サーチパス
  /// @param[in] watcher_list 行番号ウォッチャーのリスト
  /// @retval true 正常に読み込めた．
  /// @retval false 読込中にエラーが起こった．
  bool
  read(const string& filename,
       const SearchPathList& searchpath = SearchPathList(),
       const list<VlLineWatcher*> watcher_list = list<VlLineWatcher*>());

  /// @brief 今まで読み込んだ情報からネットワークを生成する．
  /// @param[in] mgr ネットワーク生成用のマネージャ
  /// @param[out] node_map MvNode と宣言要素の対応付けを保持する配列
  /// @retval true 正常に処理を行った．
  /// @retval false 生成中にエラーが起こった．
  bool
  gen_network(MvnMgr& mgr,
	      MvnVlMap& node_map);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の読み込みを行うオブジェクト
  nsVerilog::ReaderImpl* mImpl;

};

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVNVERILOGREADER_H
