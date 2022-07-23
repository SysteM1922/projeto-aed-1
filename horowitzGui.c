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

void insertion_sort(integer_t *data, int first, int one_after_last, integer_t *bits)
{
    int i, j;
    for (i = first + 1; i < one_after_last; i++)
    {
        integer_t temp = data[i];
        integer_t temp2 = bits[i];
        for (j = i; j > first && temp < data[j - 1]; j--)
        {
            data[j] = data[j - 1];
            bits[j] = bits[j - 1];
        }
        data[j] = temp;
        bits[j] = temp2;
    }
}

void Merge(integer_t *data, int first, int one_after_last, integer_t *bits)
{
    int i, j, k, middle;
    integer_t *buffer;
    integer_t *buffer2;
    if (one_after_last - first < 40)
        insertion_sort(data, first, one_after_last, bits);
    else
    {
        middle = (first + one_after_last) / 2;
        Merge(data, first, middle, bits);
        Merge(data, middle, one_after_last, bits);
        buffer = (integer_t *)malloc((size_t)(one_after_last - first) * sizeof(integer_t)) - first;
        buffer2 = (integer_t *)malloc((size_t)(one_after_last - first) * sizeof(integer_t)) - first;
        i = first;
        j = middle;
        k = first;
        while (k < one_after_last)
            if (j == one_after_last || (i < middle && data[i] <= data[j]))
            {
                buffer2[k] = bits[i];
                buffer[k++] = data[i++];
            }
            else
            {
                buffer2[k] = bits[j];
                buffer[k++] = data[j++];
            }
        for (i = first; i < one_after_last; i++)
        {
            data[i] = buffer[i];
            bits[i] = buffer2[i];
        }
        free(buffer + first);
        free(buffer2 + first);
    }
}

void somas(int n, integer_t *vals, integer_t *sums, integer_t *bits)
{
    integer_t som, comb;
    for (integer_t a = 0; a < 1 << n; a++)
    {
        som = 0;
        comb = 0;
        for (int i = 0; i < n; i++)
        {
            if (a & (1 << i))
            {
                som += vals[i];
                comb = comb | (1 << i);
            }
        }
        sums[a] = som;
        bits[a] = comb;
    }
}

int horowitz(integer_t desired_sum, int sizeA, int sizeB, integer_t sumsA[], integer_t sumsB[], integer_t bitsA[], integer_t bitsB[], int nA, int nB)
{
    sumsB = &sumsB[(int)sizeB - 1];
    bitsB = &bitsB[(int)sizeB - 1];
    while (sumsA < &sumsA[(int)sizeA] || sumsB > &sumsB[0])
    {
        if (desired_sum == (*sumsA + *sumsB))
            break;
        else if (desired_sum > (*sumsA + *sumsB))
        {
            sumsA++;
            bitsA++;
        }
        else
        {
            sumsB--;
            bitsB--;
        }
    }
    if (desired_sum == (*sumsA + *sumsB))
    {

        integer_t b = *bitsA | (*bitsB << nA);
        for (int i = 0; i < nA + nB; i++)
        {
            printf("%s", b & 1 ? "1" : "0");
            b = b >> 1;
        }
        printf("\n");
        return 1;
    }
    else
        return 0;
}

int main(void)
{
    progConfig();

    for (int i = 0; i < n_problems; i++)
    {
        if (i >= 0)
        {
            int n = all_subset_sum_problems[i].n;
            integer_t *p = all_subset_sum_problems[i].p;
            double somaTempos = 0;
            double tempoMaximo = 0;

            int nA = n / 2;
            int nB = n - nA;

            p = all_subset_sum_problems[i].p;

            int sizeA = 1 << nA;
            int sizeB = 1 << nB;

            integer_t *sumsA = malloc(sizeA * sizeof(integer_t));
            integer_t *sumsB = malloc(sizeB * sizeof(integer_t));

            integer_t *bitsA = malloc(sizeA * sizeof(integer_t));
            integer_t *bitsB = malloc(sizeB * sizeof(integer_t));
            double time2 = cpu_time();
            somas(nA, p, sumsA, bitsA);
            somas(nB, p + nA, sumsB, bitsB);

            Merge(sumsA, 0, sizeA - 1, bitsA);
            Merge(sumsB, 0, sizeB - 1, bitsB);
            time2 = cpu_time() - time2;

            for (int j = 0; j < n_sums; j++)
            {
                integer_t desired_sum = all_subset_sum_problems[i].sums[j];

                double time = cpu_time();
                if (!horowitz(desired_sum, sizeA, sizeB, sumsA, sumsB, bitsA, bitsB, nA, nB))
                {
                    printf("\nSoluçao NÃO encontrada!\n");
                    break;
                }
                time = cpu_time() - time;
                somaTempos += time;
                if (time > tempoMaximo)
                    tempoMaximo = time;
            }
            free(sumsA);
            free(sumsB);
            free(bitsA);
            free(bitsB);
            double tempoMedio = somaTempos / n_sums;
            printf("%d %f %f\n", i + 10, tempoMedio+time2, tempoMaximo+time2);
        }
    }
    return 0;
}