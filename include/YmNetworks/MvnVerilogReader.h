#ifndef NETWORKS_MVNVERILOGREADER_H
#define NETWORKS_MVNVERILOGREADER_H

/// @file YmNetworks/MvnVerilogReader.h
/// @brief MvnVerilogReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/mvn.h"
#include "YmVerilog/verilog.h"
#include "YmVerilog/vl/VlFwd.h"
#include "YmUtils/File.h"
#include "YmUtils/MsgHandler.h"
#include "YmCell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN_VERILOG

class ReaderImpl;

END_NAMESPACE_YM_NETWORKS_MVN_VERILOG


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnVerilogReader MvnVerilogReader.h "YmNetworks/MvnVerilogReader.h"
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

  /// @brief 今まで読み込んだ情報からネットワークを生成する．
  /// @param[in] mgr ネットワーク生成用のマネージャ
  /// @param[in] library セルライブラリ
  /// @param[out] node_map MvNode と宣言要素の対応付けを保持する配列
  /// @retval true 正常に処理を行った．
  /// @retval false 生成中にエラーが起こった．
  bool
  gen_network(MvnMgr& mgr,
	      const CellLibrary* library,
	      MvnVlMap& node_map);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の読み込みを行うオブジェクト
  nsVerilog::ReaderImpl* mImpl;

};

END_NAMESPACE_YM_NETWORKS_MVN

#endif // NETWORKS_MVNVERILOGREADER_H
