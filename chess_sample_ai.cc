#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

const int H = 4;
const int W = 8;
const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};

int rounds = 0;
int id;
int map[H][W];

bool exist(int x, int y) {
    return 0 <= x && x < H && 0 <= y && y < W;
}

void change() {
    int x, y, xx, yy, col, kind;
    cin >> x >> y >> xx >> yy >> col >> kind;
    int tar = col * 7 + kind;
    if (x == xx && y == yy) map[x][y] = tar;
    else {
        map[x][y] = -2;
        map[xx][yy] = tar;
    }
}

void  make_decision(int &x, int &y, int &xx, int &yy) {
    while (true) {
        x = rand() % H;
        y = rand() % W;
        if (map[x][y] == -2) continue;
        else if (map[x][y] == -1) {
            xx = x, yy = y;
            return;
        }
        else if (map[x][y] / 7 == id) {
            int tar = map[x][y] % 7;
            int d = rand() % 4;
            xx = x + dx[d];
            yy = y + dy[d];
            if (exist(xx, yy)) {
                if (map[xx][yy] == -2) {
                    return;
                } else if (tar != 5) {
                    if (map[xx][yy] < 0 || map[xx][yy] / 7 == id) continue;
                    int opp = map[xx][yy] % 7;
                    if (tar != 6) {
                        if (tar == 0){
                            if (opp != 6) return;
                        } else if (tar <= opp) return;
                    } else {
                        if (opp == 0) return;
                    }
                } else if (tar == 5) {
                    xx += dx[d];
                    yy += dy[d];
                    if (exist(xx, yy) && map[xx][yy] >= 0 && map[xx][yy] / 7 != id) {
                        return;
                    }
                }
            }
        }
    }
}

inline void end() {
    std::cout << "END\n" << std::flush;
}

int main(int argc, char** argv) {
    unsigned seed = time(0);
    if (argc == 2) {
        cerr << "Excited! Get given seed = " << argv[1] << endl;
        seed = 0;
        for (char *pc = argv[1]; *pc; ++pc)
            seed = seed * 10 + (*pc - '0');
    }
    srand(seed);

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            map[i][j] = -1;
        }
    }
    string op;
    while (true) {
        cin >> op;
        if (op == "id") {
            cin >> id;
            cout << "Sample AI" << endl;
            end();
        } else if (op == "message") {
            change();
        } else if (op == "action") {
            int x, y, xx, yy;
            make_decision(x, y, xx, yy);
            cout << x << " " << y << " " << xx << " " << yy << endl;
            end();
        }
    }
}
