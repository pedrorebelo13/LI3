#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <glib.h>



static ssize_t getline_trim_crlf(char **lineptr, size_t *n, FILE *stream) {
    ssize_t r = getline(lineptr, n, stream);
    if (r > 0) {
        while (r > 0 && ((*lineptr)[r-1] == '\n' || (*lineptr)[r-1] == '\r')) {
            (*lineptr)[--r] = '\0';
        }
    }
    return r;
}

typedef struct {
    long line;
    char *expected;
    char *produced;
} DiffEntry;

static void free_diff_entry(gpointer data) {
    DiffEntry *d = (DiffEntry *)data;
    if (!d) return;
    g_free(d->expected);
    g_free(d->produced);
    g_free(d);
}

static int compare_files(const char *file_a, const char *file_b, GSList **diffs_out) {
    FILE *fa = fopen(file_a, "r");
    FILE *fb = fopen(file_b, "r");
    if (!fa || !fb) {
        if (fa) fclose(fa);
        if (fb) fclose(fb);
        return -1; //erro
    }
    *diffs_out = NULL;
    char *la = NULL, *lb = NULL; size_t na = 0, nb = 0; ssize_t ra, rb; long line = 0;
    int different = 0;
    for (;;) {
        ra = getline(&la, &na, fa);
        rb = getline(&lb, &nb, fb);
        if (ra == -1 && rb == -1) break; // both end
        line++;

        if (ra > 0) { while (ra > 0 && (la[ra-1] == '\n' || la[ra-1] == '\r')) la[--ra] = '\0'; }
        if (rb > 0) { while (rb > 0 && (lb[rb-1] == '\n' || lb[rb-1] == '\r')) lb[--rb] = '\0'; }
        if (ra == -1 || rb == -1 || strcmp(la, lb) != 0) {
            different = 1;
            DiffEntry *d = g_new0(DiffEntry, 1);
            d->line = line;
            d->expected = (ra == -1) ? g_strdup("(fim do ficheiro)") : g_strdup(la);
            d->produced = (rb == -1) ? g_strdup("(fim do ficheiro)") : g_strdup(lb);
            *diffs_out = g_slist_append(*diffs_out, d);
        }
    }
    free(la); free(lb); fclose(fa); fclose(fb);
    return different ? 1 : 0;
}

static int parse_query_type(const char *cmd_line) {
    while (*cmd_line == ' ' || *cmd_line == '\t') cmd_line++;
    if (*cmd_line < '0' || *cmd_line > '9') return -1;
    int q = 0; while (*cmd_line >= '0' && *cmd_line <= '9') { q = q*10 + (*cmd_line - '0'); cmd_line++; }
    return q;
}

static int run_principal_in_dir(const char *principal_path, const char *workdir, const char *dataset_dir, const char *input_path, double *elapsed_sec, long *ru_kb_out, int suppress_stderr) {
    struct timespec ts1, ts2; clock_gettime(CLOCK_REALTIME, &ts1);
    pid_t pid = fork();
    if (pid == 0) {
        if (chdir(workdir) != 0) { perror("chdir workdir"); _exit(127); }
        if (suppress_stderr) {
            int devnull = open("/dev/null", O_WRONLY);
            if (devnull >= 0) { dup2(devnull, STDERR_FILENO); close(devnull); }
        }
        execl(principal_path, "programa-principal", dataset_dir, input_path, (char*)NULL);
        perror("exec programa-principal");
        _exit(127);
    } else if (pid < 0) {
        perror("fork");
        return -1;
    } else {
        int status = 0; struct rusage ru; memset(&ru, 0, sizeof(ru));
        if (wait4(pid, &status, 0, &ru) < 0) {
            perror("wait4");
        }
        clock_gettime(CLOCK_REALTIME, &ts2);
        if (elapsed_sec) *elapsed_sec = (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec)/1e9;
        if (ru_kb_out) *ru_kb_out = ru.ru_maxrss;
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <pasta-csv> <ficheiro-comandos> <pasta-outputs-esperados>\n", argv[0]);
        return 1;
    }

    const char *dataset_dir = argv[1];
    const char *commands_path = argv[2];
    const char *expected_dir = argv[3];

    char dataset_abs[PATH_MAX];
    char input_abs[PATH_MAX];
    char expected_abs[PATH_MAX];
    const char *dataset_use = realpath(dataset_dir, dataset_abs) ? dataset_abs : dataset_dir;
    const char *input_use   = realpath(commands_path, input_abs) ? input_abs : commands_path;
    const char *expected_use= realpath(expected_dir, expected_abs) ? expected_abs : expected_dir;

    char self_path[PATH_MAX]; ssize_t len = readlink("/proc/self/exe", self_path, sizeof(self_path)-1);
    if (len < 0) { perror("readlink /proc/self/exe"); return 1; }
    self_path[len] = '\0';
    char *self_dir = dirname(self_path);

    char principal_path[PATH_MAX]; snprintf(principal_path, sizeof(principal_path), "%s/%s", self_dir, "programa-principal");

    const int MAX_Q = 128;
    long total_per_q[MAX_Q]; memset(total_per_q, 0, sizeof(total_per_q));
    long ok_per_q[MAX_Q]; memset(ok_per_q, 0, sizeof(ok_per_q));
    double time_sum_per_q[MAX_Q]; memset(time_sum_per_q, 0, sizeof(time_sum_per_q));

    double total_time = 0.0;
    long max_rss_kb = 0;

    // 1) Executa o programa uma vez com o ficheiro completo para gerar todos os outputs
    // Executar em /tmp para evitar criar ficheiros/pastas dentro do repositório
    double full_elapsed = 0.0; long full_ru = 0;
    const char *tmpbase = getenv("TMPDIR"); if (!tmpbase) tmpbase = "/tmp";
    char full_tmp_tpl[PATH_MAX]; snprintf(full_tmp_tpl, sizeof(full_tmp_tpl), "%s/progtest_full_XXXXXX", tmpbase);
    char *full_tmpdir = mkdtemp(full_tmp_tpl);
    if (!full_tmpdir) {
        // fallback: usar self_dir se não for possível criar um temporário
        full_tmpdir = self_dir;
    }
    int rc = run_principal_in_dir(principal_path, full_tmpdir, dataset_use, input_use, &full_elapsed, &full_ru, 0);
    if (rc != 0) fprintf(stderr, "programa-principal terminou com código %d\n", rc);
    if (full_ru > max_rss_kb) max_rss_kb = full_ru;

    // 2) Ler comandos e comparar outputs produzidos vs esperados; acumular contagens por query
    FILE *cmdf = fopen(input_use, "r");
    if (!cmdf) { perror("Erro a abrir ficheiro de comandos"); return 1; }
    char *line = NULL; size_t n = 0; ssize_t r; long cmd_index = 0;
    while ((r = getline_trim_crlf(&line, &n, cmdf)) != -1) {
        if (r == 0) continue;
        cmd_index++;
        int qtype = parse_query_type(line);
        if (qtype < 0 || qtype >= MAX_Q) qtype = 0;
        total_per_q[qtype]++;
    // fich. produzidos localizam-se no diretório temporário onde corremos o programa
    char *produced_path = g_strdup_printf("%s/resultados/command%ld_output.txt", full_tmpdir, cmd_index);
        char *expected_path = g_strdup_printf("%s/command%ld_output.txt", expected_use, cmd_index);
        GSList *diffs = NULL;
        int cmp = compare_files(expected_path, produced_path, &diffs);
        if (cmp == 0) {
            ok_per_q[qtype]++;
        } else if (cmp == 1) {
            printf("Discrepâncias na query %ld:\n", cmd_index);
            for (GSList *it = diffs; it != NULL; it = it->next) {
                DiffEntry *d = (DiffEntry *)it->data;
                printf("   Linha %ld:\n", d->line);
                printf("      ESPERADO: %s\n", d->expected);
                printf("      PRODUZIDO: %s\n", d->produced);
            }
            printf("\n");
            printf(" ____________________________________________________________\n");
            printf("\n\n");
            

        } else {
            printf("Erro ao abrir ficheiros para comparar na query %ld (produzido: %s, esperado: %s)\n", cmd_index, produced_path, expected_path);
            printf("\n");
        }
        g_slist_free_full(diffs, free_diff_entry);
        g_free(produced_path);
        g_free(expected_path);
    }
    free(line); fclose(cmdf);

    total_time += full_elapsed;

    // cleanup: remover diretório temporário do full run se foi criado em /tmp
    if (full_tmpdir != self_dir) {
        char rmcmd_full[PATH_MAX + 16]; snprintf(rmcmd_full, sizeof(rmcmd_full), "rm -rf '%s'", full_tmpdir);
        int sysrc_full = system(rmcmd_full);
        if (sysrc_full == -1) {
            perror("system rm -rf full_tmpdir");
        }
    }

    // 3) Medir tempo médio por tipo de query: filtrar e executar por tipo separadamente
    for (int q = 1; q < MAX_Q; ++q) {
        if (total_per_q[q] == 0) continue;
        // criar ficheiro temporário (mkstemp) com apenas esse tipo, em /tmp
        char tmp_cmd_tpl[PATH_MAX]; snprintf(tmp_cmd_tpl, sizeof(tmp_cmd_tpl), "%s/progtest_q%d_cmd_XXXXXX", tmpbase, q);
        int tmpfd = mkstemp(tmp_cmd_tpl);
        if (tmpfd < 0) break;
        FILE *in = fopen(commands_path, "r"); if (!in) { close(tmpfd); unlink(tmp_cmd_tpl); break; }
        FILE *out = fdopen(tmpfd, "w"); if (!out) { close(tmpfd); fclose(in); unlink(tmp_cmd_tpl); break; }
        char *l2 = NULL; size_t n2 = 0; ssize_t r2; int kept = 0;
        while ((r2 = getline_trim_crlf(&l2, &n2, in)) != -1) {
            if (r2 == 0) continue;
            int qt = parse_query_type(l2);
            if (qt == q) { fprintf(out, "%s\n", l2); kept++; }
        }
        free(l2); fclose(in); fflush(out); fclose(out);
        if (kept == 0) { unlink(tmp_cmd_tpl); continue; }
        // executar num diretório temporário (em /tmp) para não interferir na pasta resultados principal
        char tmpdir_tpl[PATH_MAX]; snprintf(tmpdir_tpl, sizeof(tmpdir_tpl), "%s/progtest_q%d_XXXXXX", tmpbase, q);
        char *tmpdir = mkdtemp(tmpdir_tpl);
        if (!tmpdir) tmpdir = self_dir; // fallback
        double el = 0.0; long ru_kb = 0;
        run_principal_in_dir(principal_path, tmpdir, dataset_use, tmp_cmd_tpl, &el, &ru_kb, 1);
        time_sum_per_q[q] = (double)el; // total tempo para esse tipo
        if (ru_kb > max_rss_kb) max_rss_kb = ru_kb;
        // limpar ficheiro de comandos temporário e o diretório temporário criado
        unlink(tmp_cmd_tpl);
        if (tmpdir != self_dir) {
            char rmcmd[PATH_MAX + 16]; snprintf(rmcmd, sizeof(rmcmd), "rm -rf '%s'", tmpdir);
            int sysrc = system(rmcmd);
            if (sysrc == -1) {
                perror("system rm -rf tmpdir");
            }
        }
    }

    // sumario por query
    for (int q = 1; q < MAX_Q; ++q) {
        if (total_per_q[q] > 0) {
            printf("Q%d: %ld de %ld testes ok\n", q, ok_per_q[q], total_per_q[q]);
        }
    }

    // Memória utilizada
    printf("Memória utilizada: %.0fMB\n", max_rss_kb / 1024.0);

    // Tempos de execução
    printf("Tempos de execução:\n");
    for (int q = 1; q < MAX_Q; ++q) {
        if (total_per_q[q] > 0) {
            double avg_ms = (time_sum_per_q[q] / (double)total_per_q[q]) * 1000.0;
            printf("Q%d: %.1f ms\n", q, avg_ms);
        }
    }
    printf("Tempo total: %.0fs\n", total_time);

    return 0;
}
