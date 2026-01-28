
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define MAX_CMD_LEN 256
#define MAX_ARGS 32
#define MAX_BG 10

pid_t bg_pids[MAX_BG];
int bg_jobs[MAX_BG];
int bg_count = 0;
int next_job_id = 1;
pid_t last_child_pid = 0;

void parse_command(char *input, char **args, int *background) {
    char *tok;
    int argc = 0;
    *background = 0;

    tok = strtok(input, " \t");
    while (tok != NULL && argc < MAX_ARGS - 1) {
        args[argc++] = tok;
        tok = strtok(NULL, " \t");
    }
    args[argc] = NULL;

    if (argc > 0 && strcmp(args[argc - 1], "&") == 0) {
        *background = 1;
        args[argc - 1] = NULL;
    }
}

void add_bg_process(pid_t pid) {
    if (bg_count < MAX_BG) {
        bg_pids[bg_count] = pid;
        bg_jobs[bg_count] = next_job_id++;
        bg_count++;
        printf("[%d] %d\n", bg_jobs[bg_count - 1], pid);
    } else {
        fprintf(stderr, "Limite de processos em background atingido.\n");
    }
}

void remove_bg_process(int idx) {
    for (int i = idx; i < bg_count - 1; i++) {
        bg_pids[i] = bg_pids[i + 1];
        bg_jobs[i] = bg_jobs[i + 1];
    }
    bg_count--;
}

void clean_finished_processes() {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < bg_count; i++) {
            if (bg_pids[i] == pid) {
                printf("[%d]+ Done\n", bg_jobs[i]);
                remove_bg_process(i);
                break;
            }
        }
    }
}

void print_jobs() {
    if (bg_count == 0) {
        printf("Nenhum processo em background\n");
        return;
    }
    printf("Processos em background:\n");
    for (int i = 0; i < bg_count; i++) {
        printf("[%d] %d Running\n", bg_jobs[i], bg_pids[i]);
    }
}

void wait_all_bg() {
    if (bg_count == 0) {
        printf("Nenhum processo em background\n");
        return;
    }
    printf("Aguardando processos em background...\n");
    while (bg_count > 0) {
        int status;
        pid_t pid = wait(&status);
        if (pid > 0) {
            for (int i = 0; i < bg_count; i++) {
                if (bg_pids[i] == pid) {
                    printf("[%d]+ Done\n", bg_jobs[i]);
                    remove_bg_process(i);
                    break;
                }
            }
        } else if (errno == ECHILD) break;
    }
    printf("Todos os processos terminaram\n");
}

int is_internal_command(char **args) {
    if (args[0] == NULL) return 0;
    return (!strcmp(args[0], "exit") ||
            !strcmp(args[0], "pid") ||
            !strcmp(args[0], "jobs") ||
            !strcmp(args[0], "wait"));
}

void handle_internal_command(char **args, int *exit_controller) {
    if (!strcmp(args[0], "exit")) {
        if (bg_count > 0) wait_all_bg();
        *exit_controller = 0;
    }

    if (!strcmp(args[0], "pid")) {
        printf("PID do shell: %d\n", getpid());
        if (last_child_pid)
            printf("Último filho: %d\n", last_child_pid);
        else
            printf("Nenhum processo filho criado ainda.\n");
        return;
    }

    if (!strcmp(args[0], "jobs")) {
        print_jobs();
        return;
    }

    if (!strcmp(args[0], "wait")) {
        wait_all_bg();
        return;
    }
}

void execute_command(char **args, int background) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        execvp(args[0], args);
        perror("Erro no execvp");
        exit(1);
    } else {
        last_child_pid = pid;
        if (background)
            add_bg_process(pid);
        else
            waitpid(pid, NULL, 0);
    }
}

int main() {
    char input[MAX_CMD_LEN];
    char *args[MAX_ARGS];
    int background;
    int exit_controller = 1;

    printf("Mini-Shell iniciado (PID: %d)\n", getpid());
    printf("Digite 'exit' para sair\n\n");

    while (exit_controller) {
        clean_finished_processes();

        printf("minishell> ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) continue;

        parse_command(input, args, &background);

        if (is_internal_command(args)) {
            handle_internal_command(args, &exit_controller);
        }
        else{
            execute_command(args, background);
        }
    }

    if (bg_count > 0)
        wait_all_bg();

    printf("Shell encerrado!\n");
    return 0;
}