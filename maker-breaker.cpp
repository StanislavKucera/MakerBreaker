#include <bits/stdc++.h>
using namespace std;
typedef vector<pair<int, int> > edges;
typedef vector<int> degrees;

bool strat = false;
bool crit = false;

inline int simplify(edges &es, degrees &ds) {
	vector<int> index, loops;
	int be = 0, en = 0;
	for (size_t i=0; i<ds.size(); ++i) if (ds[i] == -1) ++be;
	while (true) {
		index.clear(); loops.clear();
		sort(es.begin(), es.end());
		if (es.size() < 3) break;
		for (size_t e=2; e<es.size(); ++e) if (es[e] == es[e-1] && es[e] == es[e-2] && (e == es.size()-1 || es[e] != es[e+1])) index.push_back(e);
		if (index.empty()) break;
		reverse(index.begin(), index.end());
		for (size_t i=0; i<index.size(); ++i) {
			const int v1 = es[index[i]].first, v2 = es[index[i]].second;
			if (v1 == v2) continue;
			ds[v1] += ds[v2]; ds[v2] = -1;
			for (size_t e=0; e<es.size(); ++e) {
				if (es[e].first == es[e].second) continue;
				if (es[e].first == v2) es[e].first = v1;
				if (es[e].second == v2) es[e].second = v1;
				if (es[e].first > es[e].second) swap(es[e].first, es[e].second);
				if (es[e].first == es[e].second) { ds[es[e].first] -= 2; loops.push_back(e); }
			}
		}
		sort(loops.begin(), loops.end(), greater<int>());
		for (size_t i=0; i<loops.size(); ++i) es[loops[i]] = es[es.size()-1-i];
		es.resize(es.size()-loops.size());
	}
	for (size_t i=0; i<ds.size(); ++i) if (ds[i] == -1) ++en;
	return en-be;
}

inline bool connected(const edges &es, const degrees &ds) {
	for (size_t i=0; i<ds.size(); ++i) if (ds[i] == 0) return false;
	vector<bool> vis(ds.size(), false);
	vis[es[0].first] = vis[es[0].second] = true;
	bool change = true;
	while (change) {
		change = false;
		for (size_t e=0; e<es.size(); ++e)
			if (vis[es[e].first] != vis[es[e].second])
				vis[es[e].first] = vis[es[e].second] = change = true;
	}
	for (size_t i=0; i<ds.size(); ++i) if (!vis[i] && ds[i] != -1) return false;
	return true;
}

bool maker1(const int, edges &, degrees &);
bool maker2(const int, edges &, degrees &);

bool breaker1(const int n, edges &es, degrees &ds) {	// expects sorted edges with a<b
	if (es.size() <= 1) return true;
	for (size_t i=0; i<ds.size(); ++i) if (ds[i] >= 0 && ds[i] <= 1) { if (strat) cout << "Deg" << i << "=" << ds[i]<< endl; return true; }
	for (size_t e=0; e<es.size(); ++e) {
		if (ds[es[e].first] <= 2 && ds[es[e].second] <= 2) {
			if (strat) cout << "Cut(22)" << es[e].first << "," << es[e].second << endl;
			return true;
		} else if (ds[es[e].first] <= 3 && ds[es[e].second] <= 3 && (e == 0 || es[e] != es[e-1]) && (e == es.size()-1 || es[e] != es[e+1])) {
			if (strat) cout << "Cut(23)" << es[e].first << "," << es[e].second << endl;
			return true;
		}
	}
	bool loss;
	for (size_t e1=0; e1<es.size(); ++e1) {
		if (e1 > 0 && es[e1] == es[e1-1]) continue;
		auto del = es[e1];
		--ds[del.first]; --ds[del.second];
		es[e1] = es[es.size()-1];
		es.resize(es.size()-1);

		if (strat) cout << "B" << n << "," << es.size()+1 << "c" << del.first << "-" << del.second << endl;
		loss = connected(es, ds) && maker1(n, es, ds);

		es.push_back(del);
		swap(es[e1], es[es.size()-1]);
		++ds[del.first]; ++ds[del.second];

		if (!loss) return true;
	}
	return false;
}

bool breaker2(const int n, edges &es, degrees &ds) {	// expects sorted edges with a<b
	if (es.size() <= 2) return true;
	for (size_t i=0; i<ds.size(); ++i) if (ds[i] >= 0 && ds[i] <= 2) { if (strat) cout << "Deg" << i << endl; return true; }
	for (size_t e=0; e<es.size(); ++e) {
		if (ds[es[e].first] <= 2 && ds[es[e].second] <= 2) {
			if (strat) cout << "Cut(22)" << es[e].first << "," << es[e].second << endl;
			return true;
		} else if (ds[es[e].first] <= 3 && ds[es[e].second] <= 3 && (e == 0 || es[e] != es[e-1]) && (e == es.size()-1 || es[e] != es[e+1])) {
			if (strat) cout << "Cut(23)" << es[e].first << "," << es[e].second << endl;
			return true;
		}
	}
	bool loss;
	for (size_t e1=0; e1<es.size()-1; ++e1) {
		if (e1 > 0 && es[e1] == es[e1-1]) continue;
		for (size_t e2=e1+1; e2<es.size(); ++e2) {
			if (e2 < es.size()-1 && es[e2] == es[e2+1]) continue;
			auto del1 = es[e1], del2 = es[e2];
			--ds[del1.first]; --ds[del1.second]; --ds[del2.first]; --ds[del2.second];
			es[e2] = es[es.size()-1]; es[e1] = es[es.size()-2];
			es.resize(es.size()-2);
			
			if (strat) cout << "B" << n << "," << es.size()+2 << "c" << del1.first << "-" << del1.second << "," << del2.first << "-" << del2.second << endl;
			loss = connected(es, ds) && maker2(n, es, ds);

			es.push_back(del1); es.push_back(del2);
			swap(es[e1], es[es.size()-2]); swap(es[e2], es[es.size()-1]);
			++ds[del1.first]; ++ds[del1.second]; ++ds[del2.first]; ++ds[del2.second];

			if (!loss) return true;
		}
	}
	return false;
}

bool maker1(const int n, edges &es, degrees &ds) {
	if (n <= 2) return true;
	size_t lb1 = 0, ub1 = es.size(); bool set1 = false;
	for (size_t e=0; e<es.size(); ++e) {
		if (ds[es[e].first] <= 2 && ds[es[e].second] <= 2) return false;
		if (ds[es[e].first] <= 1 || ds[es[e].second] <= 1) {
			if (set1) return false;
			else { lb1 = e; ub1 = e+1; set1 = true; }
		}
	}
	for (size_t e1=lb1; e1<ub1; ++e1) {
		if (e1 > 0 && es[e1] == es[e1-1]) continue;
		edges es2 = es;
		degrees ds2 = ds;
		auto con = es2[e1];
		vector<int> loops;

		ds2[con.first] += ds2[con.second]; ds2[con.second] = -1;
		for (size_t e=0; e<es2.size(); ++e) {
			if (es2[e].first == es2[e].second) continue;
			if (es2[e].first == con.second) es2[e].first = con.first;
			if (es2[e].second == con.second) es2[e].second = con.first;
			if (es2[e].first > es2[e].second) swap(es2[e].first, es2[e].second);
			if (es2[e].first == es2[e].second) { ds2[es2[e].first] -= 2; loops.push_back(e); }
		}
		reverse(loops.begin(), loops.end());
		for (size_t i=0; i<loops.size(); ++i) es2[loops[i]] = es2[es2.size()-1-i];
		es2.resize(es2.size()-loops.size());
		int n2 = n - 1 - simplify(es2, ds2);

		if (strat) cout << "M" << n << "," << es.size() << "c" << con.first << "-" << con.second << endl;	
		if (n2 == 1 || !(breaker2(n2, es2, ds2) || breaker1(n2, es2, ds2))) return true;
	}
	return false;
}

bool maker2(const int n, edges &es, degrees &ds) {
	if (n <= 3) return true;
	size_t lb1 = 0, ub1 = es.size()-1, lb2 = 0, ub2 = es.size(); bool set1 = false, set2 = false;
	for (size_t e=0; e<es.size(); ++e) {
		if (ds[es[e].first] <= 1 || ds[es[e].second] <= 1) {
			if (set2) return false;
			else if (set1) { lb2 = e; ub2 = e+1; set2 = true; }
			else { lb1 = e; ub1 = e+1; set1 = true; }
		}
	}
	for (size_t e1=lb1; e1<ub1; ++e1) {
		if (e1 > 0 && es[e1] == es[e1-1]) continue;
		for (size_t e2=(set1 ? lb2 : e1+1); e2<ub2; ++e2) {
			if (!set2 && e2 < ub2-1 && es[e2] == es[e2+1]) continue;
			if (es[e1] == es[e2]) continue;
			edges es2 = es;
			degrees ds2 = ds;
			auto con1 = es2[e1], con2 = es2[e2];
			vector<int> loops;

			if (con1.first == con2.first || con1.first == con2.second || con1.second == con2.first || con1.second == con2.second) {
				int mi = min(min(con1.first, con1.second), min(con2.first, con2.second));
				int ma = max(max(con1.first, con1.second), max(con2.first, con2.second));
				int md = con1.first;
				if (con1.second > mi && con1.second < ma) md = con1.second;
				if (con2.first > mi && con2.first < ma) md = con2.first;
				if (con2.second > mi && con2.second < ma) md = con2.second;

				ds2[mi] += ds2[md] + ds2[ma]; ds2[md] = ds2[ma] = -1;
				for (size_t e=0; e<es2.size(); ++e) {
					if (es2[e].first == es2[e].second) continue;
					if (es2[e].first == md || es2[e].first == ma) es2[e].first = mi;
					if (es2[e].second == md || es2[e].second == ma) es2[e].second = mi;
					if (es2[e].first > es2[e].second) swap(es2[e].first, es2[e].second);
					if (es2[e].first == es2[e].second) { ds2[es2[e].first] -= 2; loops.push_back(e); }
				}
			} else {
				ds2[con1.first] += ds2[con1.second]; ds2[con2.first] += ds2[con2.second]; ds2[con1.second] = ds2[con2.second] = -1;
				for (size_t e=0; e<es2.size(); ++e) {
					if (es2[e].first == es2[e].second) continue;
					if (es2[e].first == con1.second) es2[e].first = con1.first;
					if (es2[e].first == con2.second) es2[e].first = con2.first;
					if (es2[e].second == con1.second) es2[e].second = con1.first;
					if (es2[e].second == con2.second) es2[e].second = con2.first;
					if (es2[e].first > es2[e].second) swap(es2[e].first, es2[e].second);
					if (es2[e].first == es2[e].second) { ds2[es2[e].first] -= 2; loops.push_back(e); }
				}
			}

			reverse(loops.begin(), loops.end());
			for (size_t i=0; i<loops.size(); ++i) es2[loops[i]] = es2[es2.size()-1-i];
			es2.resize(es2.size()-loops.size());
			int n2 = n - 2 - simplify(es2, ds2);

			if (strat) cout << "M" << n << "," << es.size() << "c" << con1.first << "-" << con1.second << "," << con2.first << "-" << con2.second << endl;
			if (n2 == 1 || !(breaker2(n2, es2, ds2) || breaker1(n2, es2, ds2))) return true;
		}
	}
	return false;
}

void breakermaker(istream& in, bool t = false, bool e = false, bool f = false) {
	int n, m, a, b;
	in >> n >> m; if (n == 0 || m == 0 || !in.good()) return;
	edges es(m);
	degrees ds(n, 0);
	for (int e=0; e<m; ++e) {
		in >> a >> b;
		if (a > b) swap(a, b);
		es[e] = make_pair(a, b);
		++ds[a]; ++ds[b];
	}
	n -= simplify(es, ds);
	const bool br = n!=1 && (breaker2(n, es, ds) || breaker1(n, es, ds));
	m = es.size();
	if (t) {
		string ans, res = br ? "B" : "M";
		in >> ans;
		if (ans[0] != res[0]) {
			cout << "Difference " << ans << "->" << res << " on:" << endl;
			cout << n << " " << m << endl;
			for (int e=0; e<m; ++e) cout << es[e].first << " " << es[e].second << "  ";
			cout << endl;
		}
	} else if (e) {
		cout << n << " " << m << endl;
		for (int e=0; e<m; ++e) cout << es[e].first << " " << es[e].second << "  ";
		cout << endl << (br ? "B" : "M") << endl;
	} else if (f) {
		if (!br) {
			cout << n << " " << m << endl;
			for (int e=0; e<m; ++e) cout << es[e].first << " " << es[e].second << "  ";
			cout << endl << "M" << endl;
		}
	} else cout << (br ? "B" : "M") << endl;
}

void test(string name) {
	ifstream in = ifstream(name, ios_base::in);
	while (in.good()) breakermaker(in, true);
	cout << "Life is good!" << endl;
}

void eval(string name) {
	ifstream in = ifstream(name, ios_base::in);
	while (in.good()) breakermaker(in, false, true);
}

void find(string name) {
	ifstream in = ifstream(name, ios_base::in);
	while (in.good()) breakermaker(in, false, false, true);
}

int main(int argc, char* argv[]) {
	bool term = false;
	for (int i=1; i<argc; ++i) {
		string arg = argv[i];
		if (arg == "-s") strat = true;
		else if (arg == "-c") crit = true;
		else if (arg == "-t") { term = true; test(argv[++i]); }
		else if (arg == "-e") { term = true; eval(argv[++i]); }
		else if (arg == "-f") { term = true; find(argv[++i]); }
		//else if (arg == "-g") { term = true; gene(argv[++i]); }
		else cout << "-s (strategy), -c (critical), -t [file] (test), -e [file] (evaluate), -f [file] (find M-winning graphs), -g [file] (generate)" << endl;
	}
	if (!term) breakermaker(cin);
	return 0;
}
