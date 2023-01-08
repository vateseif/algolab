///3
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef std::vector<int> VB;
typedef long IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Stable {
long f, c, k, l, m;
Stable(){};
// This constructor is convenient for a concise initialisation. It can also
// be omitted and the member variables be set manually.
Stable(long ff, long cc, long kk, long ll, long mm) {
  f = ff; c = cc; k = kk; l = ll; m = mm;
}
};

int n;
std::vector<Stable> stables;
std::vector<long> A, P;
 
//for each a1...a24 store the biggest j s.t. ai,pj did solve the prob
std::vector<int> memo1, memo2; 


bool solve(long a, long p){
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 0, true, 1);
  const int h1 = 0; 
  const int h2 = 1;
  const int h3 = 2;

  for (int k=0; k<n; k++){
    lp.set_a(h1, k,  -stables[k].k - a*a);
    lp.set_a(h2, k,  -stables[k].l - p*p);
    lp.set_a(h3, k,  -stables[k].m - a*p);
    lp.set_b(k, stables[k].c-stables[k].f);
  }

  // solve the program, using ET as the exact type
  Solution sol = CGAL::solve_linear_program(lp, ET());
  return !sol.is_infeasible();
}

std::string testcase(){
  std::cin >> n;

  stables.clear(); stables.resize(n);
  for(int i=0; i<n; i++){
    long f, c, k, l, m;
    std::cin >> f >> c >> k >> l >> m;
    stables[i] = Stable(f, c, k, l, m);
  } 

  A.clear(); A.resize(25);
  A[0]=0;
  for (int i=1; i<25; i++){
    long ai; std::cin >> ai;
    A[i] = ai+A[i-1];
  }

  P.clear(); P.resize(25);
  P[0]=0;
  for (int i=1; i<25; i++){
    long pi; std::cin >> pi;
    P[i] = pi+P[i-1];
  }

  if (solve(0, 0)) return "0";

  int l=1;
  int r=49;
  memo1.clear(); memo1.resize(25, -1);
  memo2.clear(); memo2.resize(25, -1);
  while (l<r){
    int mid = (l+r)/2;
    bool foundSolution = false;
    
    int s = std::max(0, mid-24);
    int e = std::min(24, mid);
    for(int i=s; i<=e; i++){
      int j = s-i+e;
      long a = A[i];
      long p = P[j];
      if (j>memo1[i] && i>memo2[j]){
        if (solve(a, p)){
          foundSolution = true;
          break;
        }
        memo1[i] = std::max(memo1[i], j);
        memo2[j] = std::max(memo2[j], i);
      }
    }
    if (foundSolution){
      r = mid;
    }else{
      l = mid+1;
    }
  }

  return l==49? "Impossible!" : std::to_string(l);
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while(T--) std::cout << testcase() << std::endl;
  return 0;
}