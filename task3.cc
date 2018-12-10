#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<sys/wait.h>
#include<time.h>
#include<iostream>
using namespace std;
//------------------------------------------------------------------------
class FastRandom {
private:
    unsigned long long rnd;
public:
    FastRandom() {
        rnd = 1;
    }
    void seed(unsigned long long seed) {
	rnd = seed;
    }
    unsigned long long rand() {
        rnd ^= rnd << 21;
        rnd ^= rnd >> 35;
        rnd ^= rnd << 4;
        return rnd;
    }
};
//------------------------------------------------------------------------
struct shared{
	bool ready;
	vector<vector<int> > table;
	void print(){
		for(int i = 0; i < table[0].size(); i ++)
			print("______");
		printf("\n");
		for(int i = 0; i < table.size(); i ++){
			for (int j = 0; j < table[i].size(); j++){
				if( i % 2 == 1 && ( j == 0 || j == table[i].size() - 1)){
					if(table[i][j] == 2)
						cout<<"|"<<"_"<<"|";
					if(table[i][j] == 1)
						cout<<"|"<<"+"<<"|";
				}
				else{
					if(table[i][j] == 2)
						cout<<"|"<<"____"<<"|";
					if(table[i][j] == 1)
						cout<<"|"<<"++++"<<"|";
				}
			}
			cout<<"\n";
		}	
	}
};
struct info{
	int id;
	int MIN;
	int MAX;
	FastRandom r;
	shared * wall;
	pthread_mutex_t * mutex;
};
//------------------------------------------------------------------------
void *potok(void * arg){
	info * a = (info *)arg;
	printf("%s", (a->id == 1) ? "Batya v dele!\n" : "Sinok tuta!\n");
	int rrr = a->r.rand() % 1000 + 1000;
	while(!(a->wall->ready))
		for(int i = 0; i < a->wall->table.size(); i++)
			for(int j = 0; j < a->wall->table[i].size(); j++){
				int sleep_time = a->r.rand() % (a->MAX - a->MIN) + a->MIN;
				pthread_mutex_lock(a->mutex);
				if(a->wall->table[i][j] == 0){
					if(i % 2 == 0){
						if(i == 0){
							if(j == 0){
								if(a->id == 1){
									a->wall->table[i][j] = 1;
									pthread_mutex_unlock(a->mutex);
									printf("%d began (%d,%d)\n", a->id, i, j);
									a->wall->print();
									usleep(sleep_time);
									a->wall->table[i][j] = 2;
									printf("%d zakonchil (%d,%d)\n", a->id, i, j);
								} else{
									pthread_mutex_unlock(a->mutex);
									usleep(rrr);
								}	
							}
							else if(a->wall->table[i][j - 1] == 2 && a->id == 1){
								a->wall->table[i][j] = 1;
								pthread_mutex_unlock(a->mutex);
								printf("%d began (%d,%d)\n", a->id, i, j);
								a->wall->print();
								usleep(sleep_time);
								a->wall->table[i][j] = 2;
								printf("%d zakonchil (%d,%d)\n", a->id, i, j);
							} else{
								pthread_mutex_unlock(a->mutex);
								usleep(rrr);
							}
						}
						else{
							if(j == 0){
								if(a->wall->table[i - 1][j] * a->wall->table[i - 1][j + 1] == 4){
									a->wall->table[i][j] = 1;
									pthread_mutex_unlock(a->mutex);
									printf("%d began (%d,%d)\n", a->id, i, j);
									a->wall->print();
									usleep(sleep_time);
									a->wall->table[i][j] = 2;
									printf("%d zakonchil (%d,%d)\n", a->id, i, j);
								} else{
									pthread_mutex_unlock(a->mutex);
									usleep(rrr);
								}
							}
							else if(a->wall->table[i - 1][j] * a->wall->table[i - 1][j + 1] * a->wall->table[i][j - 1] == 8){
								a->wall->table[i][j] = 1;
								pthread_mutex_unlock(a->mutex);
								printf("%d began (%d,%d)\n", a->id, i, j);
								a->wall->print();
								usleep(sleep_time);
								a->wall->table[i][j] = 2;
								printf("%d zakonchil (%d,%d)\n", a->id, i, j);
							} else{
								pthread_mutex_unlock(a->mutex);
								usleep(rrr);
							}
						}
					}
					else{
						if(j == 0){
							if(a->wall->table[i - 1][j] == 2){
								a->wall->table[i][j] = 1;
								pthread_mutex_unlock(a->mutex);
								printf("%d began (%d,%d)\n", a->id, i, j);
								a->wall->print();
								usleep(sleep_time);
								a->wall->table[i][j] = 2;
								printf("%d zakonchil (%d,%d)\n", a->id, i, j);
							} else{
								pthread_mutex_unlock(a->mutex);
								usleep(rrr);
							}
						}
						else if(j == a->wall->table[i].size() - 1){
							if(a->wall->table[i - 1][j - 1] * a->wall->table[i][j - 1] == 4){
								a->wall->table[i][j] = 1;
								pthread_mutex_unlock(a->mutex);
								printf("%d began (%d,%d)\n", a->id, i, j);
								a->wall->print();
								usleep(sleep_time);
								a->wall->table[i][j] = 2;
								printf("%d zakonchil (%d,%d)\n", a->id, i, j);
							} else{
								pthread_mutex_unlock(a->mutex);
								usleep(rrr);
							}
						} else if(a->wall->table[i - 1][j - 1] * a->wall->table[i - 1][j] * a->wall->table[i][j - 1] == 8){
							a->wall->table[i][j] = 1;
							pthread_mutex_unlock(a->mutex);
							printf("%d began (%d,%d)\n", a->id, i, j);
							a->wall->print();
							usleep(sleep_time);
							a->wall->table[i][j] = 2;
							printf("%d zakonchil (%d,%d)\n", a->id, i, j);
						} else{
							pthread_mutex_unlock(a->mutex);
							usleep(rrr);
						}
					}
				}
				else{
					pthread_mutex_unlock(a->mutex);
					usleep(rrr);
				}
				if(a->wall->table[a->wall->table.size() - 1][a->wall->table[a->wall->table.size() - 1].size() - 1] == 2)
					a->wall->ready = true;
			}
	printf("%d: Stena gotova\n", a->id);
	return NULL;
}
//------------------------------------------------------------------------
int main(int argc, char **argv){
	int M,N,PMIN,PMAX,SMIN,SMAX;
	cin >> N >> M >> PMIN >> PMAX >> SMIN >> SMAX;
	shared wall1;
	wall1.ready = false;
	wall1.table.resize(M);
	for(int i = 0; i < M; i ++){
		wall1.table[i].resize((i % 2 == 0) ? N : (N + 1));
		for (int j = 0; j < wall1.table[i].size(); j++){
			wall1.table[i][j] = 0;
		}
	}	
	vector<info>infos;
	infos.resize(2);
	pthread_mutex_t mutex1;
	pthread_mutex_init(&mutex1, NULL);
	vector<pthread_t>threads;
	threads.resize(2);
	for(int i = 0; i < 2; i++){
		infos[i].id = i;
		infos[i].wall = &wall1;
		infos[i].mutex = &mutex1;
		infos[i].MIN = (i == 0) ? SMIN : PMIN;
		infos[i].MAX = (i == 0) ? SMAX : PMAX;
		infos[i].r.seed(time(NULL) + i * 10);
		pthread_create(&threads[i], NULL, potok, &infos[i]);
	}
	for(int i = 0; i < 2; i++){
		pthread_join(threads[i], NULL);
	}
}



























