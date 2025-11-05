#include <stdio.h>
#include <stdlib.h>

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int see_more();

int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc == 1) {
        do_more(stdin);
    } else {
        while (--argc) {
            if ((fp = fopen(*++argv, "r")) != NULL) {
                do_more(fp);
                fclose(fp);
            } else {
                exit(1);
            }
        }
    }

    return 0;
}

void do_more(FILE *fp)
/*
 * PAGELEN行の情報を読み、see_more()を呼び出して命令を待つ
 */
{
    char line[LINELEN];
    int num_of_lines = 0;
    int reply;

    see_more();

    while (fgets(line, LINELEN, fp)) { /* 入力がまだある */
        if (num_of_lines == PAGELEN) { /* 画面がいっぱいに? */
            reply = see_more();        /* y: 指示待ち */
            if (reply == 0) {          /* n: 終了 */
                break;
            }
            num_of_lines -= reply;     /* カウンタをリセット */
        }

        if (fputs(line, stdout) == EOF) {
            exit(1);
        }
        num_of_lines++;
    }
}


int see_more()
/*
 * メッセージを出力して応答を待つ。先に進める行数を返す。 
 * qならノー、スペースならイエス、CRなら1行
 */
{
    int c;

    printf("\033[7m more? \033[m");
    while ((c = getchar()) != EOF) {
        if (c == 'q') {
            return 0;
        }
        if (c == ' ') {
            return PAGELEN;
        }
        if (c == '\n') {
            return 1;
        }
    }

    return 0;
}

