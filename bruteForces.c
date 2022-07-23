//
// AED, November 2021
//
// Solution of the first practical assignement (subset sum problem)
//
// Daniel Ferreira – 102442
// Pedro Rasinhas – 103541
// Guilherme Antunes – 103600
//

#if __STDC_VERSION__ < 199901L
#error "This code must must be compiled in c99 mode or later (-std=c99)" // to handle the unsigned long long data type
#endif
#ifndef STUDENT_H_FILE
#define STUDENT_H_FILE "000000.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include "elapsed_time.h"
#include STUDENT_H_FILE

void progConfig()
{
    double time = cpu_time();
    fprintf(stderr, "Program configuration:\n");
    fprintf(stderr, "  min_n ....... %d\n", min_n);
    fprintf(stderr, "  max_n ....... %d\n", max_n);
    fprintf(stderr, "  n_sums ...... %d\n", n_sums);
    fprintf(stderr, "  n_problems .. %d\n", n_problems);
    fprintf(stderr, "  integer_t ... %d bits\n", 8 * (int)sizeof(integer_t));

    for (int i = 0; i < n_problems; i++)
    {
        int n = all_subset_sum_problems[i].n;

        if (n > 25)
            continue;
        integer_t *ptr = all_subset_sum_problems[i].p;

        printf("\nPara o problema %d: \n", n);
        printf("Os valores de p são: ");

        for (int c = 0; c < n; c++)
        {

            printf("%lld, ", ptr[c]);
        }
        printf("\n\n");
        printf("Os valores das somas são: ");

        for (int s = 0; s < n_sums; s++)
        {

            printf("%lld, ", all_subset_sum_problems[i].sums[s]);
        }
        printf("\n\n");
    }
    printf("progConfig %f\n", cpu_time() - time);
}

void showbits(integer_t x, int total_bits)
{
    for (int i = total_bits - 1; i >= 0; i--)
    {
        putchar(x & (1u << i) ? '1' : '0');
    }
}

void showbits2(integer_t x, int total_bits)
{
    for (int i = 0; i <= total_bits - 1; i++)
    {
        putchar(x & (1u << i) ? '1' : '0');
    }
}

int bruteforce_rec1(int n, integer_t *p, int currIdx, integer_t partialSum, integer_t desiredSum, integer_t mask, integer_t *comb)
{
    if (partialSum == desiredSum)
    {
        *comb = mask;
        return 1;
    }

    if (currIdx == n)
        return 0;

    if (bruteforce_rec1(n, p, currIdx + 1, partialSum + p[currIdx], desiredSum, (mask | (1 << currIdx)), comb))
        return 1;
    else
        return bruteforce_rec1(n, p, currIdx + 1, partialSum, desiredSum, mask, comb);
}

int bruteforce_rec2(int n, integer_t *p, int currIdx, integer_t partialSum, integer_t desiredSum, integer_t mask, integer_t *comb)
{
    if (partialSum > desiredSum)
        return 0;

    if (partialSum == desiredSum)
    {
        *comb = mask;
        return 1;
    }

    if (currIdx < 0)
        return 0;

    if (bruteforce_rec2(n, p, currIdx - 1, partialSum + p[currIdx], desiredSum, (mask | (1 << (currIdx))), comb))
        return 1;

    else
        return bruteforce_rec2(n, p, currIdx - 1, partialSum, desiredSum, mask, comb);
}

int main(void)
{
    progConfig();

    int func = 2;
    // 1 - > Recursiva
    // 2 - > Recursiva OTIMIZADA

    if (func == 1)
    {
        for (int i = 0; i < n_problems; i++)
        {
            if (i > 27)
                continue;

            unsigned int n = all_subset_sum_problems[i].n;
            integer_t *p = all_subset_sum_problems[i].p;
            double somaTempos = 0;
            double tempoMaximo = 0;

            for (int j = 0; j < n_sums; j++)
            {
                integer_t desiredSum = all_subset_sum_problems[i].sums[j];
                integer_t comb = 0;

                double time = cpu_time();

                if (bruteforce_rec1(n, p, 0, 0, desiredSum, 0, &comb))
                {
                    showbits(comb, n);
                    printf("\n");
                }
                else
                {
                    printf("\nSoluçao NÃO encontrada!\n");
                }
                time = cpu_time() - time;
                somaTempos += time;
                if (time > tempoMaximo)
                    tempoMaximo = time;
            }

            double tempoMedio = somaTempos / n_sums;
            printf("%d %f %f\n", i + 10, tempoMedio, tempoMaximo);
        }
    }

    if (func == 2)
    {
        for (int i = 0; i < n_problems; i++)
        {
            if (i > 27)
                continue;

            unsigned int n = all_subset_sum_problems[i].n;
            integer_t *p = all_subset_sum_problems[i].p;
            double somaTempos = 0;
            double tempoMaximo = 0;

            for (int j = 0; j < n_sums; j++)
            {
                integer_t desiredSum = all_subset_sum_problems[i].sums[j];
                integer_t comb = 0;

                double time = cpu_time();

                if (bruteforce_rec2(n, p, n - 1, 0, desiredSum, 0, &comb))
                {
                    showbits2(comb, n);
                    printf("\n");
                }
                else
                {
                    printf("\nSoluçao NÃO encontrada!\n");
                }
                time = cpu_time() - time;
                somaTempos += time;
                if (time > tempoMaximo)
                    tempoMaximo = time;
            }

            double tempoMedio = somaTempos / n_sums;
            printf("%d %f %f\n", i + 10, tempoMedio, tempoMaximo);
        }
    }

    return 0;
}
