# coding=UTF-8

from grammer import *

# LALR(1)構文解析表を作るモジュール
#
# - LALR(1) 項は (rule_id, pos, token) という3つ組で表す．

class LALR1_table :
    # コンストラクタ
    def __init__(self) :
        self.clear()

    # 内容をクリアする．
    def clear(self) :

        # 状態リスト
        # 状態とは項の集合
        self._StateList = []

        # ACTION マップ
        self._ActionMap = []

    # @brief LALR(1)構文解析表を作る．
    def make_table(self, grammer) :
        self.clear()

        # LR0 カーネルを求める．
        state_list = LR0_kernel_list(grammer)

        # カーネル項の全リストを作る．
        kernel_list = []
        for state in state_list :
            for kernel in state :
                kernel_list.append(kernel)

        # 先読みの生成/伝搬を調べる．
        generated_token = [[] in kernel_list]
        propagated_token = [[] in kernel_list]
        for (rule_id, pos) in kernel_list :
            exp_set = LR1_closure(grammer, (rule_id, pos, -1))
            for (rule_id1, pos1, token1) in exp_set :
                if token1 != -1 :
                    # 生成された先読み
                    add_token1(generated_token, rule_id1, pos1, token1)
                else :
                    # 伝搬された先読み
                    add_token2(propagated_token, rule_id, pos)

        la_token_list = generated_token
        new_token_list = []
        k_id = 0
        for token_list in la_token_list :
            (rule_id, pos) = kernel_list[k_id]
            k_id += 1
            for token in token_list :
                new_token_list.append( (rule_id, pos, token) )

        while len(new_token_list) > 0 :
            cur_token_list = new_token_list
            new_token_list = []
            for (rule_id, pos, token) in cur_token_list :
                pass

        for state in state_list :
            for (rule_id, pos) in state :
                J = LR1_closure(grammer, (rule_id, pos, -1))
                for (rule_id1, pos1, token1) in J :
                    if token1 != -1 :
                        # 生成された先読みトークン
                        la_array[state_id].append(token1)


        self.items(grammer)
        for state in self._StateList :
            state_id = self.state2id(state)
            self._ActionMap.append({})
            for token_id in range(0, len(grammer._TokenList)) :
                if grammer._TerminalList[token_id] :
                    for (rule_id, pos, token) in state :
                        (left, right) = grammer.id2rule(rule_id)
                        if len(right) > pos and right[pos] == token_id :
                            tmp_state = next_state(grammer, state, token_id)
                            next_id = self.state2id(tmp_state)
                            assert next_id != -1
                            if not self.set_action(state_id, token_id, 'shift', next_id) :
                                print "Error: not an LALR(1)"
                else :
                    tmp_state = next_state(grammer, state, token_id)
                    if len(tmp_state) > 0 :
                        next_id = self.state2id(tmp_state)
                        if not self.set_action(state_id, token_id, 'goto', next_id) :
                            print "Error: not an LALR(1)"
            for (rule_id, pos, token) in state :
                (left, right) = grammer.id2rule(rule_id)
                if len(right) == pos :
                    if left == grammer._StartNode :
                        if not self.set_action(state_id, 0, 'accept', 0) :
                            print "Error: not an LALR(1)"
                    else :
                        if not self.set_action(state_id, token, 'reduce', rule_id) :
                            print "Error: not an LALR(1)"

    # @brief ACTION 表の項目をセットする．
    # @param[in] state_id 状態番号
    # @param[in] token_id 記号番号
    # @param[in] action 動作
    # @param[in] action_id 動作に関連した番号
    #
    # 以前の設定内容と矛盾が生じた場合には False を返す．
    def set_action(self, state_id, token_id, action, action_id) :
        if self._ActionMap[state_id].has_key(token_id) :
            if self._ActionMap[state_id][token_id] != (action, action_id) :
                return False
            else :
                return True
        else :
            self._ActionMap[state_id][token_id] = (action, action_id)
            return True

    # @brief LALR(1)状態リストを表示する．
    def print_states(self, grammer) :
        for state in self._StateList :
            state_id = self.state2id(state)
            print 'State#%d:' % state_id
            print ''
            print_terms(grammer, state)
            print ""
            for key in self._ActionMap[state_id].keys() :
                (action, action_id) = self._ActionMap[state_id][key]
                token = grammer.id2token(key)
                print '  %s: %s %d' % (token, action, action_id)
            print ''

    # @brief 状態番号を返す．
    # @param[in] state 状態(項の集合)
    def state2id(self, state) :
        state_id = 0
        for state1 in self._StateList :
            if state1 == state :
                return state_id
            state_id += 1
        else :
            return -1


if __name__ == '__main__' :
    # テストプログラム
    g = Grammer()

    S = g.add_token('S')
    L = g.add_token('L')
    R = g.add_token('R')
    id = g.add_token('id')
    eq = g.add_token('=')
    star = g.add_token('*')

    g.add_rule(S, (L, eq, R))
    g.add_rule(S, (R, ))
    g.add_rule(L, (star, R))
    g.add_rule(L, (id, ))
    g.add_rule(R, (L, ))

    g.set_start(S)

    g.print_rules()

    g.print_tokens()

    """
    lr1_tab = LALR1_table()

    lr1_tab.make_table(g)

    lr1_tab.print_states(g)
    """
