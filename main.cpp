#include <iostream>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <queue>
#include <cstring>
#include <algorithm>
using namespace std;
const int N = 810;
struct Pos {
	Pos() { line = col = -1; }
	Pos(int l, int c) { line = l, col = c; }
	int line;
	int col;
};
Pos operator+(const Pos p1, const Pos p2) {
	return Pos(p1.line + p2.line, p1.col + p2.col);
}
int operator-(const Pos p1, const Pos p2) {
	return abs(p1.line - p2.line) + abs(p1.col - p2.col);
}
bool operator==(const Pos p1, const Pos p2) {
	return (p1.line == p2.line && p1.col == p2.col);
}
bool operator!=(const Pos p1, const Pos p2) {
	return !(p1 == p2);
}
const Pos dir[4] = { {1,0},{-1,0},{0,1},{0,-1} };
struct Node {
	Node(Pos pos, int tm) { p = pos, t = tm; }
	Pos p;
	int t;
};
char mp[N][N];
int n, m;
Pos boy, girl;
Pos ghost[2];
inline int read() {
	int num = 0;
	char ch = getchar();
	while (!isdigit(ch))ch = getchar();
	while (isdigit(ch)) {
		num *= 10;
		num += ch - '0';
		ch = getchar();
	}
	return num;
}
inline bool availchar(char ch) {
	return (ch == '.') || (ch == 'X') || (ch == 'M') || (ch == 'G') || (ch == 'Z');
}
inline char readchar() {
	char ch = getchar();
	while (!availchar(ch)) ch = getchar();
	return ch;
}
inline void readmap() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			mp[i][j] = readchar();
			if (mp[i][j] == 'M') boy = Pos(i, j);
			else if (mp[i][j] == 'G') girl = Pos(i, j);
			else if (mp[i][j] == 'Z')
				if (ghost[0] == Pos())ghost[0] = Pos(i, j);
				else ghost[1] = Pos(i, j);
			if (mp[i][j] != '.')mp[i][j] = 'X';
		}
	}
}
int reach[2][N][N];
bool vis[N][N];
inline bool availpos(Pos p) {
	return (p.col >= 0 && p.col < m&& p.line >= 0 && p.line < n);
}
inline void init_reach(int idx, Pos pos) {
	memset(vis, 0, sizeof(vis));
	memset(reach[idx], 0x3f, sizeof(reach[idx]));
	queue<Node>q;
	q.push(Node(pos, 0));
	vis[pos.line][pos.col] = true;
	reach[idx][pos.line][pos.col] = 0;
	while (!q.empty()) {
		Node temp = q.front();
		q.pop();
		for (int i = 0; i < 4; i++) {
			Pos tpos = temp.p + dir[i];
			if (availpos(tpos) && mp[tpos.line][tpos.col] == '.' && !vis[tpos.line][tpos.col]) {
				q.push(Node(tpos, temp.t + 1));
				reach[idx][tpos.line][tpos.col] = temp.t + 1;
				if (!idx) reach[0][tpos.line][tpos.col] = (reach[0][tpos.line][tpos.col] + 2) / 3;
				vis[tpos.line][tpos.col] = true;
			}
		}
	}
}
inline void formatmap() {
	init_reach(0, boy);
	init_reach(1, girl);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (mp[i][j] == '.') {
				int dis = min(ghost[0] - Pos(i, j), ghost[1] - Pos(i, j));
				if (((dis + 1) >> 1) <= min(reach[0][i][j], reach[1][i][j]))mp[i][j] = 'X';
			}
		}
	}
}
inline int solve(Pos start, Pos fin) {
	memset(vis, 0, sizeof(vis));
	queue<Node>q;
	q.push(Node(start, 0));
	vis[start.line][start.col] = true;
	while (!q.empty()) {
		Node ntemp = q.front();
		q.pop();
		for (int i = 0; i < 4; i++) {
			Pos tpos = ntemp.p + dir[i];
			if (availpos(tpos) && mp[tpos.line][tpos.col] != 'X' && !vis[tpos.line][tpos.col]) {
				if (tpos - fin == 1)return ntemp.t + 2;
				q.push(Node(tpos, ntemp.t + 1));
				vis[tpos.line][tpos.col] = true;
			}
		}
	}
	return -1;
}
int main() {
	int T;
	cin >> T;
	while (T--) {
		ghost[0] = Pos();
		n = read();
		m = read();
		readmap();
		formatmap();
		register int num = solve(boy, girl);
		if (num != -1) {
			cout << ((num + 3) >> 2) << endl;
		}
		else {
			cout << -1 << endl;
		}
	}
	return 0;
}
