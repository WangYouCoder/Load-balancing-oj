#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        // fork失败
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        printf("子进程: 正在替换为195575925_1.exe\n");

        // 使用execl替换当前进程
        execl("./195575925_1.exe", "195575925_1.exe", (char *)NULL);

        // 如果execl返回，说明出错了
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // 父进程
        printf("父进程: 等待子进程结束\n");
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("子进程正常结束，退出状态: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("子进程被信号终止，信号: %d\n", WTERMSIG(status));
        }
        printf("父进程: 子进程已结束\n");
    }

    return 0;
}

