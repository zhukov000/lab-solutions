#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <time.h>

#define LENGTH 1000

sigjmp_buf buffer;

int count_int = 10;

int main() {

	void callBack13();

	char data[LENGTH+1];
	int vv;
	int gg = 0;
	int g[2];

	for(int i=0; i<=LENGTH; ++i) {
		data[i] = '\0';
	}

	signal(SIGINT, callBack13);
	sigsetjmp(buffer, 1);
	vv = 1;
	do {
		sigsetjmp(buffer, 1);
		pipe(g);
		if(fork() == 0) { // child
			close(1);
			close(g[0]);
			dup2(g[1], 1);
			close(g[1]);
			execl("/usr/bin/w", "w", "-hs", NULL);
		}
		else { // parent
			wait(NULL);
			
			sigsetjmp(buffer, 1);
			close(g[1]);

			read(g[0], data, LENGTH);
			char username[33];
			for(int i=0; i<33; ++i) {
				username[i] = '\0';
			}
			int j = 0;
			for(int i=0; i <= LENGTH && data[i] != '\0'; ++i) {
				if(data[i] == ' ') {
					if (username[0] != '\0') {
						username[j] = '\0';
						printf("%s\n", username);
						username[0] = '\0';
						sleep(1);
					}
				} else if (j < 32) {
					username[j++] = data[i];
				} else if (data[i] == '\n') {
					j = 0;
				}
			}

			sigsetjmp(buffer, 1);
			vv = 0;
		}
	} while(vv);

	sigsetjmp(buffer, 1);
	printf("Good night, sweet prince!\n");

	return 0;
}

void callBack13() {
	count_int --;
	signal(SIGINT, callBack13);

	if(count_int <= 0) {
		printf("Good bye!\n");
		exit(1);
	}

	time_t rawTime;
	struct tm* timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	printf("Get interrupt at %s", asctime(timeInfo));
	printf("%d times left\n", count_int);

	siglongjmp(buffer, 1);
}
