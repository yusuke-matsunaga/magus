
/// @file utils/FileLoc.h
/// @brief FileLocのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

class FileLoc {

%TypeHeaderCode
#include "utils/FileLoc.h"
using nsYm::FileLoc;
%End

public:

  /// @brief 空のコンストラクタ
  /// @note 無効なデータを持つ
  FileLoc();

  /// @brief 内容を指定するコンストラクタ
  /// @param[in] file_info ファイル情報
  /// @param[in] line 行番号
  /// @param[in] column コラム番号
  FileLoc(FileInfo file_info,
	  unsigned int line,
	  unsigned int column);

  /// @brief データの妥当性のチェック
  /// @retval true 意味のある値を持っている時
  /// @retval false 無効なデータの時
  bool
  is_valid() const;

  /// @brief ファイル情報の取得
  /// @return ファイル情報を返す．
  FileInfo
  file_info() const;

  /// @brief ファイル名を返す．
  const char*
  filename() const;
  %MethodCode
  sipRes = sipCpp->filename().c_str();
  %End

  /// @brief インクルード元のファイル位置を返す．
  /// @note インクルードされていないファイルの場合には無効なデータが返される．
  FileLoc
  parent_loc() const;

  /// @brief インクルード元のファイル位置の情報をすべてリストに積む．
  /// @param[out] loc_list ファイルの位置情報のリスト
  /// @note トップレベルのファイルが先頭になる．
  SIP_PYLIST
  parent_loc_list() const;
  %MethodCode
  using std::list;
  list<FileLoc> loc_list;
  sipCpp->parent_loc_list(loc_list);
  ymuint n = loc_list.size();
  sipRes = PyList_New(n);
  ymuint i = 0;
  for (list<FileLoc>::iterator p = loc_list.begin();
       p != loc_list.end(); ++ p, ++ i) {
    FileLoc& fl = *p;
    PyObject* obj = sipConvertFromNewType(&fl, sipType_FileLoc, NULL);
    PyList_SetItem(sipRes, i, obj);
  }
  %End

  /// @brief 行番号の取得
  /// @return 行番号
  unsigned int
  line() const;

  /// @brief コラム位置の取得
  /// @return コラム位置
  unsigned int
  column() const;

};