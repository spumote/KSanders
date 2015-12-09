#include <iostream>
#include <cassert>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <map>
#include <set>
#include <vector>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<vi> vii;

#define forn(i, n) for (int i = 0; i < (int)n; i++)
#define fore(i, b, e) for (int i = (int)b; i <= (int)e; i++)
#define f first
#define s second
#define next qwertyusdfgh
#define read(x) scanf("%d", &x)
#define write(x) printf("%d ", x)
#define writeln(x) printf("%d\n", x)
#define pb push_back
#define mp make_pair

class SuffixArray {
public:
	vi suffixArray;
	vi lcp;

	SuffixArray() {}
	~SuffixArray() {}
	SuffixArray(vi str, int alphabetSize);
	void countLcp(vi str);
};

bool operator<(const vi& a, const vi& b) {
	if (a.size() < b.size())
		return true;
	if (a.size() > b.size())
		return false;
	for (int i = 0; i < (int)a.size(); i++)
		if (a[i] < b[i])
			return true;
		else
			if (a[i] > b[i])
				return false;
	return false;
}

ostream& operator<<(ostream& os, const vi& a) {
	for (int i = 0; i < (int)a.size(); i++)
		os << a[i] << ' ';
	return os;		
}

ostream& operator<<(ostream& os, const vii& a) {
	for (int i = 0; i < (int)a.size(); i++) {
		for (int j = 0; j < (int)a[i].size(); j++)
			os << a[i][j] << ' ';
		os << endl;
	}
	return os;		
}

vi make_vector(int a, int b, int c) {
	vi ans(3);
	ans[0] = a;
	ans[1] = b;
	ans[2] = c;
	return ans;
}

vi get_3char(const vi& str, int i) {
	vi ans(4);
	for (int j = i; j < i + 3; j++)
		if (j >= (int)str.size())
			ans[j - i] = -1;
		else
			ans[j - i] = str[j];
	ans[3] = i;
	return ans;	
}

vii stable_sort_k(vii& a, int k, int alphabetSize) {
	int n = a.size();
	vector<int> cnt(alphabetSize);
	for (int i = 0; i < n; i++) {
		assert(a[i][k] + 1 < alphabetSize);
		cnt[a[i][k] + 1]++;
	}
	vector<int> pos(alphabetSize);
	for (int i = 1; i < alphabetSize; i++) 
		pos[i] = pos[i - 1] + cnt[i - 1];

	vii ans(n);
	for (int i = 0; i < n; i++) {
		ans[pos[a[i][k] + 1]].resize(a[i].size());
		for (int j = 0; j < (int)a[i].size(); j++)
			ans[pos[a[i][k] + 1]][j] = a[i][j];
		pos[a[i][k] + 1]++;
	}

	return ans;
}

bool cmp(const vi& str, int a, int b, const vi& position) {
	assert(b < (int)str.size() - 2);
	if (a % 3 == 1)
		return (mp(str[a], position[a + 1]) < mp(str[b], position[b + 1]));
	else 
		if (a + 1 >= (int)str.size())
			return (mp(mp(str[a], -1), -1) < mp(mp(str[b], str[b + 1]), position[b + 2]));
		else
		    if (a + 2 >= (int)str.size())
		    	return (mp(mp(str[a], str[a + 1]), -1) < mp(mp(str[b], str[b + 1]), position[b + 2]));
		    else
				return (mp(mp(str[a], str[a + 1]), position[a + 2]) < mp(mp(str[b], str[b + 1]), position[b + 2]));
}

SuffixArray::SuffixArray(vector<int> str, int alphabetSize) {
	//str < 5
	if (str.size() < 5) {
		vii tmp;
		for (int i = 0; i < (int)str.size(); i++) {
			vi cur;
			for (int j = i; j < (int)str.size(); j++)
				cur.pb(str[j]);
			while (cur.size() < str.size())
				cur.pb(-1);
			cur.pb(i);
			tmp.pb(cur);
		}
		sort(tmp.begin(), tmp.end());
		suffixArray.resize(0);
		for (int i = 0; i < (int)tmp.size(); i++)
			suffixArray.pb(tmp[i].back());
		
		return;
	}

	//$ in the end
	int n = str.size();
	while (str.size() % 3 != 0)
		str.pb(-1);

	//new alphabet
	vii tmp;
	for (int i = 0; i < (int)str.size(); i++)
		if (i % 3 != 0)
			tmp.pb(get_3char(str, i));	
	for (int i = 2; i >= 0; i--) {
		tmp = stable_sort_k(tmp, i, alphabetSize + 1);
	}
    

	vector<int> newAlphabet(str.size());
	int j = 0;
	for (int i = 0; i < (int)tmp.size(); i++) {
		if (i > 0 && (tmp[i][0] != tmp[i - 1][0] || tmp[i][1] != tmp[i - 1][1] || tmp[i][2] != tmp[i - 1][2]))
			j++;
		newAlphabet[tmp[i][3]] = j;
	}

	vector<int> newStr;
	for (int i = 1; i < (int)str.size(); i += 3)
		newStr.pb(newAlphabet[i]);
	for (int i = 2; i < (int)str.size(); i += 3)
		newStr.pb(newAlphabet[i]);
	SuffixArray rec(newStr, newStr.size());

	vector<int> firstSuffixArray;
	for (int i = 0; i < (int)rec.suffixArray.size(); i++)
		if (rec.suffixArray[i] < (int)str.size() / 3)
			firstSuffixArray.pb(rec.suffixArray[i] * 3 + 1);
		else
			firstSuffixArray.pb((rec.suffixArray[i] - (int)str.size() / 3) * 3 + 2);

	//2
	tmp.clear();
	for (int i = 0; i < (int)firstSuffixArray.size(); i++)
		if (firstSuffixArray[i] % 3 == 1)
			tmp.pb(make_vector(str[firstSuffixArray[i] - 1], newAlphabet[firstSuffixArray[i]], firstSuffixArray[i] - 1));
	tmp = stable_sort_k(tmp, 0, alphabetSize + 1);
	vector<int> secondSuffixArray;
	for (int i = 0; i < (int)tmp.size(); i++)
		secondSuffixArray.pb(tmp[i][2]);

	//3
	vi position(str.size());
	for (int i = 0; i < (int)firstSuffixArray.size(); i++)
		position[firstSuffixArray[i]] = i;	

	vi ans;
	int i = 0;
	j = 0;
	while (i < (int)firstSuffixArray.size() && j < (int)secondSuffixArray.size()) {
		if (cmp(str, firstSuffixArray[i], secondSuffixArray[j], position)) {
			ans.pb(firstSuffixArray[i]);
			i++;		
		}
		else {
			ans.pb(secondSuffixArray[j]);
			j++;
		}
	}
	while (i < (int)firstSuffixArray.size())
		ans.pb(firstSuffixArray[i++]);
	while (j < (int)secondSuffixArray.size())
		ans.pb(secondSuffixArray[j++]);

	suffixArray.resize(0);
	for (int i = (int)ans.size() - n; i < (int)ans.size(); i++)
		suffixArray.pb(ans[i]);
}

void SuffixArray::countLcp(vi str) {
	str.pb(-1);
	vi position(suffixArray.size());
	lcp.resize(suffixArray.size());
	for (int i = 0; i < (int)suffixArray.size(); i++)
		position[suffixArray[i]] = i;

	int x = 0;
	for (int i = 0; i < (int)suffixArray.size(); i++) {
		x = max(x - 1, 0);
		int a = position[i];
		if (a == (int)suffixArray.size() - 1) {
			x = 0;
			continue;
		}
		int b = suffixArray[a + 1];
		while (str[i + x] == str[b + x])
			x++;
		lcp[a] = x; 
	}	 
}

int main() {
	int k;
	string s;
	cin >> k >> s;
	int n = s.size();
	s += s;	
	vi a(s.size());
	for (int i = 0; i < (int)a.size(); i++)
		a[i] = s[i] - 'a';

	SuffixArray S(a, 26);
	S.countLcp(a);

	for (int i = 0; i < n; i++) {
		int l = i;
		int r = (i + k - 1);
		
		int x = 0;
		int cur = 0;
		for (int j = 0; j < (int)S.suffixArray.size(); j++) {
			if (S.suffixArray[j] >= l && S.suffixArray[j] <= r) {
				if (x > r - S.suffixArray[j] + 1)
					cur += r - S.suffixArray[j] + 1;
				else {
					cur += x;
					x = min(r - S.suffixArray[j] + 1, S.lcp[j]);
				}
		   	}
		   	x = min(x, S.lcp[j]);
		}
		cout << k * (k + 1) / 2 - cur << ' ';	
	}                                        

	return 0;
}