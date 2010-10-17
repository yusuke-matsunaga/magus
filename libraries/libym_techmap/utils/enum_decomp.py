# -*- coding: utf-8 -*-
# @file libym_techmap/utils/enum_decomp.py
# @brief 2項分解をすべて列挙するプログラム
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2005-2010 Yusuke Matsunaga
# All rights reserved.


# @brief リスの長さを返す．
# @param[in] l リスト
# ただし l は多段にネストリストなのでそれを潰したときの要素数を数える．
def list_len( l ) :
    if isinstance( l, list) :
        ans = 0
        for elem in l :
            ans += list_len(elem)
        return ans

    return 1

# @brief 2つの2分木の比較関数
# @param[in] left, right 引数
# @retval 1 left > right
# @retval 0 left = right
# @retval -1 left < right
# 比較関数は一貫していればなんでもよい．
def comp_tree( left, right ) :
    while 1 :
        # 要素数で比較
        l_left = list_len(left)
        l_right = list_len(right)

        if l_left < l_right :
            return 0

        if l_left > l_right :
            return 1

        # ここまできたら同数だった．
        if l_left == 1 :
            return 1

        # 左の子供で繰り返す．
        left = left[0]
        right = right[0]



# @brief 下請け関数
# @param[in] input_num 入力数
# @param[in] base 入力の開始番号
# base から始まる input_num 個の入力を葉とする2分木を全て列挙する．
# ただし，対称なものは重複させない．
def enum_decomp( input_num, base_num ) :
    if input_num == 0 :
        print "Error!: input_num = 0."
        return []

    if input_num == 1 :
        return [ base_num ]

    if input_num == 2 :
        ans_list = [ [ base_num, base_num + 1 ] ]
        return [ [ base_num, base_num + 1 ] ]

    start = (input_num + 1) / 2
    end = input_num
    ans_list = []
    for l in range(start, end) :
        l_list = enum_decomp( l, base_num )
        r = input_num - l
        r_base = base_num + l
        r_list = enum_decomp( r, r_base )
        for e1 in l_list :
            for e2 in r_list :
                if comp_tree( e1, e2 ) :
                    e3 = [ e1, e2 ]
                    ans_list.append( e3 )

    return ans_list


# @brief 2分木を逆ポーランド方式の文字列に変換する．
# @param[in] tree 2分木
def dump_tree ( tree ) :
    if isinstance( tree, list ) :
        if len( tree ) == 2 :
            ans = "+"
            ans += dump_tree( tree[0] )
            ans += " "
            ans += dump_tree( tree[1] )
            return ans;
        else :
            return str(tree[0])
    else :
        return str(tree)

ans = enum_decomp( 10, 0)

num = 0
for e in ans :
    print num, ": ", dump_tree( e )
    num = num + 1

