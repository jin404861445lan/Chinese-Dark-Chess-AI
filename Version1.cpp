//
//  main.cpp
//  AI
//
//  Created by YingSihao on 15/12/8.
//  Copyright (c) 2015年 YingSihao. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <string>
#include <ctime>
#include <map>

typedef unsigned int uint;

int MAX_DEPTH = 5;
const int INF = 777777777;
//const int MOD = 100000007;

const int WEIGHT[34] = {0, 200, 200, 200, 200, 200, 420, 420, 380, 380, 761, 761, 1523, 1523, 3047, 3047, 6095, 200, 200, 200, 200, 200, 420, 420, 380, 380, 761, 761, 1523, 1523, 3047, 3047, 6095, 0};

int Board[32];
int Position[33];
int current[33];
int current_num[2];
int current_index[33];
int MoveTab[32][5];
int OrderTab[34][34];

bool eaten[33];
int noteaten[2] = {16, 16};

int id;
int now = 0; // now == 0 ? me       now == 1 ? enemy

uint ZobristKey = 0;
uint ZobristPlayer;
uint Zobrist[34][32];
//std::map<uint, int> value;

inline void end() {
    std::cout << "END\n" << std::flush;
}

inline int get_pos(const int& x, const int& y) {
    return (x << 3) + y;
}

inline int flip_chess(int color, int kind) {

    int add = color << 4;
    
    switch (kind) {
        case 0:
            return add + 16;
            break;
            
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        	for (int i = add + 16 - (kind << 1); i <= add + 17 - (kind << 1); ++i)
            	if (Position[i] == 33 && !eaten[i]) return i;
            break;
            
        case 6:
        	for (int i = add + 1; i <= add + 5; ++i)
            	if (Position[i] == 33 && !eaten[i]) return i;
            break;
            
        default:
            break;
    }
    
    return -1;
}

// return the color of the chess (1..32)
inline int get_color(int chess) {
    return chess <= 16 ? 0 : 1;
}

/*void calc_zobrish() {

	ZobristPlayer = rand() % MOD;
	
    for (int i = 1; i <= 33; ++i)
    	for (int j = 0; j <= 31; ++j)
        	Zobrist[i][j] = rand() % MOD;
    for (int j = 0; j <= 31; ++j)
    	ZobristKey ^= Zobrist[33][j];
}*/

void pre_do() {

    memset(Board, 0, sizeof(Board));
    
    // At first all the chesses are unavalible
    for (int i = 1; i <= 32; ++i)
    	Position[i] = 33;
    
    memset(current, 0, sizeof(current));
    memset(current_num, 0, sizeof(current_num));
    
    memset(MoveTab, 0 ,sizeof(MoveTab));
    
    // Calculate MoveTab
    
    for (int i = 0; i < 32; ++i) {
    	if (i - 8 >= 0)
        	MoveTab[i][++MoveTab[i][0]] = i - 8;
        if (i + 8 <= 31)
        	MoveTab[i][++MoveTab[i][0]] = i + 8;
        if (i % 8 != 0)
            MoveTab[i][++MoveTab[i][0]] = i - 1;
        if (i % 8 != 7)
            MoveTab[i][++MoveTab[i][0]] = i + 1;
    }
    
    // Calculate OrderTab
    
    for (int i = 1; i <= 5; ++i) {
        for (int j = 0; j <= 16; ++j)
        	OrderTab[i][j] = -1;
        for (int j = 17; j <= 21; ++j)
        	OrderTab[i][j] = 1;
        for (int j = 22; j <= 31; ++j)
        	OrderTab[i][j] = -1;
        OrderTab[i][32] = 1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 6; i <= 7; ++i) {
        for (int j = 0; j <= 16; ++j)
        	OrderTab[i][j] = -1;
        for (int j = 17; j <= 32; ++j)
        	OrderTab[i][j] = 1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 8; i <= 9; ++i) {
        for (int j = 0; j <= 16; ++j)
            OrderTab[i][j] = -1;
        for (int j = 17; j <= 25; ++j)
            OrderTab[i][j] = 1;
        for (int j = 26; j <= 32; ++j)
        	OrderTab[i][j] = -1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 10; i <= 11; ++i) {
        for (int j = 0; j <= 16; ++j)
            OrderTab[i][j] = -1;
        for (int j = 17; j <= 27; ++j)
            OrderTab[i][j] = 1;
        for (int j = 28; j <= 32; ++j)
            OrderTab[i][j] = -1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 12; i <= 13; ++i) {
        for (int j = 0; j <= 16; ++j)
            OrderTab[i][j] = -1;
        for (int j = 17; j <= 29; ++j)
            OrderTab[i][j] = 1;
        for (int j = 30; j <= 32; ++j)
            OrderTab[i][j] = -1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 14; i <= 15; ++i) {
        for (int j = 0; j <= 16; ++j)
            OrderTab[i][j] = -1;
        for (int j = 17; j <= 31; ++j)
            OrderTab[i][j] = 1;
        for (int j = 32; j <= 32; ++j)
            OrderTab[i][j] = -1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 16; i <= 16; ++i) {
        for (int j = 0; j <= 16; ++j)
            OrderTab[i][j] = -1;
        for (int j = 22; j <= 32; ++j)
            OrderTab[i][j] = 1;
        for (int j = 17; j <= 21; ++j)
            OrderTab[i][j] = -1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 17; i <= 21; ++i) {
        for (int j = 17; j <= 32; ++j)
            OrderTab[i][j] = -1;
        for (int j = 1; j <= 5; ++j)
            OrderTab[i][j] = 1;
        for (int j = 6; j <= 15; ++j)
            OrderTab[i][j] = -1;
        OrderTab[i][0] = -1;
        OrderTab[i][16] = 1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 22; i <= 23; ++i) {
        for (int j = 17; j <= 32; ++j)
            OrderTab[i][j] = -1;
        for (int j = 1; j <= 16; ++j)
            OrderTab[i][j] = 1;
        OrderTab[i][0] = -1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 24; i <= 25; ++i) {
        for (int j = 17; j <= 32; ++j)
            OrderTab[i][j] = -1;
        for (int j = 1; j <= 9; ++j)
            OrderTab[i][j] = 1;
        for (int j = 10; j <= 16; ++j)
            OrderTab[i][j] = -1;
        OrderTab[i][0] = -1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 26; i <= 27; ++i) {
        for (int j = 17; j <= 32; ++j)
            OrderTab[i][j] = -1;
        for (int j = 1; j <= 11; ++j)
            OrderTab[i][j] = 1;
        for (int j = 12; j <= 16; ++j)
            OrderTab[i][j] = -1;
        OrderTab[i][0] = -1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 28; i <= 29; ++i) {
        for (int j = 17; j <= 32; ++j)
            OrderTab[i][j] = -1;
        for (int j = 1; j <= 13; ++j)
            OrderTab[i][j] = 1;
        for (int j = 13; j <= 16; ++j)
            OrderTab[i][j] = -1;
        OrderTab[i][0] = -1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 30; i <= 31; ++i) {
        for (int j = 17; j <= 32; ++j)
            OrderTab[i][j] = -1;
        for (int j = 1; j <= 15; ++j)
            OrderTab[i][j] = 1;
        for (int j = 16; j <= 16; ++j)
            OrderTab[i][j] = -1;
        OrderTab[i][0] = -1;
        OrderTab[i][33] = 0;
    }
    
    for (int i = 32; i <= 32; ++i) {
        for (int j = 17; j <= 32; ++j)
            OrderTab[i][j] = -1;
        for (int j = 6; j <= 16; ++j)
            OrderTab[i][j] = 1;
        for (int j = 1; j <= 5; ++j)
            OrderTab[i][j] = -1;
        OrderTab[i][0] = -1;
        OrderTab[i][33] = 0;
    }
	
}

/**
	walk step
*/

inline void walk(int posx, int posy, int tox, int toy, int color = 0, int kind = 0) {
    
    int pos1 = get_pos(posx, posy), pos2 = get_pos(tox, toy);
    
    // flip
    if (posx == tox && posy == toy) {
    
    	int chess = flip_chess(color, kind);
        
        Board[pos2] = chess;
        
        Position[chess] = pos2;
        
        current[(++current_num[color]) + (color << 4)] = chess;
        current_index[chess] = (color << 4) + current_num[color];
        
        
    } else if (Board[pos2] == 33) {		// move now_chess from pos to to.
        int now_chess = Board[pos1];
        
        Board[pos1] = 33;
        Board[pos2] = now_chess;
        
        Position[now_chess] = pos2;
    } else {							// chess in pos1 eats chess int pos2.
        int pos_chess = Board[pos1];
        int to_chess = Board[pos2];
        int to_color = get_color(to_chess);
        
        Board[pos1] = 33;
        Board[pos2] = pos_chess;
        
        Position[pos_chess] = pos2;
        Position[to_chess] = 33;
        
        current[current_index[to_chess]] = current[(to_color << 4) + current_num[to_color]];
        current_index[current[(to_color << 4) + (current_num[to_color]--)]] = current_index[to_chess];
        
    	eaten[to_chess] = 1;
        --noteaten[to_color];
    }
    
    //ZobristKey ^= ZobristPlayer;
    
}

inline void walk(int pos1, int pos2) {

    if (Board[pos2] == 33) {		// move now_chess from pos to to.
        int now_chess = Board[pos1];
        
        Board[pos1] = 33;
        Board[pos2] = now_chess;
        
        Position[now_chess] = pos2;
    } else {							// chess in pos1 eats chess int pos2.
        int pos_chess = Board[pos1];
        int to_chess = Board[pos2];
        int to_color = get_color(to_chess);
        
        Board[pos1] = 33;
        Board[pos2] = pos_chess;
        
        Position[pos_chess] = pos2;
        Position[to_chess] = 33;
        
        current[current_index[to_chess]] = current[(to_color << 4) + current_num[to_color]];
        current_index[current[(to_color << 4) + (current_num[to_color]--)]] = current_index[to_chess];
        
        eaten[to_chess] = 1;
        --noteaten[to_color];
    }
    
}

inline void unwalk(int pos, int chess, int to_id, int to_pos, int to_chess) {
	
    if (to_chess != 33) {
    
    	Board[pos] = chess;
    	Position[chess] = pos;
    
    	Board[to_pos] = to_chess;
    	Position[to_chess] = to_pos;
    
    	//change current[] of to
    	current[(to_id << 4) + (++current_num[to_id])] = current[current_index[to_chess]];
    	current_index[current[(to_id << 4) + current_num[to_id]]] = (to_id << 4) + current_num[to_id];
    	current[current_index[to_chess]] = to_chess;
    
    	eaten[to_chess] = 0;
    	++noteaten[to_id];
    
    } else {
        
        Board[pos] = chess;
        Position[chess] = pos;
        
        Board[to_pos] = 33;
        
    }
}

int score[2];
int base[33];
int power[33];

inline bool is_pao(int chess) {
    return chess == 6 || chess == 7 || chess == 22 || chess == 23;
}

/*
inline int Evaluate(int id) {

	memset(score, 0, sizeof(score));
    memset(base, 0, sizeof(base));
    memset(power, 0, sizeof(power));
    
    for (int now_id = 0; now_id <= 1; ++now_id) {
        for (int i = 1 + (now_id << 4); i <= 16 + (now_id << 4); ++i) {
        
        	//if (Position[i] == 33) continue;
            
            if (!is_pao(i)) {
                for (int j =  1 + ((now_id ^ 1) << 4); j <= 16 + ((now_id ^ 1) << 4); ++j)
                    if (!eaten[j] && OrderTab[i][j]) base[i] += 4;
            } else {
                base[i] = (noteaten[now_id] << 2) + noteaten[now_id ^ 1];
            }
            
        }
    }
    
    for (int now_id = 0; now_id <= 1; ++now_id) {
        for (int i = 16 + (now_id << 4); i >= 1 + (now_id << 4); --i) {
        
            if (!is_pao(i)) {
                for (int j =  1 + ((now_id ^ 1) << 4); j <= 16 + ((now_id ^ 1) << 4); ++j)
                    if (!eaten[j] && OrderTab[i][j]) power[i] += base[j];
            } else {
                power[i] = power[14 + (now_id << 4)] * 4 / 15;
            }
            
            power[i] *= WEIGHT[i];
            
        }
    }
    
    for (int now_id = 0; now_id <= 1; ++now_id) {
        for (int i = 1 + (now_id << 4); i <= 16 + (now_id << 4); ++i)
            if (!eaten[i]) score[now_id] += power[i];
    }
    
    return score[id] - score[id ^ 1];
}
*/

inline bool can_flip() {
    for (int i = 0; i <= 31; ++i)
        if (Board[i] == 0) return 1;
    return 0;
}

int dec_pos, dec_to;

int AlphaBeta(int id, int depth, int alpha, int beta) {
    int val;
    int to_weight = 0;
    
    if (depth == 0) {
    
        if (0 >= beta)
            return beta;
        
        if (0 > alpha)
            alpha = 0;
        
        return alpha;
    }
    
    if (can_flip()) { //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        
        val = -AlphaBeta(id ^ 1, depth - 1, -beta, -alpha);
        
        if (val >= beta)
            return beta;
        
        if (val + 0 > alpha + to_weight) {
            alpha = val;
            to_weight = 0;
            if (depth == MAX_DEPTH) {
                dec_pos = -1;
            }
        }
        
    }
    
    bool can_move = 0;
    
    for (int i = 1; i <= current_num[id]; ++i) {
    
        int chess = current[(id << 4) + i];
    	int pos = Position[chess];
        
        // search four directions
        for (int j = 1; j <= MoveTab[pos][0]; ++j) {
        
            int to_chess = Board[MoveTab[pos][j]];
            
            if ((OrderTab[chess][to_chess] >= 0 && !is_pao(chess)) || (is_pao(chess) && to_chess == 33)) {
            
            	can_move = 1;
                walk(pos, MoveTab[pos][j]);
                val = -AlphaBeta(id ^ 1, depth - 1, -beta, -alpha);
                //val += WEIGHT[to_chess];
                unwalk(pos, chess, id ^ 1, MoveTab[pos][j], to_chess);
                
                if (val + WEIGHT[to_chess] >= beta)
                	return beta;
                
                if (val + WEIGHT[to_chess] >= alpha + to_weight) {
                	alpha = val;
                	to_weight = WEIGHT[to_chess];
                    if (depth == MAX_DEPTH) {
                    	dec_pos = pos;
                    	dec_to = MoveTab[pos][j];
                    }
                }
            }
        }
        
        // Pao's eat
        if (is_pao(chess)) {
        
        	int to_chess = Board[pos - 16];
            
            if (pos - 16 >= 0 && OrderTab[chess][to_chess] == 1 && Board[pos - 8] != 33) {
            
            	can_move = 1;
            	walk(pos, pos - 16);
                val = -AlphaBeta(id ^ 1, depth - 1, -beta, -alpha);
                //val += WEIGHT[to_chess];
                unwalk(pos, chess, id ^ 1, pos - 16, to_chess);
                
                if (val + WEIGHT[to_chess] >= beta)
                    return beta;
                
                if (val + WEIGHT[to_chess] >= alpha + to_weight) {
                    alpha = val;
                    to_weight = WEIGHT[to_chess];
                    if (depth == MAX_DEPTH) {
                    	dec_pos = pos;
                    	dec_to = pos - 16;
                    }
                }
            }
            
            to_chess = Board[pos + 16];
            
            if (pos + 16 <= 31 && OrderTab[chess][to_chess] == 1 && Board[pos + 8] != 33) {
                
                can_move = 1;
                walk(pos, pos + 16);
                val = -AlphaBeta(id ^ 1, depth - 1, -beta, -alpha);
                //val += WEIGHT[to_chess];
                unwalk(pos, chess, id ^ 1, pos + 16, to_chess);
                
                if (val + WEIGHT[to_chess] >= beta)
                    return beta;
                
                if (val + WEIGHT[to_chess] >= alpha + to_weight) {
                    alpha = val;
                    to_weight = WEIGHT[to_chess];
                    if (depth == MAX_DEPTH) {
                    	dec_pos = pos;
                    	dec_to = pos + 16;
                    }
                }
            }
            
            to_chess = Board[pos - 2];
            
            if (pos % 8 >= 2 && OrderTab[chess][to_chess] == 1 && Board[pos - 1] != 33) {
                
                can_move = 1;
                walk(pos, pos - 2);
                val = -AlphaBeta(id ^ 1, depth - 1, -beta, -alpha);
                //val += WEIGHT[to_chess];
                unwalk(pos, chess, id ^ 1, pos - 2, to_chess);
                
                if (val + WEIGHT[to_chess] >= beta)
                    return beta;
                
                if (val + WEIGHT[to_chess] >= alpha + to_weight) {
                    alpha = val;
                    to_weight = WEIGHT[to_chess];
                    if (depth == MAX_DEPTH) {
                    	dec_pos = pos;
                    	dec_to = pos - 2;
                    }
                }
            }
            
            to_chess = Board[pos + 2];
            
            if (pos % 8 <= 5 && OrderTab[chess][to_chess] == 1 && Board[pos + 1] != 33) {
                
                can_move = 1;
                walk(pos, pos + 2);
                val = -AlphaBeta(id ^ 1, depth - 1, -beta, -alpha);
                //val += WEIGHT[to_chess];
                unwalk(pos, chess, id ^ 1, pos + 2, to_chess);
                
                if (val + WEIGHT[to_chess] >= beta)
                    return beta;
                
                if (val + WEIGHT[to_chess] >= alpha + to_weight) {
                    alpha = val;
                    to_weight = WEIGHT[to_chess];
                    if (depth == MAX_DEPTH) {
                    	dec_pos = pos;
                    	dec_to = pos + 2;
                    }
                }
            }
            
        }
        
    }
    
    if (!can_move && !can_flip()) return alpha;
    
    return alpha = alpha + to_weight;
}

inline void do_flip() {
    do {
        dec_pos = dec_to = rand() % 32;
        //std::cin >> dec_pos >> dec_to;
    } while (Board[dec_pos] != 0);
}

inline void my_treat() {

	dec_pos = -1;
    
    MAX_DEPTH = noteaten[id ^ 1] <= 2 ? 2 : 6;
    
    int walk_val = AlphaBeta(id, MAX_DEPTH, -INF, INF);
    
    //std::cerr << "walk_val : " << walk_val << "   now_val : " << now_val << std::endl;
    
    if ((walk_val < 0 && can_flip()) || dec_pos == -1 || (walk_val == 0 && rand() % 100 <= 3 && can_flip()))
    	do_flip();
    
    int px = dec_pos / 8;
    int py = dec_pos % 8;
    int tx = dec_to / 8;
    int ty = dec_to % 8;
    
    std::cout << px << " " << py << " " << tx << " " << ty << std::endl;
}

/**
void print_MoveTab() {
    
    for (int i = 0; i < 32; ++i) {
        std::cout << i << " : ";
        for (int j = 0; j <= MoveTab[i][0]; ++j)
            std::cout << MoveTab[i][j] << ' ';
        std::cout << std::endl;
    }
}
*/

int main(int argc, char** argv) {

    unsigned seed = time(0);
    if (argc == 2) {
        std::cerr << "Excited! Get given seed = " << argv[1] << std::endl;
        seed = 0;
        for (char *pc = argv[1]; *pc; ++pc)
            seed = seed * 10 + (*pc - '0');
    }
    srand(seed);
    
    pre_do();
    //calc_zobrish();
    //print_MoveTab();
    
    std::string op;
    
    while (1) {

        std::cin >> op;
        
        if (op == "id") {
            std::cin >> id;
            std::cout << "INTERESTING" << std::endl;
            end();
        } else if (op == "message") {
        
            int posx, posy, tox, toy, color, kind;
            std::cin >> posx >> posy >> tox >> toy >> color >> kind;
            
            walk(posx, posy, tox, toy, color, kind);
            
        } else if (op == "action") {
            my_treat();
            end();
        }
        
    }
    
    return 0;
}

//以大换小？？？
//不吃对方东西？？？
