/**
 * The maximum weighted matching in O(n^3)
 * @author: donggu
 * Reference: http://www.math.uwo.ca/~mdawes/courses/344/kuhn-munkres.pdf
 */

#include <cstdio>
#include <cstdlib>
#define edge(X,Y) (l[(X)]+l[n+(Y)]==w[(X)][(Y)])

const int M = 102;
const int oo = 2147483647;
static int w[M][M];
static int l[M*2];
static bool S[M], T[M];

void update_neighbor(int n, int w[][M], bool neighbor[])
{
	for(int i=1;i<=n;++i) neighbor[i] = false;
	for(int i=1;i<=n;++i)
		if(S[i])
			for(int j=1;j<=n;++j)
				neighbor[j] |= edge(i,j);
}

int hungarian(int n, int w[][M])
{
	int n_matched = 0;

	static int matched[M*2];
	static bool neighbor[M];
	static bool visit[M];
	static int q[M];
	static int path[M];

Init:
	for(int i=1;i<=n;++i)
	{
		matched[i] = 0;
		matched[n+i] = 0;

		// l[X] = max(w(x,y))
		l[i] = w[i][1];
		for(int j=2;j<=n;++j)
			if(w[i][j]>l[i])
				l[i] = w[i][j];

		l[n+i] = 0; // l[Y] = 0
	}

Step1:
	if(n_matched == n) goto End;
	int x;
	for(int i=1;i<=n;++i)
	{
		S[i] = false;
		T[i] = false;
	}
	for(int i=1;i<=n;++i)
		if(!matched[i])
		{
			x = i;
			break;
		}
	S[x] = true;

Step2:
	{
		update_neighbor(n, w, neighbor);

		for(int i=1;i<=n;++i)
			if(neighbor[i] != T[i])
				goto Step3;

		int al = oo;
		for(int i=1;i<=n;i++) if(S[i])
			for(int j=1;j<=n;++j) if(!T[j])
			{
				int v = l[i] + l[n+j] - w[i][j];
				if(al > v) al = v;
			}
		for(int i=1;i<=n;++i)
		{
			if(S[i]) l[i]-=al;
			if(T[i]) l[n+i]+=al;
		}
	}

Step3:
	{
		update_neighbor(n, w, neighbor);

		int y;
		for(int i=1;i<=n;++i)
			if(neighbor[i] && !T[i])
			{
				y=i;
				break;
			}
		if(matched[n+y])
		{
			S[matched[n+y]] = true;
			T[y] = true;
			goto Step2;
		}

		// Find an alternating path from x to y
		int front=0;
		int rear=1;
		q[rear] = x;
		for(int i=1;i<=n;++i) visit[i]=false;
		while(front != rear)
		{
			++front;
			for(int i=1;i<=n;++i) // i in Y
				if(!visit[i] && edge(q[front],i))
				{
					visit[i] = true;
					path[i] = q[front];
					if(matched[n+i])
						q[++rear] = matched[n+i];
					else if(i==y)
					{
						front=rear;
						break;
					}
				}
		}

		int old;
		for(int i=y;i;i=old)
		{
			old = matched[path[i]];
			matched[path[i]] = i;
			matched[n+i] = path[i];
		}
		++n_matched;

		goto Step1;
	}

End:
	int ans = 0;
	for(int i=1;i<=n;++i)
		ans += w[i][matched[i]];

	return ans;
}

int _n;
int _ans;
bool _v[M];
void slow_recursive(int x,int sum)
{
	if(x>_n)
	{
		if(sum>_ans)
			_ans = sum;
	}
	else
	{
		for(int i=1;i<=_n;++i)
			if(!_v[i])
			{
				_v[i]=true;
				slow_recursive(x+1,sum+w[x][i]);
				_v[i]=false;
			}
	}
}
int slow_algorithm()
{
	_ans = -oo;
	for(int i=1;i<=_n;++i) _v[i]=false;
	slow_recursive(1,0);
	return _ans;
}

void test()
{
	int T = 100;
	for(int t=1;t<=T;++t)
	{
		int n = rand()%10+1;
		for(int i=1;i<=n;++i)
			for(int j=1;j<=n;++j)
				w[i][j] = rand()%1000;


		for(int i=1;i<=n;++i,printf("\n"))
			for(int j=1;j<=n;++j)
				printf("%3d ",w[i][j]);

		_n = n;
		int ans1 = hungarian(n, w);
		int ans2 = slow_algorithm();
		if(ans1==ans2)
			printf("%4d correct : %d\n",t,ans1);
		else
		{
			printf("%4d wrong : %d %d\n",t,ans1,ans2);
			break;
		}
	}
	exit(0);
}

int main()
{
	//test();

	int n;
	scanf("%d",&n);
	for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++)
			scanf("%d",&w[i][j]);

	int ans = hungarian(n,w);
	printf("%d\n",ans);

	return 0;
}
