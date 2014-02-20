
/// @file FileInfo.sip
/// @brief FinleInfo の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

class FileInfo {

%TypeHeaderCode
#include "utils/FileInfo.h"
using namespace nsYm;
%End

public:

  /// @brief 空のコンストラクタ
  /// @note 無効な ID で初期化される．
  FileInfo();

  /// @brief ファイル名を指定したコンストラクタ
  /// @param[in] filename ファイル名
  FileInfo(const char* filename);

  /// @brief ファイル名を指定したコンストラクタ
  /// @param[in] filename ファイル名
  FileInfo(const string& filename);

  /// @brief ファイル名とインクルード元の親ファイルの情報
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  FileInfo(const char* filename,
	   const FileLoc& parent_loc);

  /// @brief ファイル名とインクルード元の親ファイルの情報
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  FileInfo(const string& filename,
	   const FileLoc& parent_loc);

  /// @brief 有効なデータを持っているときに true を返す．
  bool
  is_valid() const;

  /// @brief ID番号を得る．
  /// @note ほとんど等価比較演算子のための関数
  ymuint
  id() const;

  /// @brief ファイル名を返す．
  string
  filename() const;

  /// @brief インクルード元のファイル位置を返す．
  /// @note インクルードされていないファイルの場合には無効なデータが返される．
  FileLoc
  parent_loc() const;

  /// @brief インクルード元のファイル位置の情報をすべてリストに積む．
  /// @param[out] loc_list ファイルの位置情報のリスト
  /// @note トップレベルのファイルが先頭になる．
  void
  parent_loc_list(list<FileLoc>& loc_list) const;

};

/// @relates FileInfo
/// @brief 等価比較演算子
/// @param[in] left, right オペランド
bool
operator==(FileInfo left,
	   FileInfo right);
